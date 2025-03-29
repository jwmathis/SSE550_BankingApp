#pragma once
#include <string>
#include <iostream>

using namespace std;

template<typename T>
class Account {
protected:
	int id;
	string accountNum;
	T balance;

public:
	Account(int id, const string& accNum, double initialBalance)
		: id(id), accountNum(accNum), balance(initialBalance) {
	}
	Account() : id(0), accountNum(""), balance(0.0) {}

	int getId() const {
		return id;
	}

	const string& getAccountNum() const {
		return accountNum;
	}

	T getBalance() const {
		return balance;
	}

	void withdraw(T amount) {
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

	void deposit(T amount) {
		if (amount > 0) {
			balance = balance + amount;
			cout << "Deposited " << amount << ". New Balance: " << balance << endl;
		}
		else {
			cerr << "Error: Deposit amount must be positive." << endl;
		}
	}

	virtual void applyInterest() {
		cout << "No interest" << endl;
	}

	virtual ~Account() = default;
};