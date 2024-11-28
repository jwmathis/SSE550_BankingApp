#include "Customer.h"
#include <string>
#include <iostream>

Customer::Customer() : name(""), username(""), pin("") {}
Customer::Customer(string name, string username, string pin) 
	: name(name), username(username), pin(pin) {}

void Customer::openAccount(int accountNumber, double initialBalance) {
	Account newAccount(to_string(accountNumber), initialBalance); // create new Account object
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

string Customer::getName() const {
	return name;
}

string Customer::getUsername() const {
	return username;
}

string Customer::getPin() const {
	return pin;
}
