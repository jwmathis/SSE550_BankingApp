#pragma once

#include "Account.h"

class SavingsAccount : public Account {
private:
	double interestRate;

public:
	SavingsAccount(int id, const string& accNum, double initialBalance, double interestRate);

	void applyInterest() override;

	~SavingsAccount() override = default;
};

