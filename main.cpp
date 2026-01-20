#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <thread>
#include <atomic>

#include "CRC32.hpp"
#include "IO.hpp"

/// @brief Переписывает последние 4 байта значением value
void replaceLastFourBytes(std::vector<char> &data, uint32_t value) {
  std::copy_n(reinterpret_cast<const char *>(&value), 4, data.end() - 4);
}

//TODO
struct ThreadData {
    const std::vector<char>* original;    // Указатель на оригинальные данные
    std::vector<char>* result;            // Указатель на результат (изменяется только последние 4 байта)
    const std::string* injection;         // Строка для вставки
    uint32_t start;                       // Начало диапазона перебора
    uint32_t end;                         // Конец диапазона перебора (не включая)
    uint32_t originalCrc32;               // Целевой CRC32
    std::atomic<bool>* found;             // Флаг нахождения решения
    std::atomic<uint32_t>* solution;      // Найденное значение
    std::atomic<uint32_t>* progressCounter; // Счетчик прогресса
};

void threadWorker(ThreadData data) {
    // Создаем локальную копию данных для этого потока
    std::vector<char> localResult = *data.result;
    
    // Перебираем свой диапазон значений
    for (uint32_t i = data.start; i < data.end && !data.found->load(); ++i) {
        replaceLastFourBytes(localResult, i);
        uint32_t currentCrc32 = crc32(localResult.data(), localResult.size());
        
        if (currentCrc32 == data.originalCrc32) {
            // Сохраняем найденное решение
            data.solution->store(i);
            data.found->store(true);
            return;
        }
        
        // Обновляем прогресс каждые 1000 итераций
        if ((i - data.start) % 1000 == 0) {
            (*data.progressCounter) += 1000;
        }
    }
}
//FIXME

/**
 * @brief Формирует новый вектор с тем же CRC32, добавляя в конец оригинального
 * строку injection и дополнительные 4 байта
 * @details При формировании нового вектора последние 4 байта не несут полезной
 * нагрузки и подбираются таким образом, чтобы CRC32 нового и оригинального
 * вектора совпадали
 * @param original оригинальный вектор
 * @param injection произвольная строка, которая будет добавлена после данных
 * оригинального вектора
 * @return новый вектор
 */
//TODO
// std::vector<char> hack(const std::vector<char> &original,
//                        const std::string &injection) {
//   const uint32_t originalCrc32 = crc32(original.data(), original.size());

//   std::vector<char> result(original.size() + injection.size() + 4);
//   auto it = std::copy(original.begin(), original.end(), result.begin());
//   std::copy(injection.begin(), injection.end(), it);

//   /*
//    * Внимание: код ниже крайне не оптимален.
//    * В качестве доп. задания устраните избыточные вычисления
//    */
//   const size_t maxVal = std::numeric_limits<uint32_t>::max();
//   for (size_t i = 0; i < maxVal; ++i) {
//     // Заменяем последние четыре байта на значение i
//     replaceLastFourBytes(result, uint32_t(i));
//     // Вычисляем CRC32 текущего вектора result
//     auto currentCrc32 = crc32(result.data(), result.size());

//     if (currentCrc32 == originalCrc32) {
//       std::cout << "Success\n";
//       return result;
//     }
//     // Отображаем прогресс
//     if (i % 1000 == 0) {
//       std::cout << "progress: "
//                 << static_cast<double>(i) / static_cast<double>(maxVal)
//                 << std::endl;
//     }
//   }
//   throw std::logic_error("Can't hack");
// }

std::vector<char> hack(const std::vector<char> &original,
                       const std::string &injection) {
    const uint32_t originalCrc32 = crc32(original.data(), original.size());
    
    // Создаем результат: оригинал + строка + 4 байта
    std::vector<char> result(original.size() + injection.size() + 4);
    auto it = std::copy(original.begin(), original.end(), result.begin());
    std::copy(injection.begin(), injection.end(), it);
    
    // Количество потоков
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 4; // fallback
    }
    
    // Диапазон значений для перебора
    const uint64_t maxVal = std::numeric_limits<uint32_t>::max();
    const uint64_t rangeSize = maxVal / numThreads;
    
    // Атомарные переменные для синхронизации
    std::atomic<bool> found(false);           //флаг нахождения решения
    std::atomic<uint32_t> solution(0);        //найденное значение
    std::atomic<uint32_t> progressCounter(0); //счетчик прогресса
    
    // Вектор потоков и данных для них
    std::vector<std::thread> threads;
    std::vector<ThreadData> threadData(numThreads);
    
    // Создаем потоки
    //Каждый поток работает с локальной копией вектора result
    for (unsigned int i = 0; i < numThreads; ++i) {
        threadData[i].original = &original;
        threadData[i].result = &result;
        threadData[i].injection = &injection;
        threadData[i].originalCrc32 = originalCrc32;
        threadData[i].found = &found;
        threadData[i].solution = &solution;
        threadData[i].progressCounter = &progressCounter;
        
        // Распределяем диапазоны
        threadData[i].start = static_cast<uint32_t>(i * rangeSize);
        threadData[i].end = (i == numThreads - 1) 
                          ? static_cast<uint32_t>(maxVal) + 1  // Включая последнее значение
                          : static_cast<uint32_t>((i + 1) * rangeSize);
        
        threads.emplace_back(threadWorker, threadData[i]);
    }
    
    // Поток для отображения прогресса
    std::thread progressThread([&found, &progressCounter, maxVal]() {
        while (!found.load()) {
            uint32_t currentProgress = progressCounter.load();
            std::cout << "progress: "
                      << static_cast<double>(currentProgress) / static_cast<double>(maxVal)
                      << std::endl;
            
            // Ждем немного перед следующим обновлением
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    
    // Ждем завершения рабочих потоков
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Ждем завершения потока прогресса
    progressThread.join();
    
    if (found.load()) {
        std::cout << "Success! Found value: " << solution.load() << std::endl;
        replaceLastFourBytes(result, solution.load());
        return result;
    }
    
    throw std::logic_error("Can't hack");
}

//FIXME

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Call with two args: " << argv[0]
              << " <input file> <output file>\n";
    return 1;
  }

  try {
    const std::vector<char> data = readFromFile(argv[1]);
    const std::vector<char> badData = hack(data, "He-he-he");
    writeToFile(argv[2], badData);
  } catch (std::exception &ex) {
    std::cerr << ex.what() << '\n';
    return 2;
  }
  return 0;
}
