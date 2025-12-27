#include "consoleOutput.hpp"
#include "common.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  
#include <iomanip>    
#include <windows.h>
#include <cstring> 

using namespace std;

//// Функция для преобразования UTF-8 в Windows-1251
//string utf8_to_cp1251(const string& utf8) {
//    if (utf8.empty()) return "";
//
//    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), NULL, 0);
//    if (size_needed == 0) return "";
//
//    wstring wstr(size_needed, 0);
//    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wstr[0], size_needed);
//
//    int size_needed2 = WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
//    if (size_needed2 == 0) return "";
//
//    string str(size_needed2, 0);
//    WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed2, NULL, NULL);
//
//    return str;
//}
//
//// Функция для корректного получения строки с преобразованием кодировки
//string get_cell_value(const pqxx::field& field) {
//    const char* utf8_str = field.c_str();
//    if (!utf8_str) {
//        return "";
//    }
//
//    string utf8_value = utf8_str;
//    return utf8_to_cp1251(utf8_value);
//}
//
//// Функция для автоматического определения ширины колонок с учетом кодировки
//vector<int> calculate_column_widths(const pqxx::result& R) {
//    vector<int> widths(R.columns(), 0);
//
//    // Проверяем заголовки
//    for (pqxx::row::size_type i = 0; i < R.columns(); ++i) {
//        string column_name = utf8_to_cp1251(R.column_name(i));
//        widths[i] = max(widths[i], (int)column_name.length());
//    }
//
//    // Проверяем данные
//    for (pqxx::row const& row : R) {
//        for (pqxx::row::size_type col = 0; col < row.size(); ++col) {
//            string cell_value = get_cell_value(row[col]);
//            widths[col] = max(widths[col], (int)cell_value.length());
//        }
//    }
//
//    // Добавляем отступ
//    for (auto& w : widths) {
//        w += 2;
//    }
//
//    return widths;
//}

std::vector<Record> OutputConsole(const pqxx::result& R, int colRecords)
{
    vector<Record> allRecords;
    allRecords.reserve(colRecords);

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Если хотите использовать UTF-8 в консоли (Windows 10+), то раскомментируйте эти строки:
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    // И уберите вызовы utf8_to_cp1251() из функций get_cell_value() и calculate_column_widths()

    cout << "\nQuery result:" << endl;

    // Проверяем, есть ли результаты
    if (R.empty()) {
        cout << "No records found." << endl;
        return allRecords;
    }
    else
    {
        vector<int> col_widths = calculate_column_widths(R);

        // Заголовки
        for (pqxx::row::size_type col = 0; col < R.columns(); ++col)
        {
            int length = col_widths[col] > 25 ? 25 : col_widths[col];
            string column_name = utf8_to_cp1251(R.column_name(col));
            cout << left << setw(length) << column_name << " ";
        }
        cout << endl;

        // Разделитель
        for (int width : col_widths)
        {
            int length = width > 25 ? 25 : width;
            cout << string(length, '-') << " ";
        }
        cout << endl;

        // Данные
        for (pqxx::row const& row : R)
        {
            Record record;
            for (pqxx::row::size_type col = 0; col < row.size(); ++col)
            {
                int length = col_widths[col] > 25 ? 25 : col_widths[col];

                // Получаем значение как строку с правильной кодировкой
                string currentValue = get_cell_value(row[col]);

                // Сохраняем в структуру Record
                if (!currentValue.empty())
                {
                    switch (col)
                    {
                    case 0: record.code = currentValue; break;
                    case 1: record.name = currentValue; break;
                    case 2: 
                        if (currentValue == "t" || currentValue == "true" || currentValue == "1" || currentValue == "Yes") 
                        {
                            record.isKlient = true;
                        }
                        else 
                        {
                            record.isKlient = false;
                        }
                        break;
                    case 3: 
                        if (currentValue == "t" || currentValue == "true" || currentValue == "1" || currentValue == "Yes") 
                        {
                            record.isPostavshik = true;
                        }
                        else 
                        {
                            record.isPostavshik = false;
                        }
                        break;
                    case 4: record.birthdate = currentValue; break;
                    case 5: record.typeKI = currentValue; break;
                    case 6: record.kontactInformation = currentValue; break;
                    }
                }

                std::string Phone = "\\xa873cb4ad71d17b2459f9a70d4e2da66";
                std::string Address = "\\xae8167157822c4b643d29fdc57b31a5d";
                std::string Email = "\\x82e6d573ee35d0904bf4d326a84a91d2";

                if (currentValue == Phone)
                {
                    currentValue = "Phone";
                }
                else if (currentValue == Address)
                {
                    currentValue = "Address";
                }
                else if (currentValue == Email)
                {
                    currentValue = "Email";
                }

                if (col == 2 || col == 3) {
                    if (col == 2 && record.isKlient) {
                        currentValue = "Yes"; 
                    }
                    else if (col == 2 && !record.isKlient) {
                        currentValue = "No"; 
                    }
                    else if (col == 3 && record.isPostavshik) {
                        currentValue = "Yes"; 
                    }
                    else if (col == 3 && !record.isPostavshik) {
                        currentValue = "No"; 
                    }
                }

                // Обрезаем слишком длинные строки для вывода
                string displayValue = currentValue;
                if (displayValue.length() > length - 1)
                {
                    displayValue = displayValue.substr(0, length - 4) + "...";
                }

                // Выводим значение
                cout << left << setw(length) << displayValue << " ";
            }
            allRecords.push_back(record);
            cout << endl;
        }

        // Выводим количество найденных записей
        cout << "\nTotal records found: " << R.size() << endl;
    }
    return allRecords;
}
