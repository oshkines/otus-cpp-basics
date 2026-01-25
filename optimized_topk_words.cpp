#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <queue>
#include <condition_variable>
#include <sstream>

const size_t TOPK = 10;
const size_t SMALL_FILE_THRESHOLD = 10 * 1024;      // 10 KB порог для маленьких файлов
const size_t LARGE_FILE_THRESHOLD = 100 * 1024;     // 100 KB порог для больших файлов
const size_t CHUNK_SIZE = 100 * 1024;               // 100 KB размер чанка

using Counter = std::map<std::string, std::size_t>;

std::string tolower(const std::string &str);
Counter count_words_in_stream(std::istream& stream);
Counter count_words_in_chunk(const std::string& chunk);
void merge_counters(Counter& dest, const Counter& src);
void print_topk(std::ostream& stream, const Counter& counter, const size_t k);
size_t find_word_boundary(const std::string& data, size_t pos);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();
    
    // Определяем количество потоков
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 2;  // Для тестов 1-2 MB лучше 2 потока
    }
    
    // Ограничиваем число потоков для небольших файлов
    if (static_cast<size_t>(argc - 1) < num_threads) {
        num_threads = argc - 1;
    }
    
    std::cout << "Using " << num_threads << " threads for " << (argc - 1) << " file(s)\n";
    
    // Глобальный счетчик и мьютекс
    Counter global_counter;
    std::mutex global_counter_mutex;
    
    // Вектор для хранения потоков
    std::vector<std::future<void>> futures;
    
    // Разделяем файлы между потоками
    std::vector<std::vector<std::string>> thread_files(num_threads);
    
    for (int i = 1; i < argc; ++i) {
        thread_files[(i - 1) % num_threads].push_back(argv[i]);
    }
    
    // Запускаем потоки для обработки своих наборов файлов
    for (size_t i = 0; i < num_threads; ++i) {
        if (!thread_files[i].empty()) {
            futures.push_back(std::async(std::launch::async, 
                [&thread_files, i, &global_counter, &global_counter_mutex]() {
                    
                    Counter thread_counter;
                    
                    // Обрабатываем все файлы, назначенные этому потоку
                    for (const auto& filename : thread_files[i]) {
                        std::ifstream file(filename, std::ios::binary | std::ios::ate);
                        if (!file.is_open()) {
                            std::cerr << "Failed to open file " << filename << " in thread\n";
                            continue;
                        }
                        
                        size_t file_size = file.tellg();
                        file.close();
                        
                        // Маленький файл (< 10 KB) - обрабатываем целиком
                        if (file_size <= SMALL_FILE_THRESHOLD) {
                            std::ifstream input(filename);
                            if (input.is_open()) {
                                auto file_counter = count_words_in_stream(input);
                                merge_counters(thread_counter, file_counter);
                            }
                        }
                        // Средний файл (10-100 KB) - тоже целиком, но в отдельной задаче
                        else if (file_size <= LARGE_FILE_THRESHOLD) {
                            std::ifstream input(filename);
                            if (input.is_open()) {
                                auto file_counter = count_words_in_stream(input);
                                merge_counters(thread_counter, file_counter);
                            }
                        }
                        // Большой файл (> 100 KB) - делим на чанки
                        else {
                            std::ifstream input(filename, std::ios::binary);
                            if (!input.is_open()) continue;
                            
                            // Читаем весь файл
                            std::string buffer;
                            buffer.resize(file_size);
                            input.read(&buffer[0], file_size);
                            input.close();
                            
                            // Делим на куски по 100 кб
                            size_t num_chunks = (file_size + CHUNK_SIZE - 1) / CHUNK_SIZE;
                            
                            for (size_t chunk_idx = 0; chunk_idx < num_chunks; ++chunk_idx) {
                                size_t start_pos = chunk_idx * CHUNK_SIZE;
                                size_t end_pos = std::min(start_pos + CHUNK_SIZE, file_size);
                                
                                // Корректируем границы по словам
                                if (chunk_idx > 0) {
                                    start_pos = find_word_boundary(buffer, start_pos);
                                    if (start_pos >= end_pos) continue;
                                }
                                
                                if (chunk_idx < num_chunks - 1) {
                                    end_pos = find_word_boundary(buffer, end_pos);
                                }
                                
                                if (end_pos > start_pos) {
                                    std::string chunk = buffer.substr(start_pos, end_pos - start_pos);
                                    auto chunk_counter = count_words_in_chunk(chunk);
                                    merge_counters(thread_counter, chunk_counter);
                                }
                            }
                        }
                    }
                    
                    // Объединяем результаты потока с глобальным счетчиком
                    {
                        std::lock_guard<std::mutex> lock(global_counter_mutex);
                        merge_counters(global_counter, thread_counter);
                    }
                }
            ));
        }
    }
    
    // Ждем завершения всех потоков
    for (auto& future : futures) {
        future.get();
    }
    
    print_topk(std::cout, global_counter, TOPK);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
    
    return EXIT_SUCCESS;
}

// Находит границу слова в строке начиная с позиции pos
size_t find_word_boundary(const std::string& data, size_t pos) {
    if (pos >= data.size()) return pos;
    
    // Если pos уже на пробеле, ищем начало следующего слова
    if (std::isspace(static_cast<unsigned char>(data[pos]))) {
        while (pos < data.size() && std::isspace(static_cast<unsigned char>(data[pos]))) {
            ++pos;
        }
        return pos;
    }
    
    // Если pos на середине слова, ищем конец этого слова
    while (pos < data.size() && !std::isspace(static_cast<unsigned char>(data[pos]))) {
        ++pos;
    }
    
    // Пропускаем пробелы до начала следующего слова
    while (pos < data.size() && std::isspace(static_cast<unsigned char>(data[pos]))) {
        ++pos;
    }
    
    return pos;
}

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
}

// Подсчет слов из потока
Counter count_words_in_stream(std::istream& stream) {
    Counter local_counter;
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&local_counter](const std::string &s) { 
                      ++local_counter[tolower(s)]; 
                  });
    return local_counter;
}

// Подсчет слов в каждом куске файла
Counter count_words_in_chunk(const std::string& chunk) {
    Counter local_counter;
    std::istringstream stream(chunk);
    
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&local_counter](const std::string &s) { 
                      ++local_counter[tolower(s)]; 
                  });
    
    return local_counter;
}

// Объединение счетчиков
void merge_counters(Counter& dest, const Counter& src) {
    for (const auto& item : src) {
        dest[item.first] += item.second;
    }
}

// Вывод том 10-ти слов
void print_topk(std::ostream& stream, const Counter& counter, const size_t k) 
{
    if (counter.empty()) 
    {
        stream << "No words found\n";
        return;
    }
    
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = counter.begin(); it != counter.end(); ++it) 
    {
        words.push_back(it);
    }

    std::partial_sort(
        words.begin(), words.begin() + std::min(k, words.size()), words.end(),
        [](Counter::const_iterator lhs, Counter::const_iterator rhs) 
        { 
            return lhs->second > rhs->second; 
        });

    std::cout << "\nTop " << std::min(k, words.size()) << " words:\n";
    for (size_t i = 0; i < std::min(k, words.size()); ++i) 
    {
        stream << std::setw(4) << words[i]->second << " " << words[i]->first << '\n';
    }
}