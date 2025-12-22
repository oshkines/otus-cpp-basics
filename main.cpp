#include "readSQL.hpp"
#include "updataSQL.hpp"
#include "deleteSQL.hpp"
#include <iostream>
#include <map>
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Устанавливаем кодировку консоли
    #ifdef _WIN32
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    #endif

    int colRecords;
    std::cout << "Enter the number of records requested from the customer table:" << std::endl;
    std::cin >> colRecords;

    std::string code = "";

    std::vector<Record> records = readSql(colRecords, code);

    // Можно дальше работать с полученными записями
    std::cout << "\nRecords returned from the function: " << records.size() << std::endl;

    if (records.size() == 0)
    {
        return 0;
    }
    bool thereAreChanges = false;
        std::cout << "Select the following operation with the SQL database:" << std::endl;
        std::cout << "1. change the record." << std::endl;
        std::cout << "2. delete the record." << std::endl;
        std::cout << "3. exit." << std::endl;

        int entryNumber;
        std::cin >> entryNumber;

        if (entryNumber == 1)
        {
            std::map<std::string, std::string> fields;
            std::cout << "Enter the client record code:" << std::endl;
            std::cin >> code;
            records = readSql(colRecords, code);
            int option;
            for (const auto& record : records) {
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
                    if (record.isKlient == "Yes") {
                        fields["_fld55051"] = "No";
                    }
                    else {
                        fields["_fld55051"] = "Yes";
                    }
                    thereAreChanges = true;
                }

                std::cout << "You want to invert the postavshik field (boolean)? Yes = 1 No = any character:" << std::endl;
                std::cin >> option;
                if (option == 1)
                {
                    if (record.isPostavshik == "Yes") {
                        fields["_fld55053"] = "No";
                    }
                    else {
                        fields["_fld55053"] = "Yes";
                    }
                    thereAreChanges = true;
                }

                //std::cout << "Do you want to change the client kontact information? Yes = 1 No = any character:" << std::endl;
                //std::cin >> option;
                //if (option == 1)
                //{
                //    std::cout << "Enter new kontact information:" << std::endl;
                //    std::cin.ignore();
                //    std::getline(std::cin, fields["kontactinformation"]);
                //    thereAreChanges = true;
                //}

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
                    bool result = updataSql(code, fields);

                    if (result) {
                        records = readSql(colRecords, code);
                    }
                    break;
                }
            }
        }
        else if (entryNumber == 2)
        {
            std::cout << "Enter the client delete code:" << std::endl;
            std::cin >> code;

            bool result = deleteSql(code);

            if (result) 
            {
                std::vector<Record> records = readSql(colRecords, "");
            }
        }
        else
        {
            return 0;
        }
    return 0;
}