#pragma once
#include <string>
#include <pqxx/pqxx>
#include "common.hpp"  
#include "connectionmanager.hpp"

//class ConnectionManager;

pqxx::result readSql(ConnectionManager& dbManager, int colRecords, std::string code = "");