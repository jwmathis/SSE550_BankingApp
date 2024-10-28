#pragma once
#include <string>
#include <vector>
#include "Transfer.h"
#include "Account.h"

using namespace std;

class Transfer {
private:
	Account accountFrom;
	Account accountTo;
	double amount;
	vector<Account> accounts;

public:
	Transfer(Account accountFrom, Account accountTo);
	
	Account getAccountFrom() const;
	Account getAccountTo() const;
	void setAccountFrom(Account accountFrom);
	void setAccountTo(Account accountTo);

	void setAmount(double amount);

};