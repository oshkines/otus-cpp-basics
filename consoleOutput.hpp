#pragma once
#include <pqxx/pqxx>
#include <vector>
#include <string>
#include "common.hpp"  

std::vector<Record> OutputConsole(const pqxx::result& R, int colRecords);