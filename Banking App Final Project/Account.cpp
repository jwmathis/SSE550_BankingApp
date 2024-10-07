#include Account.h
using namespace std;
#include string


Account::Account(string x, double y) {
	accountNum = x;
	initialBal = y;


	void withdraw(double Amount) {
		initialBal = initialBal - Amount;
	}

	void deposit(double Amount) {
		initialBal = initialBal + Amount;
	}

	string getAccountNum() {
		return accountNum;
 	}

	double getBalance() {
		return initialBal;
	}
}