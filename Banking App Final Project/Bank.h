#pragma once
#include <string>
#include <unordered_map>
#include "Customer.h"
using namespace std;

class Bank
{
private:
	unordered_map<string, Customer> customers;

public:
	void registerCustomer(const string& name, const string& username, const string& pin);
	Customer* login(const string& username, const string& pin);
};

