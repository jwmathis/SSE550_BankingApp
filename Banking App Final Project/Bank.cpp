#include "Bank.h"
#include <iostream>

void Bank::registerCustomer(const string& id, const string& name) {
	if (customers.find(id) == customers.end()) {
		customers[id] = Customer(id, name);
		cout << "Customer " << name << " registered successfully!" << endl;
	}
	else {
		cout << "Customer ID already exists." << endl;
	}
}

Customer* Bank::login(const string& customerId) {
	auto it = customers.find(customerId);
	if (it != customers.end()) {
		return &it->second;
	}
	return nullptr;
}
