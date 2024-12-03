#pragma once
#include "Bank.h"
#include "Customer.h"
#include "Transfer.h"

// Enums for menu options
enum MainMenuOption {
	REGISTER = 1,
	LOGIN,
	EXIT
};

enum CustomerMenuOption {
	NEW_ACCOUNT = 1,
	BALANCE_INQUIRY,
	DEPOSIT_AMOUNT,
	WITHDRAW_AMOUNT,
	TRANSFER_AMOUNT,
	CLOSE_ACCOUNT,
	LOGOUT
};

// Constants
const string WELCOME_MESSAGE = "			Welcome to Mercer Money Banking!";
const string INVALID_OPTION = "Invalid option. Please try again.";

// Functions declarations
void registerCustomer(Bank& bank);
bool loginCustomer(Bank& bank);
void customerMenu(Customer* customer, Bank& bank);
