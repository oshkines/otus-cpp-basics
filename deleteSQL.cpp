#include "deleteSQL.hpp"
#include "connectionmanager.hpp"
#include "common.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <pqxx/pqxx> 

using namespace std;

bool deleteSql(ConnectionManager& dbManager, string code) {
	string baseRequest = "DELETE FROM  _Reference541 WHERE _code = $1"; 
	//string connection_string = "dbname=ERP user=postgres password=1978 host=127.0.0.1 port=5432";
    try {
        //pqxx::connection C(connection_string);
        ////cout << "Успешное подключение к базе данных: " << C.dbname() << endl;

        //pqxx::work W(C);

        //auto conn = ConnectionManager::getInstance().getConnection();
        auto conn = dbManager.getConnection();
        if (!conn || !conn->is_open()) {
            cout << "Database connection failed." << endl;
            return false;
        }

        pqxx::work W(*conn);

        // Выводим запрос для отладки
        cout << "SQL запрос: " << baseRequest << endl;
        cout << "Параметр code: " << code << endl;

        pqxx::result R = W.exec_params(baseRequest, code);
        W.commit();
        cout << "Rows deleted: " << R.affected_rows() << endl;
        return true;
    }
    catch (const exception& e)
    {
        //string error_msg = utf8_to_cp1251(e.what());
        cout << "Connection or request error: " << e.what() << endl;
        return false;  // Возвращаем пустой вектор при ошибке
    }
}