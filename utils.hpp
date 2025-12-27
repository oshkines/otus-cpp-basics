#pragma once
#include <string>
#include <vector>
#include <pqxx/pqxx>

std::string utf8_to_cp1251(const std::string& utf8);
std::string get_cell_value(const pqxx::field& field);
std::vector<int> calculate_column_widths(const pqxx::result& R);
bool string_to_bool(const std::string& str);