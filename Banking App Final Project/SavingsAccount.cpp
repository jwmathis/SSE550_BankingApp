#include "SavingsAccount.h"
#include <iostream>

using namespace std;

SavingsAccount::SavingsAccount(int id, const string& accNum, double initialBalance, double interestRate)
	: Account(id, accNum, initialBalance), interestRate(interestRate) {}

void SavingsAccount::applyInterest() {
	if (interestRate > 0) {
		double interest = balance * interestRate;
		balance += interest;
	}
}