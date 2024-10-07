#include "Customer.h"
#include <iostream>

Customer::Customer(string id, string name) 
	: customerId(id), name(name) {}

void Customer::openAccount(string accountNumber, double initialBalance) {
	Account newAccount(accountNumber, initialBalance); // create new Account object
	accounts.push_back(newAccount);
	cout << "Account " << accountNumber << " opened for " << name << endl;
}

Account* Customer::getAccount(string accountNumber) {
	for (auto& account : accounts) {
		if (account.getAccountNumber() == accountNumber) {
			return &account;
		}
	}
	return nullptr;
}

string Customer::getCustomerId() const {
	return customerId;
}

string Customer::getName() const {
	return name;
}