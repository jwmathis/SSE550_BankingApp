#pragma once
#include <string>
#include <unordered_map>
#include "Customer.h"
using namespace std;

class Bank
{
private:
	unordered_map<string, Customer> customers;
	int accountNumberCounter = 100000; // Start account numbers

public:
	void registerCustomer(const string& name, const string& username, const string& pin);
	Customer* login(const string& username, const string& pin);

	int generateAccountNumber(); // Method to generate account numbers for customers
};

