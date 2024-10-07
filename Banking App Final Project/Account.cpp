#include "Account.h"
#include <string>
using namespace std;


Account::Account(string x, double y) {
	accountNum = x;
	balance = y;


	void withdraw(double Amount) {
		balance = balance - Amount;
	}

	void deposit(double Amount) {
		balance = balance + Amount;
	}

	string getAccountNum() {
		return accountNum;
 	}

	double getBalance() {
		return balance;
	}
}