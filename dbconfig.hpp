#pragma once
#include <string>

struct DatabaseConfig {
    std::string host = "127.0.0.1";
    std::string port = "5432";
    std::string dbname = "ERP";
    std::string user = "postgres";
    std::string password = "1978";
    std::string charset = "utf8";
    std::string timezone = "UTC";
    
    // Метод для получения строки подключения
    std::string getConnectionString() const;
    
    // Метод для загрузки из файла
    bool loadFromFile(const std::string& filename);
    
    // Метод для сохранения в файл
    bool saveToFile(const std::string& filename) const;
    
    // Метод для редактирования через консоль
    void editInteractive();
};