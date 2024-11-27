#include "Customer.h"
#include <iostream>

Customer::Customer() : name("") {}
Customer::Customer(string pin, string name) 
	: pin(pin), name(name) {}

void Customer::openAccount(string accountNumber, double initialBalance) {
	Account newAccount(accountNumber, initialBalance); // create new Account object
	accounts.push_back(newAccount);
	cout << "Account " << accountNumber << " opened for " << name << endl;
}

Account* Customer::getAccount(string accountNumber) {
	for (auto& account : accounts) {
		if (account.getAccountNum() == accountNumber) {
			return &account;
		}
	}
	return nullptr;
}

string Customer::getPin() const {
	return pin;
}

string Customer::getName() const {
	return name;
}