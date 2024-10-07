#include "Account.h"
#include <string>
using namespace std;


Account::Account(string x, double y) {
	accountNum = x;
	balance = y;


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