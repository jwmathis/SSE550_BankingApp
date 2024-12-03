#include <iostream>
#include <string>
#include <algorithm>
#include "CustomerMenu.h"

// Bank function declarations
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
	if (bank.registerCustomer(name, username, pin)) {
		Customer* customer = bank.login(username, pin);
		cout << "You've been registered " << customer->getName() << "! Thanks for signing up!" << endl;
		customerMenu(customer, bank);
	}
}

bool loginCustomer(Bank& bank) {
	string username, pin;

	cout << "Enter your username: ";
	cin >> username;

	cout << "Enter your PIN: ";
	cin >> pin;

	Customer* customer = bank.login(username, pin);
	if (!customer) {
		cout << "Login failed. Invalid username or PIN." << endl;
		return false;
	}
	cout << "Welcome, " << customer->getName() << "!" << endl;
	customerMenu(customer, bank);
	return true; // Login successful
}

void customerMenu(Customer* customer, Bank& bank) {
	while (true) {
		cout << "\nCustomer Menu:\n";
		cout << "1. Open Account\n";
		cout << "2. View Accounts\n";
		cout << "3. Deposit\n";
		cout << "4. Withdraw\n";
		cout << "5. Transfer Funds\n";
		cout << "6. Close Account\n";
		cout << "7. Logout\n";
		cout << "What would you like to do today? ";

		int choice;
		cin >> choice;

		switch (choice) {
		case NEW_ACCOUNT: {
			double initialBalance;
			bool repeat = false;
			cout << "Enter initial balance: ";
			do {
				repeat = false;
				cin >> initialBalance;
				if (initialBalance < 0) {
					cout << "Invalid input. Your account cannot be created with a negative balance. "
						"Please enter a positive balance or 0 to create your account.\n";
					repeat = true;
					cout << "Enter initial balance: ";
				}
			} while (repeat);
			int accountNumber = bank.generateAccountNumber();
			if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance)) {
				cout << "Your Account has been created! Your account number is: " + to_string(accountNumber) << endl;
			}
			break;
		}

		case BALANCE_INQUIRY: {
			cout << "Accounts:\n";
			auto accounts = bank.getAccountsForCustomer(customer->getId());
			for (const auto& account : accounts) {
				cout << "Account Number: " << account.getAccountNum() << ", Balance: " << account.getBalance() << endl;
			}
			break;
		}


		case DEPOSIT_AMOUNT: {
			string accountNumber;
			double amount;
			cout << "Enter account number: ";
			cin >> accountNumber;
			Account* account = bank.getAccountByNumber(accountNumber); // Fetch the account from database
			if (account) {
				cout << "Enter amount to deposit: ";
				cin >> amount;
				account->deposit(amount); // Adjust balance in memory
				if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
					cout << "Deposit successful. New balance: " << account->getBalance() << endl;
				}
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
			Account* account = bank.getAccountByNumber(accountNumber); // Fetch the account from the database
			if (account) {
				cout << "Enter amount to withdraw: ";
				cin >> amount;
				account->withdraw(amount); // Adjust balance in memory
				if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
					cout << "Withdraw successful. New balance: " << account->getBalance() << endl;
				}
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

			Account* sender = bank.getAccountByNumber(fromAccount);
			Account* receiver = bank.getAccountByNumber(toAccount);

			if (sender && receiver) {
				cout << "Enter amount to transfer: ";
				cin >> amount;

				if (sender->getBalance() >= amount) {
					Transfer transfer(*sender, *receiver);
					transfer.setAmount(amount);
					sender->withdraw(amount);
					receiver->deposit(amount);
					if (bank.updateAccountBalance(sender->getId(), sender->getBalance()) && bank.updateAccountBalance(receiver->getId(), receiver->getBalance())) {
						cout << "Transfer successful!" << endl;
					}
				}
				else {
					cout << "Insufficient funds in sender's account.";
				}
			}
			else {
				cout << "invalid account numbers." << endl;
			}
			break;
		}

		case CLOSE_ACCOUNT: {
			string fromAccount, toAccount;
			double amount;
			cout << "Enter account number you wish to close: ";
			cin >> fromAccount;
			cout << "Enter account number you wish to send funds to: ";
			cin >> toAccount;

			Account* sender = bank.getAccountByNumber(fromAccount);
			Account* receiver = bank.getAccountByNumber(toAccount);

			amount = sender->getBalance();

			if (sender && receiver) {
				Transfer transfer(*sender, *receiver);
				transfer.setAmount(amount);
				sender->withdraw(amount);
				receiver->deposit(amount);
				if (bank.updateAccountBalance(sender->getId(), sender->getBalance()) && bank.updateAccountBalance(receiver->getId(), receiver->getBalance())) {
					cout << "Transfer successful!" << endl;
				}
			}
			else {
				cout << "Invalid account numbers." << endl;
			}

			bank.accountRemoveAccount(sender->getAccountNum());
			break;
		}

		case LOGOUT: {
			cout << "You've been logged out." << endl;
			return;
		}

		default: {
			cout << INVALID_OPTION << endl;
		}
		}
	}
}