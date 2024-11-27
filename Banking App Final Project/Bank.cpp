#include "Bank.h"
#include <iostream>

void Bank::registerCustomer(const string& pin, const string& name) {
	if (customers.find(pin) == customers.end()) {
		customers[pin] = Customer(pin, name);
		cout << "Customer " << name << " registered successfully!" << endl;
	}
	else {
		cout << "Customer ID already exists." << endl;
	}
}

Customer* Bank::login(const string& pin) {
	auto it = customers.find(pin);
	if (it != customers.end()) {
		return &it->second;
	}
	return nullptr;
}
