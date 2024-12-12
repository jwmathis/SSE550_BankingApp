#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include "CustomerMenu.h"
#include "Customer.h"

// Bank function declarations

void displayCustomerAccountsMenu(Bank& bank, Customer* customer) {
	int countNumOfAccounts = 0;
	cout << "Accounts:\n";
	auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
	for (const auto& account : displayAccounts) {
		countNumOfAccounts++;
		cout << countNumOfAccounts << ". Account Number: " << account.getAccountNum() << ", Balance: " << account.getBalance() << endl;
	}
}

string selectCustomerAccount(Bank& bank, Customer* customer, int userSelection) {
	int countNumOfAccounts = 0;

	// Get list of customer's accounts
	auto customerAccounts = bank.getAccountsForCustomer(customer->getId());

	for (const auto& account : customerAccounts) {
		countNumOfAccounts++;

		// Check if selected account is customer selection
		if (userSelection == countNumOfAccounts) {
			return account.getAccountNum(); // Fetch the account from database
		}
	}

	return ""; // If no valid selection
}

string promptForAccountSelection(Bank& bank, Customer* customer, const string& promptMessage) {
	string selectedAccount;
	int userSelection;

	displayCustomerAccountsMenu(bank, customer);

	do {
		cout << promptMessage;
		cin >> userSelection;

		selectedAccount = selectCustomerAccount(bank, customer, userSelection);

		if (selectedAccount.empty()) {
			cout << "Invalid selection. Please try again." << endl;
		}
	} while (selectedAccount.empty());

	return selectedAccount;
}

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
		newCustomer(customer, bank);
	}
}

//New registered customers will go straight to Option 1 to open an account
void newCustomer(Customer* customer, Bank& bank) {
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
			displayCustomerAccountsMenu(bank, customer);
			break;
		}


		case DEPOSIT_AMOUNT: {
			string selectedAccount;
			double amount;
			string transaction;

			string prompt = "Select an account by entering the corresponding number : ";	
			selectedAccount = promptForAccountSelection(bank, customer, prompt);

			Account* account = bank.getAccountByNumber(selectedAccount); // Fetch the account from database

			if (account) {
				cout << "Enter amount to deposit: ";
				cin >> amount;
				account->deposit(amount); // Adjust balance in memory
				if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
					transaction = "Deposit successful. New balance: " + to_string(account->getBalance());
					cout << transaction << endl;
					customer->generateTransactionReceipt(transaction);
				}
			}
			else {
				cout << "Account not found." << endl;
			}
			break;
		}

		case WITHDRAW_AMOUNT: {
			string selectedAccount;
			double amount;

			string prompt = "Select an account by entering the corresponding number : ";
			selectedAccount = promptForAccountSelection(bank, customer, prompt);

			Account* account = bank.getAccountByNumber(selectedAccount); // Fetch the account from database

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
			string selectedAccount;
			double amount;
			int userSelection;

			string senderAccountNumber = promptForAccountSelection(bank, customer, "Select the account to transfer from by entering the corresponding number: ");
			string recieverAccountNumber = promptForAccountSelection(bank, customer, "Select the account to transfer to by entering the corresponding number: ");

			Account* senderAccount = bank.getAccountByNumber(senderAccountNumber); // Fetch the sender account from database
			Account* recieverAccount = bank.getAccountByNumber(recieverAccountNumber); // Fetch the recieverr account from database

			if (senderAccount && recieverAccount) {
				cout << "Enter amount to transfer: ";
				cin >> amount;

				if (senderAccount->getBalance() >= amount) {
					Transfer transfer(*senderAccount, *recieverAccount);
					transfer.setAmount(amount);
					senderAccount->withdraw(amount);
					recieverAccount->deposit(amount);

					if (bank.updateAccountBalance(senderAccount->getId(), senderAccount->getBalance()) && bank.updateAccountBalance(recieverAccount->getId(), recieverAccount->getBalance())) {
						cout << "Transfer successful!" << endl;
					}
				}
				else {
					cout << "Insufficient funds in sender's account.";
				}
			}
			else {
				cout << "Invalid account numbers." << endl;
			}
			break;
		}

		case CLOSE_ACCOUNT: {

			string selectedAccount;
			double amount;
			int userSelection;

			string senderAccountNumber = promptForAccountSelection(bank, customer, "Select the account you wish to close: ");
			string recieverAccountNumber = promptForAccountSelection(bank, customer, "Select the account you wish to transfer money to: ");

			Account* senderAccount = bank.getAccountByNumber(senderAccountNumber); // Fetch the sender account from database
			Account* recieverAccount = bank.getAccountByNumber(recieverAccountNumber); // Fetch the recieverr account from database

			amount = senderAccount->getBalance();

			if (senderAccount && recieverAccount) {
				Transfer transfer(*senderAccount, *recieverAccount);
				transfer.setAmount(amount);
				senderAccount->withdraw(amount);
				recieverAccount->deposit(amount);
				if (bank.updateAccountBalance(senderAccount->getId(), senderAccount->getBalance()) && bank.updateAccountBalance(recieverAccount->getId(), recieverAccount->getBalance())) {
					cout << "Transfer successful!" << endl;
				}
			}
			else {
				cout << "Invalid account numbers." << endl;
			}

			bank.accountRemoveAccount(senderAccount->getAccountNum());
			break;
		}

		case LOGOUT: {
			string filename = customer->generateTransactionReceiptFilename();
			remove(filename.c_str());
			cout << "You've been logged out." << endl;
			return;
		}

		default: {
			cout << INVALID_OPTION << endl;
		}
		}
	}
}