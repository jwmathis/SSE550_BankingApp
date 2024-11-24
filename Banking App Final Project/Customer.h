#pragma once
#include <string>
#include <vector>
#include "Account.h"

using namespace std;

class Customer {
private:
	string customerId;
	string name;
	vector<Account> accounts;

public:
	Customer();
	Customer(string id, string name);

	void openAccount(string accountNumber, double initialBalance);
	Account* getAccount(string accountNumber);
	string getCustomerId() const;
	string getName() const;

	const vector<Account>& getAccounts() const { return accounts; }
};
