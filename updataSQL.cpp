#include <exception>
#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <pqxx/pqxx> 

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

bool updataSql(string code, std::map<std::string, std::string> fields) {

    string baseRequest;
    string startOfRequest = "UPDATE _Reference541 SET (";
    string requestCondition = ") = (";
    string requestEnd = ") WHERE _code = $1";

    string allFields{ "" }, allValue{""};

    for (const auto& [field, value] : fields) {
        if (value == "Yes" || value == "No") {
            allFields   =   (allFields  == "")  ? field : allFields + ", " + field;
            allValue    =   (allValue   == "")  ? "NOT " + field : allValue + ", NOT " + field;
        }
        else {
            allFields   =   (allFields  == "")  ? field : allFields + ", " + field;
            allValue    =   (allValue == "")    ? "'" + value + "'" : allValue + ", '" + value + "'";
        }
    }

    baseRequest = startOfRequest + allFields + requestCondition + allValue + requestEnd;

    string connection_string = "dbname=ERP user=postgres password=1978 host=127.0.0.1 port=5432";

    try {
        pqxx::connection C(connection_string);
        //cout << "Успешное подключение к базе данных: " << C.dbname() << endl;

        pqxx::work W(C);

        // Выводим запрос для отладки
        cout << "SQL запрос: " << baseRequest << endl;
        cout << "Параметр code: " << code << endl;

        pqxx::result R = W.exec_params(baseRequest, code);
        W.commit();
    }
    catch (const exception& e)
        {
            //string error_msg = utf8_to_cp1251(e.what());
            cout << "Connection or request error: " << e.what() << endl;
            return false;  // Возвращаем пустой вектор при ошибке
        }
    return true;
}