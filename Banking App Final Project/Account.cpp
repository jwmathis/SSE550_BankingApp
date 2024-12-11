#include "Account.h"
#include <string>
#include <iostream>

using namespace std;

Account::Account(int id, const string& accNum, double initialBalance)
	: id(id), accountNum(accNum), balance(initialBalance) {}
Account::Account() : id(0), accountNum(""), balance(0.0) {}

int Account::getId() const {
	return id;
}

const string& Account::getAccountNum() const {
	return accountNum;
}

double Account::getBalance() const {
	return balance;
}

void Account::withdraw(double amount) {
	if (amount > 0 && amount <= balance) {
		balance = balance - amount;
		cout << "Withdrew " << amount << ". New Balance: " << balance << endl;
	}
	else if (amount > balance) {
		cerr << "Error: Insufficient funds." << endl;
	}
	else {
		cerr << "Error: Withdrawal amount must be positive." << endl;
	}
}

void Account::deposit(double amount) {
	if (amount > 0) {
		balance = balance + amount;
		cout << "Deposited " << amount << ". New Balance: " << balance << endl;
	}
	else {
		cerr << "Error: Deposit amount must be positive." << endl;
	}
}