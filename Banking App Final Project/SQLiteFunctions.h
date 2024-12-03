#pragma once
#include "sqlite3.h"
#include <string>

void showAllTables(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);

