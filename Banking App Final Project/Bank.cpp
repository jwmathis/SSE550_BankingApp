#include "Bank.h"
#include <iostream>

void Bank::registerCustomer(const string& name, const string& username, const string& pin) {
	if (customers.find(username) == customers.end()) {
		customers[username] = Customer(name, username, pin);
		cout << "Customer " << name << " registered successfully! Your username is: " << username << endl;
	}
	else {
		cout << "Error: Username is already in use. Choose a different username." << endl;
	}
}

Customer* Bank::login(const string& username, const string& pin) {
	auto it = customers.find(username);
	if (it != customers.end() && it->second.getPin() == pin) {
		return &it->second; // Login successful
	}
	return nullptr; // Login failed
}
