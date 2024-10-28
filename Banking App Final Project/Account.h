#pragma once
class Account {
private:
	string accountNum;
	double balance;

public:
	Account(string x, double y);
	Account();
//Declare Functions
	void withdraw(double Amount);
	void deposit(double Amount);
	string getAccountNum();
	double getBalance();
};

