#include "Account.h"
#include <string>
#include <iostream>

using namespace std;

Account::Account() : accountNum(""), balance(0.0) {}

Account::Account(string accNum, double initialBalance) {
	accountNum = accNum;
	balance = initialBalance;
}

void Account::withdraw(double Amount) {
	balance = balance - Amount;
	cout << "Withdrew " << Amount << ". New Balance: " << balance << endl;
}

void Account::deposit(double Amount) {
	balance = balance + Amount;
	cout << "Deposited " << Amount << ". New Balance: " << balance << endl;
}

string Account::getAccountNum() const {
	return accountNum;
}

double Account::getBalance() const {
	return balance;
}

void Account::setBalance(double newBalance) {
	balance = newBalance;
}
