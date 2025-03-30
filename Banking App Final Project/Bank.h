#pragma once
#include <string>
#include <iostream>
#include "sqlite3.h"
#include "Customer.h"
#include "SavingsAccount.h"
using namespace std;


class Bank
{
private:
	sqlite3* db;

	bool createTables();
	bool executeSQL(const string& query, sqlite3_stmt** stmt = nullptr);
	int getUserId(const string& username);
	int accountNumberCounter = 100000;
public:
	struct acc {
		int accountId;
		int userId;
		string accountNumber;
		double balance;
		string accountType;
	};

	Bank(const string& dbName); // Constructor to open database
	~Bank(); // Destructor to close database

	bool registerCustomer(const string& name, const string& username, const string& pin);
	Customer* login(const string& username, const string& pin);
	bool addAccountForCustomer(int userId, const string& accountNumber, double initialBalance, const string& accountType);
	template <typename T>
	vector<Account<T>*> getAccountsForCustomer(int userId);
	template <typename T>
	Account<T>* getAccountByNumber(const string& accountNumber);

	bool updateAccountBalance(int accountId, double newBalance);
	bool accountRemoveAccount(const string& accountNumber);
	int generateAccountNumber(const int accountType);
	bool accountNumberExists(const string& accountNumber);
	vector <acc> fetchAccountsFromDatabase();
};

