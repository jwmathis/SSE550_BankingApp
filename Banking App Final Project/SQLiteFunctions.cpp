#include "SQLiteFunctions.h"
#include <iostream>
#include <iomanip>

using namespace std;

static bool printedHeaders = false;

// SQLite Database management functions

void showAllTables(const char* s) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);

	if (exit != SQLITE_OK) {
		cerr << "Error opening databases: " << sqlite3_errmsg(DB) << endl;
	}
	printedHeaders = false; // Flag for formatting table data
	string sqlUsers = "SELECT * FROM users;";
	cout << "Users Table: " << endl;
	sqlite3_exec(DB, sqlUsers.c_str(), callback, (void*)"Users", NULL);

	printedHeaders = false; // Flag for formatting table data
	string sqlAccounts = "SELECT * FROM accounts;";
	cout << "\nAccounts Table:" << endl;
	sqlite3_exec(DB, sqlAccounts.c_str(), callback, (void*)"Accounts", NULL);

	sqlite3_close(DB);

}

static int callback(void* data, int argc, char** argv, char** azColName) {
	const char* table = (const char*)data;

	if (argv != NULL && !printedHeaders) {
		if (table != NULL) {
			cout << "\n" << table << " Data: " << endl;
			cout << "------------------------------------------------------------------------------------" << endl;
		}
		for (int i = 0; i < argc; i++) {
			cout << left << setw(20) << azColName[i] << "|";
		}
		cout << "\n------------------------------------------------------------------------------------" << endl;
		printedHeaders = true;
	}

	for (int i = 0; i < argc; i++) {
		cout << left << setw(20) << (argv[i] ? argv[i] : "NULL") << "|";
	}
	cout << endl;
	return 0;
}