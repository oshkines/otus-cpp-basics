#include "common.hpp"
#include "readSQL.hpp"
#include "updataSQL.hpp"
#include "deleteSQL.hpp"
#include "consoleOutput.hpp"
#include "connectionmanager.hpp"
#include <iostream>
#include <map>
#include <limits>
#include <string>  // Добавляем
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
#endif

    // Создаем менеджер подключений
    ConnectionManager dbManager;

    // Инициализация менеджера подключений
    std::cout << "Initializing database connection..." << std::endl;
    if (!dbManager.initialize("database.conf")) {
        std::cout << "Failed to initialize database connection." << std::endl;
        std::cout << "Please check your configuration file or edit settings." << std::endl;

        // Предлагаем редактировать настройки
        char choice;
        std::cout << "Edit database settings now? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            dbManager.editConfig();

            // Пытаемся снова
            if (!dbManager.testConnection()) {
                std::cout << "Still cannot connect. Exiting." << std::endl;
                return 1;
            }
        }
        else {
            return 1;
        }
    }

    std::cout << "Database connection established successfully!" << std::endl;

    int colRecords;

    std::cout << "Enter the number of records requested from the customer table:" << std::endl;
    while (true)
    {
        std::cin >> colRecords;
        if (std::cin.fail()) {
            std::cout << "Error! Please enter a valid number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }

    std::string code = "";
    pqxx::result R = readSql(dbManager, colRecords, code);  // Передаем dbManager
    std::vector<Record> records = OutputConsole(R, colRecords);

    std::cout << "\nRecords returned from the function: " << records.size() << std::endl;

    if (records.size() == 0)
    {
        return 0;
    }

    while (true)
    {
        bool thereAreChanges = false;
        std::cout << "\nSelect the following operation with the SQL database:" << std::endl;
        std::cout << "1. Change the record." << std::endl;
        std::cout << "2. Delete the record." << std::endl;
        std::cout << "3. Database Settings." << std::endl;
        std::cout << "4. Exit." << std::endl;

        int entryNumber;
        while (true)
        {
            std::cin >> entryNumber;
            if (std::cin.fail())
            {
                std::cout << "Error! Please enter a number between 1 and 4: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else
            {
                if (entryNumber >= 1 && entryNumber <= 4)
                {
                    break;
                }
            }
        }

        if (entryNumber == 1)
        {
            std::map<std::string, std::string> fields;
            std::cout << "Enter the client record code:" << std::endl;
            std::cin >> code;

            R = readSql(dbManager, colRecords, code);  // Передаем dbManager
            records = OutputConsole(R, colRecords);

            int option;
            for (const auto& record : records)
            {
                std::cin.ignore();
                std::cout << "Do you want to change the client name? Yes = 1 No = any character:" << std::endl;
                std::cin >> option;
                if (option == 1)
                {
                    std::cout << "Enter new name:" << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, fields["_description"]);
                    thereAreChanges = true;
                }

                std::cout << "You want to invert the client field (boolean)? Yes = 1 No = any character:" << std::endl;
                std::cin >> option;
                if (option == 1)
                {
                    fields["_fld55051"] = record.isKlient ? "false" : "true";
                    thereAreChanges = true;
                }

                std::cout << "You want to invert the postavshik field (boolean)? Yes = 1 No = any character:" << std::endl;
                std::cin >> option;
                if (option == 1)
                {
                    fields["_fld55053"] = record.isPostavshik ? "false" : "true";
                    thereAreChanges = true;
                }

                std::cout << "Do you want to change the client birthdate (`1978-02-08`)? Yes = 1 No = any character:" << std::endl;
                std::cin >> option;
                if (option == 1)
                {
                    std::cout << "Enter new date of birth:" << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, fields["_fld55064"]);
                    thereAreChanges = true;
                }

                if (thereAreChanges)
                {
                    bool result = updataSql(dbManager, code, fields);  // Передаем dbManager

                    if (result) {
                        R = readSql(dbManager, colRecords, code);  // Передаем dbManager
                        records = OutputConsole(R, colRecords);
                    }
                    break;
                }
            }
        }
        else if (entryNumber == 2)
        {
            std::cout << "Enter the client delete code:" << std::endl;
            std::cin >> code;

            bool result = deleteSql(dbManager, code);  // Передаем dbManager

            if (result)
            {
                R = readSql(dbManager, colRecords, "");  // Передаем dbManager
                records = OutputConsole(R, colRecords);
            }
        }
        else if (entryNumber == 3)
        {
            // Меню настроек базы данных
            std::cout << "\n=== Database Settings ===" << std::endl;
            std::cout << "1. Edit connection settings" << std::endl;
            std::cout << "2. Test connection" << std::endl;
            std::cout << "3. Show current settings" << std::endl;
            std::cout << "4. Back to main menu" << std::endl;

            int settingsChoice;
            std::cin >> settingsChoice;

            switch (settingsChoice) {
            case 1:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                dbManager.editConfig();
                break;
            case 2:
                if (dbManager.testConnection()) {
                    std::cout << "Connection test: SUCCESS" << std::endl;
                }
                else {
                    std::cout << "Connection test: FAILED" << std::endl;
                }
                break;
            case 3: {
                const auto& config = dbManager.getConfig();
                std::cout << "\nCurrent settings:" << std::endl;
                std::cout << "Host: " << config.host << std::endl;
                std::cout << "Port: " << config.port << std::endl;
                std::cout << "Database: " << config.dbname << std::endl;
                std::cout << "User: " << config.user << std::endl;
                std::cout << "Password: " << std::string(config.password.length(), '*') << std::endl;
                std::cout << "Charset: " << config.charset << std::endl;
                std::cout << "Timezone: " << config.timezone << std::endl;
                break;
            }
            case 4:
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
            }
        }
        else
        {
            return 0;
        }
    }

    return 0;
}