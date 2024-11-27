#pragma once
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Customer {
private:
	string pin; //replaced customerID with a PIN for authentication
	string name;
	vector<Account> accounts;

public:
	Customer();
	Customer(string pin, string name);

	void openAccount(string accountNumber, double initialBalance);
	Account* getAccount(string accountNumber);
	string getPin() const;
	string getName() const;

	const vector<Account>& getAccounts() const { return accounts; }
};
