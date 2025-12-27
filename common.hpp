#pragma once
#include <string>

struct Record {
    std::string code;
    std::string name;
    bool isKlient;
    bool isPostavshik;
    std::string birthdate;
    std::string typeKI;
    std::string kontactInformation;
};