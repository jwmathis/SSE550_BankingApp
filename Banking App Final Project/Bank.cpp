#include "Bank.h"
#include <ctime>
#include <cstdlib>
#include <iostream>

Bank::Bank(const string& dbName) {
	if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
		cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
		db = nullptr;
	}
	else {
		//cout << "Database opened successfully." << endl;

		if (!createTables()) {
			cerr << "Failed to create tables in database." << endl;
		}
	}
}

Bank::~Bank() {
	if (db) {
		sqlite3_close(db);
	}
}
bool Bank::createTables() {
	string sqlUsersTable = "CREATE TABLE IF NOT EXISTS users ( "
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name TEXT NOT NULL, "
		"username TEXT NOT NULL UNIQUE, "
		"pin TEXT NOT NULL);";

	string sqlAccountsTable = "CREATE TABLE IF NOT EXISTS accounts ( "
		"account_id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"user_id INTEGER NOT NULL, "
		"account_number TEXT NOT NULL, "
		"balance DOUBLE NOT NULL DEFAULT 0.0, "
		"FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE);";

	char* messageError = nullptr;

	// Create Users table
	if (sqlite3_exec(db, sqlUsersTable.c_str(), nullptr, 0, &messageError) != SQLITE_OK) {
		cerr << "Error creating users table" << messageError << endl;
		sqlite3_free(messageError);
		return false;
	}
	//cout << "Users table created successfully" << endl;
		
	// Create accounts table
	if (sqlite3_exec(db, sqlAccountsTable.c_str(), nullptr, 0, &messageError) != SQLITE_OK) {
		cerr << "Error creating accounts table" << messageError << endl;
		sqlite3_free(messageError);
		return false;
	}
	//cout << "Accounts table created successfully" << endl;

	return true;
}
bool Bank::executeSQL(const string& query, sqlite3_stmt** stmt) {
	if (sqlite3_prepare_v2(db, query.c_str(), -1, stmt, nullptr) != SQLITE_OK) {
		cerr << "SQL error: " << sqlite3_errmsg(db) << endl;
		return false;
	}
	return true;
}

int Bank::getUserId(const string& username) {
	string userIdSQL = "SELECT id FROM users WHERE username = '" + username + "';";
	sqlite3_stmt* stmt;

	if (!executeSQL(userIdSQL, &stmt)) return -1;

	int userId = -1;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		userId = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return userId;
}

bool Bank::registerCustomer(const string& name, const string& username, const string& pin) {
	if (getUserId(username) != -1) {
		cerr << "Error: Failed to register user. Username already exists." << endl;
		return false;
	}

	string sql("INSERT INTO users (name, username, pin) VALUES ( '" + name + "', '" + username + "', '" + pin + "');");
	char* messageError;

	if (sqlite3_exec(db, sql.c_str(), nullptr, 0, &messageError) != SQLITE_OK) {
		cerr << "Error: Could not insert user data into database" << endl;
		sqlite3_free(messageError);
		return false;
	}
	return true;
}

Customer* Bank::login(const string& username, const string& pin) {
	string sql = "SELECT id, name FROM users WHERE username = '" + username + "' AND pin = '" + pin + "';";
	sqlite3_stmt* stmt;

	if (!executeSQL(sql, &stmt)) return nullptr;

	Customer* customer = nullptr;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		customer = new Customer(id, name, username, pin);
	}
	sqlite3_finalize(stmt);
	return customer;
}

bool Bank::addAccountForCustomer(int userId, const string& accountNumber, double initialBalance) {
	string sql = "INSERT INTO accounts (user_id, account_number, balance) VALUES (" +
		to_string(userId) + ", '" + accountNumber + "', " + to_string(initialBalance) + ");";
	char* errMessage = nullptr;

	if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMessage) != SQLITE_OK) {
		cerr << "Error adding account: " << errMessage << endl;
		sqlite3_free(errMessage);
		return false;
	}
	return true;
}

vector<Account> Bank::getAccountsForCustomer(int userId) {
	vector<Account> accounts;
	string sql = "SELECT account_id, account_number, balance FROM accounts WHERE user_id = " + to_string(userId) + ";";
	sqlite3_stmt* stmt;

	if (!executeSQL(sql, &stmt)) return accounts;

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int accountId = sqlite3_column_int(stmt, 0);
		string accountNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		double balance = sqlite3_column_double(stmt, 2);
		accounts.emplace_back(accountId, accountNumber, balance);
	}
	sqlite3_finalize(stmt);
	return accounts;
}

Account* Bank::getAccountByNumber(const string& accountNumber) {
	string sql = "SELECT account_id, account_number, balance FROM accounts WHERE account_number = '" + accountNumber + "'; ";
	sqlite3_stmt* stmt;

	if (!executeSQL(sql, &stmt)) return nullptr;

	Account* account = nullptr;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		int accountId = sqlite3_column_int(stmt, 0);
		string accNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		double balance = sqlite3_column_double(stmt, 2);
		account = new Account(accountId, accNumber, balance);
	}
	sqlite3_finalize(stmt);
	return account;
}

bool Bank::updateAccountBalance(int accountId, double newBalance) {
	string sql = "UPDATE accounts SET balance = ? WHERE account_id = ?;";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
		return false;
	}

	sqlite3_bind_double(stmt, 1, newBalance); // Bind newBalance to the first placeholder
	sqlite3_bind_int(stmt, 2, accountId); // Bind account_id to the second placeholder

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		cerr << "Error updating balance: " << sqlite3_errmsg(db) << endl;
		sqlite3_finalize(stmt);
		return false;
	}

	sqlite3_finalize(stmt);
	return true;
}

int Bank::generateAccountNumber() {
	int accountNumber;
	srand(time(0));

	do {
		accountNumber = (rand() % 900000) + 100000;

	} while (accountNumberExists(to_string(accountNumber)));

	return accountNumber;
}

bool Bank::accountNumberExists(const string& accountNumber) {
	string sql = "SELECT COUNT(*) FROM accounts WHERE account_number = '" + accountNumber + "';";
	sqlite3_stmt* stmt;

	if (!executeSQL(sql, &stmt)) return -1;

	int count = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW) {
		count = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return count > 0;
}

bool Bank::accountRemoveAccount(const string& accountNumber) {
	string sql = "DELETE FROM accounts WHERE account_number = '" + accountNumber + "';";
	char* errMessage = nullptr;

	if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMessage) != SQLITE_OK) {
		cerr << "Error deleting account: " << errMessage << endl;
		sqlite3_free(errMessage);
		return false;
	}
	return true;
}