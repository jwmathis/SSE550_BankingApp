#pragma once
class Account {
private:
	string accountNum;
	double initialBal;

public:
	Account(string x, double y);
//Declare Functions
	void withdraw(double Amount);
	void deposit(double Amount);
	string getAccountNum();
	double getBalance();
};

