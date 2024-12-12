#pragma once
#include <string>

using namespace std;

class Account {
protected:
	int id;				// Unique ID in the database
	string accountNum;	// Unique account number
	double balance;		// Current account balance

public:
	Account(int id, const string& accNum, double initialBalance);
	Account();

	// Virtual Destructor
	virtual ~Account() = default;

	// Getters
	int getId() const;
	const string& getAccountNum() const;
	double getBalance() const;

	// Account operations that are Database independent
	void withdraw(double Amount);
	void deposit(double Amount);

	// Virtual method for interest calculation
	virtual void applyInterest();
};

