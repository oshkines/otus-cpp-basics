#pragma once
#include "dbconfig.hpp"
#include <pqxx/pqxx>
#include <memory>
#include <string>

class ConnectionManager {
private:
    DatabaseConfig config;
    std::shared_ptr<pqxx::connection> connection;

public:
    // Конструктор по умолчанию
    ConnectionManager();

    // Деструктор
    ~ConnectionManager();

    // Удаляем копирование
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    // Инициализация
    bool initialize(const std::string& configFile = "database.conf");

    // Получение подключения
    std::shared_ptr<pqxx::connection> getConnection();

    // Переподключение
    bool reconnect();

    // Получение конфигурации
    const DatabaseConfig& getConfig() const;

    // Редактирование конфигурации
    void editConfig();

    // Сохранение конфигурации
    bool saveConfig(const std::string& filename = "database.conf");

    // Проверка подключения
    bool testConnection();
};