#pragma once
#include <string>
#include <map>
#include "connectionmanager.hpp"

//class ConnectionManager;

bool updataSql(ConnectionManager& dbManager, std::string code, std::map<std::string, std::string> fields);