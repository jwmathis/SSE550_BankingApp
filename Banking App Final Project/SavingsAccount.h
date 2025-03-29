#pragma once

#include "Account.h"
#include <iostream>
#include <string>

template <typename T>
class SavingsAccount : public Account<T> {
private:
	T interestRate;

public:
	SavingsAccount(int id, const string& accNum, double initialBalance, double interestRate) 
		: Account<T>(id, accNum, initialBalance), interestRate(interestRate) {}

	void applyInterest() override {
		if (this->interestRate > 0) {
			T interest = this->balance * this->interestRate;
			this->balance += interest;
		}
		else {
			cerr << "Error: Interest rate must be greater than 0." << endl;
		}
	}
};
