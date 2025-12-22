#pragma once
#include <string>
#include <vector>

struct Record {
    std::string code;
    std::string name;
    std::string isKlient;
    std::string isPostavshik;
    std::string birthdate;
    std::string typeKI;
    std::string kontactInformation;
};

std::vector<Record> readSql(int colRecords, std::string code);