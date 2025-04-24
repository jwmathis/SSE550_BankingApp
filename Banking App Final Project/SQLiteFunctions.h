#pragma once

#include "sqlite3.h"
#include <string>

/**
 * @file sqlitefunctions.h
 * @brief Provides utility functions for working with SQLite databases.
 */

 /**
  * @brief Displays the names of all tables in the specified SQLite database.
  *
  * This function queries the database for all table names and prints them to the console.
  *
  * @param s The name of the SQLite database file to query.
  */
void showAllTables(const char* s);

/**
 * @brief Callback function used for SQLite query execution.
 *
 * This function is called for each row of query results retrieved by SQLite.
 * It prints the column names and corresponding values of the current row to the console.
 *
 * @param NotUsed Reserved for future use; not currently used.
 * @param argc Number of columns in the current row.
 * @param argv Array of strings representing column values in the current row.
 * @param azColName Array of strings representing column names.
 * @return Always returns 0 to indicate successful execution.
 */
static int callback(void* NotUsed, int argc, char** argv, char** azColName);