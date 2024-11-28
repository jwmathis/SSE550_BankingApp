// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <algorithm>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"
#include "Transfer.h"

using namespace std;
	/*TODO:
	1. Use Constants/enums for menu options to avoid duplications and to make code easier to update
	2. Make functions for repetitive blocks like register and login and the customer menus
	3. Potentially add validation for inputs to aavoid breaking program
	4. Clean up else if chains by using switch case*/

// Enums for menu options
enum MainMenuOption {
	REGISTER = 1,
	LOGIN,
	EXIT
};

enum CustomerMenuOption {
	NEW_ACCOUNT = 1,
	VIEW_ACCOUNTS,
	DEPOSIT_AMOUNT,
	WITHDRAW_AMOUNT,
	TRANSFER_AMOUNT,
	LOGOUT
};

// Constants
const string WELCOME_MESSAGE = "Welcome to Mercer Money Banking!";
const string INVALID_OPTION = "Invalid option. Please try again.";

// Functions for modularization
void registerCustomer(Bank& bank);
void loginCustomer(Bank& bank);
void customerMenu(Customer* customer, Bank& bank);

int main() {

	////--------------------------------------------------------------------------------------
	////  Constant Declaration:

	//const int New_Account = 1;
	//const int Balance_Inquiry = 2;
	//const int Deposit_Amount = 3;
	//const int Withdraw_Amount = 4;
	//const int Transfer_Amount = 5;
	//const int Close_Account = 6;
	//const int EXIT = 7;

	//string	userName, password, firstName, lastName, status;
	//int userInput = 0;
	//string input;
	////--------------------------------------------------------------------------------------
	//Bank MercerBank;

	//// Welcome Screen 
	//cout << "Welcome to Mercer Money Banking!" << endl;

	//while (true) {
	//	cout << "\nMenu:\n";
	//	cout << "1. Register\n";
	//	cout << "2. Login\n";
	//	cout << "2. Exit\n";
	//	cout << "Enter option: ";
	//	cin >> input;

	//	if (input == "1") {
	//		string id, name;
	//		cout << "Enter Customer ID: ";
	//		cin >> id;
	//		cout << "Enter Name: ";
	//		cin >> name;
	//		MercerBank.registerCustomer(id, name);
	//	}
	//	else if (input == "2") {
	//		string id;
	//		cout << "Enter Customer ID: ";
	//		cin >> id;
	//		Customer* customer = MercerBank.login(id);
	//		if (customer == nullptr) {
	//			cout << "Sorry something went wrong!" << endl;
	//			continue;
	//		}
	//		cout << "Welcome, " << customer->getName() << "!" << endl;
	//		//Add menu for other actions after login: 1. Open account, 2. View Account, 3. Deposit, 4. Withdraw, 5. Transfer Funds, 6. Logout
	//		while (true) {
	//			cout << "\nCustomer Menu:\n";
	//			cout << "1. Open Account\n";
	//			cout << "2. View Account\n";
	//			cout << "3. Deposit\n";
	//			cout << "4. Withdraw\n";
	//			cout << "5. Transfer Funds\n";
	//			cout << "6. Logout\n";
	//			cout << "Enter your choice: ";
	//			cin >> input;

	//			if (input == "1") {
	//				string accountNumber;
	//				double initialBalance;
	//				cout << "Enter new account number: ";
	//				cin >> accountNumber;
	//				cout << "Enter initial balance: ";
	//				cin >> initialBalance;
	//				customer->openAccount(accountNumber, initialBalance);
	//			}
	//			else if (input == "2") {
	//				cout << "Accounts:\n";
	//				for (const auto& account : customer->getAccounts()) {
	//					cout << "Account Number: " << account.getAccountNum() << ", Balance: " << account.getBalance() << endl;
	//				}
	//			}
	//			else if (input == "3") {
	//				string accountNumber;
	//				double amount;
	//				cout << "Enter account number: ";
	//				cin >> accountNumber;
	//				Account* account = customer->getAccount(accountNumber);
	//				if (account) {
	//					cout << "Enter amount to deposit: ";
	//					cin >> amount;
	//					account->deposit(amount);
	//				}
	//				else {
	//					cout << "Account not found." << endl;
	//				}
	//			}
	//			else if (input == "4") {
	//				string accountNumber;
	//				double amount;
	//				cout << "Enter account number: ";
	//				cin >> accountNumber;
	//				Account* account = customer->getAccount(accountNumber);
	//				if (account) {
	//					cout << "Enter amount to withdraw: ";
	//					cin >> amount;
	//					account->withdraw(amount);
	//				}
	//				else {
	//					cout << "Account not found." << endl;
	//				}
	//			}
	//			else if (input == "5") {
	//				string fromAccount, toAccount;
	//				double amount;
	//				cout << "Enter sender account number: ";
	//				cin >> fromAccount;
	//				cout << "Enter reciever account number: ";
	//				cin >> toAccount;

	//				Account* sender = customer->getAccount(fromAccount);
	//				Account* receiver = customer->getAccount(toAccount);

	//				if (sender && receiver) {
	//					cout << "Enter amount to transfer: ";
	//					cin >> amount;

	//					if (sender->getBalance() >= amount) {
	//						Transfer transfer(*sender, *receiver);
	//						transfer.setAmount(amount);
	//						sender->withdraw(amount);
	//						receiver->deposit(amount);
	//						cout << "Transfer successful!" << endl;
	//					}
	//					else {
	//						cout << "Insufficient funds in sender's account.";
	//					}
	//				}
	//				else {
	//					cout << "invlaid account numbers." << endl;
	//				}
	//			}

	//			else if (input == "6") {
	//				cout << "Logged out." << endl;
	//			}
	//			else {
	//				cout << "Invalid choice. Please try again." << endl;
	//				break;
	//			}
	//		}
	//	}

	//	else if (input == "3") {
	//		cout << "Goodbye!" << endl;
	//		break;
	//	}
	//	else {
	//		cout << "Invalid option. Please try again." << endl;
	//	}
	//}

	Bank MercerBank;

	cout << WELCOME_MESSAGE << endl;

	while (true) {
		cout << "\nMain Menu:\n";
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "3. Exit\n";
		cout << "What would you like to do today? ";

		int choice;
		cin >> choice;

		switch (choice) {
		case REGISTER:
			registerCustomer(MercerBank);
			break;

		case LOGIN:
			loginCustomer(MercerBank);
			break;

		case EXIT:
			cout << "Thanks for banking with us!" << endl;
			return 0;

		default:
			cout << INVALID_OPTION << endl;

		}
	}
	
	return 0;
}

// Function declarations

void registerCustomer(Bank& bank) {
	string name, username, pin;
	
	cout << "Enter your full legal name: ";
	cin.ignore(); // Error handling to allow any name to be entered
	getline(cin, name);

	cout << "Choose your username: ";
	cin >> username;

	cout << "Enter a 4-digit PIN for account registration: ";
	cin >> pin;
	if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
		cout << "Error: PIN must be exactly 4 digits." << endl;
		return;
	}

	bank.registerCustomer(name, username, pin);
}

void loginCustomer(Bank& bank) {
	string username, pin;

	cout << "Enter your username: ";
	cin >> username;

	cout << "Enter your PIN: ";
	cin >> pin;

	Customer* customer = bank.login(username, pin);
	if (!customer) {
		cout << "Login failed. Invalid username or PIN." << endl;
		return;
	}

	cout << "Welcome, " << customer->getName() << "!" << endl;
	customerMenu(customer, bank);
}

void customerMenu(Customer* customer, Bank& bank) {
	while (true) {
		cout << "\nCustomer Menu:\n";
		cout << "1. Open Account\n";
		cout << "2. View Accounts\n";
		cout << "3. Deposit\n";
		cout << "4. Withdraw\n";
		cout << "5. Transfer Funds\n";
		cout << "6. Logout\n";
		cout << "What would you like to do today? ";

		int choice;
		cin >> choice;

		switch (choice) {
		case NEW_ACCOUNT: {
			double initialBalance;
			cout << "Enter initial balance: ";
			cin >> initialBalance;

			int accountNumber = bank.generateAccountNumber();
			customer->openAccount(accountNumber, initialBalance);
			break;
		}

		case VIEW_ACCOUNTS: {
			cout << "Accounts:\n";
			for (const auto& account : customer->getAccounts()) {
				cout << "Account Number: " << account.getAccountNum() << ", Balance: " << account.getBalance() << endl;
			}
			break;
		}


		case DEPOSIT_AMOUNT: {
			string accountNumber;
			double amount;
			cout << "Enter account number: ";
			cin >> accountNumber;
			Account* account = customer->getAccount(accountNumber);
			if (account) {
				cout << "Enter amount to deposit: ";
				cin >> amount;
				account->deposit(amount);
			}
			else {
				cout << "Account not found." << endl;
			}
			break;
		}

		case WITHDRAW_AMOUNT: {
			string accountNumber;
			double amount;
			cout << "Enter account number: ";
			cin >> accountNumber;
			Account* account = customer->getAccount(accountNumber);
			if (account) {
				cout << "Enter amount to withdraw: ";
				cin >> amount;
				account->withdraw(amount);
			}
			else {
				cout << "Account not found." << endl;
			}
			break;
		}

		case TRANSFER_AMOUNT: {
			string fromAccount, toAccount;
			double amount;
			cout << "Enter sender account number: ";
			cin >> fromAccount;
			cout << "Enter reciever account number: ";
			cin >> toAccount;

			Account* sender = customer->getAccount(fromAccount);
			Account* receiver = customer->getAccount(toAccount);

			if (sender && receiver) {
				cout << "Enter amount to transfer: ";
				cin >> amount;

				if (sender->getBalance() >= amount) {
					Transfer transfer(*sender, *receiver);
					transfer.setAmount(amount);
					sender->withdraw(amount);
					receiver->deposit(amount);
					cout << "Transfer successful!" << endl;
				}
				else {
					cout << "Insufficient funds in sender's account.";
				}
			}
			else {
				cout << "invlaid account numbers." << endl;
			}
			break;
		}

		case LOGOUT:
			cout << "You've been logged out." << endl;
			return;

		default: 
			cout << INVALID_OPTION << endl;
		}
	}
}