#pragma once
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Customer {
private:
	string name; // Customer name for accounts
	string username; // Username for login
	string pin; //replaced customerID with a PIN for authentication
	vector<Account> accounts;

public:
	Customer();
	Customer(string name, string username, string pin);

	void openAccount(int accountNumber, double initialBalance);
	Account* getAccount(string accountNumber);

	string getName() const;
	string getUsername() const;
	string getPin() const;

	const vector<Account>& getAccounts() const { return accounts; }
};
