#include <iostream>
#include "Transfer.h"
#include "Account.h"
#include "Customer.h"

using namespace std;

Transfer::Transfer(Account accountFrom, Account accountTo)
{
	this->accountFrom = accountFrom;
	this->accountTo = accountTo;
}

// Returns sender account
Account Transfer::getAccountFrom() const
{
	return this->accountFrom;
}

// Returns receiver account
Account Transfer::getAccountTo() const
{
	return this->accountTo;
}

// Set sender account.
void Transfer::setAccountFrom(Account accountFrom)
{
	this->accountFrom = accountFrom;
}

// Sets receiver account.
void Transfer::setAccountTo(Account accountTo)
{
	this->accountTo = accountTo;
}

// Sets amount to be transferred.
void Transfer::setAmount(double amount)
{
	if (amount <= 0 || amount >= accountFrom.getBalance())
	{
		cout << "Error: Could not set the amount of transfer." << endl;
		return;
	}

	this->amount = amount;
}
