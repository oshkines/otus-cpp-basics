#include <exception>
#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <pqxx/pqxx> 

using namespace std;

bool deleteSql(string code) {
	string baseRequest = "DELETE FROM  _Reference541 WHERE _code = $1"; 
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