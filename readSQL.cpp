#include "readSQL.hpp"
#include "utils.hpp"
#include "connectionmanager.hpp"
#include <pqxx/pqxx> 
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  
#include <iomanip>    
#include <windows.h>
#include <cstring> 

using namespace std;

//// Не используем using namespace std в заголовочных файлах или .cpp файлах
//// Лучше явно указывать пространства имен
//
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

pqxx::result readSql(ConnectionManager& dbManager, int colRecords, string code)
{

    //vector<Record> allRecords;
    //allRecords.reserve(colRecords);

    string baseRequest;

    if (code == "")
    {
        baseRequest = "SELECT\
            _Reference541._code AS Code,\
            _Reference541._description AS Name,\
            _Reference541._fld55051 AS isKlient,\
            _Reference541._fld55053 AS isPostavshik,\
            _Reference541._fld55064 AS birthdate,\
            _reference541_vt55075._fld55077rref AS TypeKI,\
            _reference541_vt55075._fld55079 AS KontactInformation\
            FROM _Reference541\
            JOIN _reference541_vt55075 ON _Reference541._idrref = _reference541_vt55075._reference541_idrref\
            WHERE _marked = false\
            GROUP BY Code, Name, isKlient, isPostavshik, birthdate, TypeKI, KontactInformation\
            ORDER BY _code\
            LIMIT $1";
            //string baseRequest = "SELECT\
            //   _Reference541._code AS Code,\
            //   _Reference541._description AS Name,\
            //   CASE WHEN _Reference541._fld55051 THEN 'Да' ELSE 'Нет' END AS Klient,\
            //   CASE WHEN _Reference541._fld55053 THEN 'Да' ELSE 'Нет' END AS Postavshik\
            //   FROM _Reference541\
            //   WHERE _marked = false\
            //   LIMIT $1";
    }
    else
    {
        baseRequest = "SELECT\
            _Reference541._code AS Code,\
            _Reference541._description AS Name,\
            _Reference541._fld55051 AS isKlient,\
            _Reference541._fld55053 AS isPostavshik,\
            _Reference541._fld55064 AS birthdate,\
            _reference541_vt55075._fld55077rref AS TypeKI,\
            _reference541_vt55075._fld55079 AS KontactInformation\
            FROM _Reference541\
            JOIN _reference541_vt55075 ON _Reference541._idrref = _reference541_vt55075._reference541_idrref\
            WHERE _code = $1\
            GROUP BY Code, Name, isKlient, isPostavshik, birthdate, TypeKI, KontactInformation\
            ORDER BY _code";
    }

    //string connection_string = "dbname=ERP user=postgres password=1978 host=127.0.0.1 port=5432";

    try {
        //pqxx::connection C(connection_string);
        


        //pqxx::work W(C);
        auto conn = dbManager.getConnection();
        //auto conn = ConnectionManager::getInstance().getConnection();
        if (!conn || !conn->is_open()) {
            cout << "Database connection failed." << endl;
            return pqxx::result();
        }

        pqxx::work W(*conn);

        pqxx::result R;

        if (code == "") {
            // Выполняем параметризованный запрос
            R = W.exec_params(baseRequest, colRecords);
            //pqxx::result R = W.exec(baseRequest);
        }
        else
        {
            R = W.exec_params(baseRequest, code);
        }
        W.commit();
        cout << "\nThe transaction was completed successfully." << endl;

        return R;

        //SetConsoleOutputCP(1251);
        //SetConsoleCP(1251);

        //// Если хотите использовать UTF-8 в консоли (Windows 10+), то раскомментируйте эти строки:
        //// SetConsoleOutputCP(CP_UTF8);
        //// SetConsoleCP(CP_UTF8);
        //// И уберите вызовы utf8_to_cp1251() из функций get_cell_value() и calculate_column_widths()

        //cout << "\nQuery result:" << endl;

        //// Проверяем, есть ли результаты
        //if (R.empty()) {
        //    cout << "No records found." << endl;
        //}
        //else {
        //    vector<int> col_widths = calculate_column_widths(R);

        //    // Заголовки
        //    for (pqxx::row::size_type col = 0; col < R.columns(); ++col) {
        //        int length = col_widths[col] > 25 ? 25 : col_widths[col];
        //        string column_name = utf8_to_cp1251(R.column_name(col));
        //        cout << left << setw(length) << column_name << " ";
        //    }
        //    cout << endl;

        //    // Разделитель
        //    for (int width : col_widths) {
        //        int length = width > 25 ? 25 : width;
        //        cout << string(length, '-') << " ";
        //    }
        //    cout << endl;

        //    // Данные
        //    for (pqxx::row const& row : R) {
        //        Record record;
        //        for (pqxx::row::size_type col = 0; col < row.size(); ++col) {
        //            int length = col_widths[col] > 25 ? 25 : col_widths[col];

        //            // Получаем значение как строку с правильной кодировкой
        //            string currentValue = get_cell_value(row[col]);

        //            // Сохраняем в структуру Record
        //            if (!currentValue.empty()) {
        //                switch (col) {
        //                case 0: record.code = currentValue; break;
        //                case 1: record.name = currentValue; break;
        //                case 2: record.isKlient = currentValue; break;
        //                case 3: record.isPostavshik = currentValue; break;
        //                case 4: record.birthdate = currentValue; break;
        //                case 5: record.typeKI = currentValue; break;
        //                case 6: record.kontactInformation = currentValue; break;
        //                }
        //            }

        //            std::string Phone = "\\xa873cb4ad71d17b2459f9a70d4e2da66";
        //            std::string Address = "\\xae8167157822c4b643d29fdc57b31a5d";
        //            std::string Email = "\\x82e6d573ee35d0904bf4d326a84a91d2";

        //            if (currentValue == Phone)
        //            {
        //                currentValue = "Phone";
        //            }
        //            else if (currentValue == Address)
        //            {
        //                currentValue = "Address";
        //            }
        //            else if (currentValue == Email)
        //            {
        //                currentValue = "Email";
        //            }

        //            // Обрезаем слишком длинные строки для вывода
        //            string displayValue = currentValue;
        //            if (displayValue.length() > length - 1) {
        //                displayValue = displayValue.substr(0, length - 4) + "...";
        //            }

        //            // Выводим значение
        //            cout << left << setw(length) << displayValue << " ";
        //        }
        //        allRecords.push_back(record);
        //        cout << endl;
        //    }

        //    // Выводим количество найденных записей
        //    cout << "\nTotal records found: " << R.size() << endl;

        //    //// Пример использования сохраненных данных
        //    //cout << "\n=== An example of working with saved data ===" << endl;
        //    //cout << "First 5 entries:" << endl;
        //    //int count = min(5, (int)allRecords.size());
        //    //for (int i = 0; i < count; i++) {
        //    //    cout << i + 1 << ". Code: " << allRecords[i].code
        //    //        << " | Name: " << allRecords[i].name
        //    //        << " | Klient: " << allRecords[i].isKlient << endl;
        //    //}
        //}

        //W.commit();
        //cout << "\nThe transaction was completed successfully." << endl;
    }
    catch (const exception& e)
    {
        // Преобразуем сообщение об ошибке тоже
        string error_msg = utf8_to_cp1251(e.what());
        cout << "Connection or request error: " << error_msg << endl;
        //return allRecords;  // Возвращаем пустой вектор при ошибке
        return pqxx::result();
    }

    //return allRecords;
}