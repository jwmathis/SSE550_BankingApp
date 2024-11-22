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
	void registerCustomer(const string& id, const string& name);
	Customer* login(const string& customerId);
};

