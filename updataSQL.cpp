#include "updataSQL.hpp"
#include "connectionmanager.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <windows.h>
#include <map>
#include <pqxx/pqxx> 

using namespace std;

// Вспомогательная функция для обновления полей по отдельности
bool updateFieldsSeparately(pqxx::work& W, const string& code, const map<string, string>& fields) {
    try {
        for (const auto& [field, value] : fields) {
            string sql = "UPDATE _Reference541 SET " + field + " = $2 WHERE _code = $1";

            if (value == "Yes" || value == "No") {
                bool newValue = (value == "Yes");
                W.exec_params(sql, code, newValue);
            }
            else if (value == "true" || value == "false") {
                bool newValue = (value == "true");
                W.exec_params(sql, code, newValue);
            }
            else {
                W.exec_params(sql, code, value);
            }

            cout << "Updated " << field << " for code " << code << endl;
        }
        return true;
    }
    catch (const exception& e) {
        cout << "Error in separate updates: " << e.what() << endl;
        return false;
    }
}

bool updataSql(ConnectionManager& dbManager, string code, std::map<std::string, std::string> fields) {

    //string baseRequest;
    //string startOfRequest = "UPDATE _Reference541 SET (";
    //string requestCondition = ") = (";
    //string requestEnd = ") WHERE _code = $1";

    //string allFields{ "" }, allValue{""};

    //for (const auto& [field, value] : fields) {
    //    if (value == "Yes" || value == "No") {
    //        allFields   =   (allFields  == "")  ? field : allFields + ", " + field;
    //        allValue    =   (allValue   == "")  ? "NOT " + field : allValue + ", NOT " + field;
    //    }
    //    else {
    //        allFields   =   (allFields  == "")  ? field : allFields + ", " + field;
    //        allValue    =   (allValue == "")    ? "'" + value + "'" : allValue + ", '" + value + "'";
    //    }
    //}

    //baseRequest = startOfRequest + allFields + requestCondition + allValue + requestEnd;

    //string connection_string = "dbname=ERP user=postgres password=1978 host=127.0.0.1 port=5432";

    //try {
    //    pqxx::connection C(connection_string);
    //    //cout << "Успешное подключение к базе данных: " << C.dbname() << endl;

    //    pqxx::work W(C);

    //    // Выводим запрос для отладки
    //    cout << "SQL запрос: " << baseRequest << endl;
    //    cout << "Параметр code: " << code << endl;

    //    pqxx::result R = W.exec_params(baseRequest, code);
    //    W.commit();
    //}
    //catch (const exception& e)
    //    {
    //        //string error_msg = utf8_to_cp1251(e.what());
    //        cout << "Connection or request error: " << e.what() << endl;
    //        return false;  // Возвращаем пустой вектор при ошибке
    //    }
    //return true;

    if (fields.empty()) {
        cout << "No fields to update." << endl;
        return false;
    }

    //string connection_string = "dbname=ERP user=postgres password=1978 host=127.0.0.1 port=5432";

    try {
        //pqxx::connection C(connection_string);
        //pqxx::work W(C);

        //auto conn = ConnectionManager::getInstance().getConnection();
        auto conn = dbManager.getConnection();
        if (!conn || !conn->is_open()) {
            cout << "Database connection failed." << endl;
            return false;
        }

        pqxx::work W(*conn);

        // Строим SQL запрос с параметрами
        string sql = "UPDATE _Reference541 SET ";

        // Подготавливаем список SET частей
        vector<string> setClauses;
        vector<string> paramValues;

        for (const auto& [field, value] : fields) {
            if (value == "Yes" || value == "No") {
                // Для Yes/No - это инвертирование булевого поля
                // В PostgreSQL: true/false или 't'/'f'
                bool newValue = (value == "Yes");
                setClauses.push_back(field + " = $" + to_string(setClauses.size() + 2));
                paramValues.push_back(newValue ? "true" : "false");
            }
            else if (value == "true" || value == "false") {
                // Прямые булевы значения
                setClauses.push_back(field + " = $" + to_string(setClauses.size() + 2));
                paramValues.push_back(value);
            }
            else {
                // Строковые значения
                setClauses.push_back(field + " = $" + to_string(setClauses.size() + 2));
                paramValues.push_back(value);
            }
        }

        // Объединяем SET части
        for (size_t i = 0; i < setClauses.size(); ++i) {
            if (i > 0) sql += ", ";
            sql += setClauses[i];
        }

        // Добавляем WHERE условие
        sql += " WHERE _code = $1";

        // Выводим для отладки
        cout << "SQL: " << sql << endl;
        cout << "Code: " << code << endl;
        for (size_t i = 0; i < paramValues.size(); ++i) {
            cout << "Param $" << (i + 2) << ": " << paramValues[i] << endl;
        }

        // Собираем все параметры
        vector<string> allParams;
        allParams.push_back(code);  // $1
        allParams.insert(allParams.end(), paramValues.begin(), paramValues.end());

        // Выполняем запрос с параметрами
        // Создаем statement с параметрами
        pqxx::result R;

        if (allParams.size() == 1) {
            // Только код (хотя такого не должно быть, т.к. fields не пустой)
            R = W.exec_params(sql, allParams[0]);
        }
        else if (allParams.size() == 2) {
            R = W.exec_params(sql, allParams[0], allParams[1]);
        }
        else if (allParams.size() == 3) {
            R = W.exec_params(sql, allParams[0], allParams[1], allParams[2]);
        }
        else if (allParams.size() == 4) {
            R = W.exec_params(sql, allParams[0], allParams[1], allParams[2], allParams[3]);
        }
        else if (allParams.size() == 5) {
            R = W.exec_params(sql, allParams[0], allParams[1], allParams[2], allParams[3], allParams[4]);
        }
        else {
            // Для большего количества параметров используем альтернативный подход
            cout << "Too many parameters for simple exec_params." << endl;

            // Альтернатива: выполнять каждый UPDATE отдельно
            return updateFieldsSeparately(W, code, fields);
        }

        W.commit();
        cout << "Update successful. Rows affected: " << R.affected_rows() << endl;
        return true;
    }
    catch (const exception& e)
    {
        cout << "Connection or request error: " << e.what() << endl;
        return false;
    }
}
