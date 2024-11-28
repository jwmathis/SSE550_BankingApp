#include "Customer.h"
#include <string>
#include <iostream>

Customer::Customer(int id, const string& name, const string& username, const string& pin) 
	: id(id), name(name), username(username), pin(pin) {}

int Customer::getId() const {
	return id;
}

const string& Customer::getName() const {
	return name;
}

const string& Customer::getUsername() const {
	return username;
}

bool Customer::verifyPin(const string& inputPin) const {
	return inputPin == pin;
}
