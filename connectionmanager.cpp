#include "connectionmanager.hpp"
#include <iostream>
#include <windows.h>

using namespace std;

ConnectionManager::ConnectionManager() {
    // Конструктор по умолчанию
}

ConnectionManager::~ConnectionManager() {
    // Деструктор
    if (connection && connection->is_open()) {
        connection->close();
    }
}

bool ConnectionManager::initialize(const string& configFile) {
    // Загружаем конфигурацию
    if (!config.loadFromFile(configFile)) {
        cerr << "Using default configuration." << endl;
        // Можно создать файл с настройками по умолчанию
        config.saveToFile(configFile);
    }

    // Пытаемся подключиться
    return reconnect();
}

shared_ptr<pqxx::connection> ConnectionManager::getConnection() {
    if (!connection || !connection->is_open()) {
        reconnect();
    }
    return connection;
}

bool ConnectionManager::reconnect() {
    try {
        string connStr = config.getConnectionString();
        connection = make_shared<pqxx::connection>(connStr);

        if (connection->is_open()) {
            cout << "Connected to database: " << connection->dbname() << endl;
            return true;
        }
        else {
            cerr << "Connection failed." << endl;
            return false;
        }
    }
    catch (const exception& e) {
        cerr << "Connection error: " << e.what() << endl;
        return false;
    }
}

const DatabaseConfig& ConnectionManager::getConfig() const {
    return config;
}

void ConnectionManager::editConfig() {
    config.editInteractive();
    saveConfig();
    reconnect();
}

bool ConnectionManager::saveConfig(const string& filename) {
    return config.saveToFile(filename);
}

bool ConnectionManager::testConnection() {
    try {
        auto conn = getConnection();
        if (conn && conn->is_open()) {
            pqxx::work txn(*conn);
            pqxx::result r = txn.exec("SELECT 1");
            txn.commit();
            return true;
        }
        return false;
    }
    catch (const exception& e) {
        cerr << "Connection test failed: " << e.what() << endl;
        return false;
    }
}