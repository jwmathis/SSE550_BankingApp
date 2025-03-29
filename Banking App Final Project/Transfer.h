#pragma once
#include <string>
#include <vector>
#include "Transfer.h"
#include "Account.h"
#include "Customer.h"

using namespace std;

template <typename T>
class Transfer {
private:
	Account<T>* accountFrom;
	Account<T>* accountTo;
	double amount;
	vector<Account<T>*> accounts;

public:
	Transfer()
		: accountFrom(nullptr), accountTo(nullptr), amount(0) {}

	Transfer(Account<T>* accountFrom, Account<T>* accountTo)
	{
		this->accountFrom = accountFrom;
		this->accountTo = accountTo;
		this->amount = 0;
	}

	// Returns sender account
	Account<T>* getAccountFrom() const
	{
		return this->accountFrom;
	}

	// Returns receiver account
	Account<T>* getAccountTo() const
	{
		return this->accountTo;
	}

	// Set sender account.
	void setAccountFrom(Account<T>* accountFrom)
	{
		this->accountFrom = accountFrom;
	}

	// Sets receiver account.
	void setAccountTo(Account<T>* accountTo)
	{
		this->accountTo = accountTo;
	}

	// Sets amount to be transferred.
	void setAmount(T amount)
	{
		if (amount <= 0 || amount > accountFrom->getBalance())
		{
			cout << "Error: Could not set the amount of transfer." << endl;
			return;
		}

		this->amount = amount;
	}

};