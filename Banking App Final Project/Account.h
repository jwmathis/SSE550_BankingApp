#pragma once
#include <string>

using namespace std;

class Account {
private:
	int id;				// Unique ID in the database
	string accountNum;	// Unique account number
	double balance;		// Current account balance

public:
	Account(int id, const string& accNum, double initialBalance);
	Account();
	// Getters
	int getId() const;
	const string& getAccountNum() const;
	double getBalance() const;

	// Behavior methods that are Database independent
	void withdraw(double Amount);
	void deposit(double Amount);
};

