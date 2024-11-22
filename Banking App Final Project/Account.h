#pragma once
#include <string>

using namespace std;

class Account {
private:
	string accountNum;
	double balance;

public:
	Account(string accNum, double initialBalance);
	Account();
	// Declare Functions
	void withdraw(double Amount);
	void deposit(double Amount);
	string getAccountNum() const;
	double getBalance() const;
	void setBalance(double newBalance);
};

