#include "Bank.h"
#include <iostream>

void Bank::registerCustomer(const string& pin, const string& name) {
	if (customers.find(pin) == customers.end()) {
		customers[pin] = Customer(pin, name);
		cout << "Customer " << name << " registered successfully!" << endl;
	}
	else {
		cout << "Error: PIN is already associated with this name. Choose a different pin." << endl;
	}
}

Customer* Bank::login(const string& pin, const string& name) {
	auto it = customers.find(pin);
	if (it != customers.end() && it->second.getName() == name) {
		return &it->second; // Login successful
	}
	return nullptr; // Login failed
}
