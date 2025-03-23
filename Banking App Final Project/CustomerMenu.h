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
	REG_ACCOUNT = 1,
	SAVINGS_ACC,
	BALANCE_INQUIRY,
	DEPOSIT_AMOUNT,
	WITHDRAW_AMOUNT,
	TRANSFER_AMOUNT,
	CLOSE_ACCOUNT,
	LOGOUT
};

enum AccountTypeOption {
	CHECKING_ACCOUNT = 1,
	SAVINGS_ACCOUNT
};

// Constants
const string WELCOME_MESSAGE = "					Welcome to Mercer Money Banking!";
const string INVALID_OPTION = "Invalid option. Please try again.";

// Functions declarations
void displayCustomerAccountsMenu(Bank& bank, Customer* customer);
string selectCustomerAccount(Bank& bank, Customer* customer, int userSelection);
string promptForAccountSelection(Bank& bank, Customer* customer, const string& promptMessage);
void registerCustomer(Bank& bank);
bool loginCustomer(Bank& bank);
void customerMenu(Customer* customer, Bank& bank);
void newCustomer(Customer* customer, Bank& bank);

