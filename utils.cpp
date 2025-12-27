#include "utils.hpp"
#include <windows.h>
#include <algorithm>
#include <cstring>

using namespace std;

// Функция для преобразования UTF-8 в Windows-1251
string utf8_to_cp1251(const string& utf8) {
    if (utf8.empty()) return "";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), NULL, 0);
    if (size_needed == 0) return "";

    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wstr[0], size_needed);

    int size_needed2 = WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    if (size_needed2 == 0) return "";

    string str(size_needed2, 0);
    WideCharToMultiByte(1251, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed2, NULL, NULL);

    return str;
}

// Функция для корректного получения строки с преобразованием кодировки
string get_cell_value(const pqxx::field& field) {
    const char* utf8_str = field.c_str();
    if (!utf8_str) {
        return "";
    }

    string utf8_value = utf8_str;
    return utf8_to_cp1251(utf8_value);
}

// Функция для автоматического определения ширины колонок с учетом кодировки
vector<int> calculate_column_widths(const pqxx::result& R) {
    vector<int> widths(R.columns(), 0);

    // Проверяем заголовки
    for (pqxx::row::size_type i = 0; i < R.columns(); ++i) {
        string column_name = utf8_to_cp1251(R.column_name(i));
        widths[i] = max(widths[i], (int)column_name.length());
    }

    // Проверяем данные
    for (pqxx::row const& row : R) {
        for (pqxx::row::size_type col = 0; col < row.size(); ++col) {
            string cell_value = get_cell_value(row[col]);
            widths[col] = max(widths[col], (int)cell_value.length());
        }
    }

    // Добавляем отступ
    for (auto& w : widths) {
        w += 2;
    }

    return widths;
}

bool string_to_bool(const string& str) 
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) { return tolower(c); });

    if (lowerStr == "t" || lowerStr == "true" || lowerStr == "1" ||
        lowerStr == "yes" || lowerStr == "да" || lowerStr == "y") {
        return true;
    }

    if (lowerStr == "f" || lowerStr == "false" || lowerStr == "0" ||
        lowerStr == "no" || lowerStr == "нет" || lowerStr == "n") {
        return false;
    }

    // По умолчанию возвращаем false
    return false;
}