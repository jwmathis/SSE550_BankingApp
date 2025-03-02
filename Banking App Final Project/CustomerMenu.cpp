#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include "CustomerMenu.h"
#include "Customer.h"
#include "SavingsAccount.h"

#include <iostream>
#include <string>
#include "SQLiteFunctions.h"
#include "UIHelpers.h"
#include "CustomerMenu.h"

// include files for FTXUI
#include <ftxui\component\component.hpp>
#include <ftxui\component\screen_interactive.hpp>
#include <ftxui\dom\elements.hpp>
using namespace ftxui;
// Bank function declarations
void displayCustomerAccountsMenu(Bank& bank, Customer* customer) {
	int countNumOfAccounts = 0;
	string accountType;
	cout << "Accounts:\n";
	auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
	for (const auto& account : displayAccounts) {
		countNumOfAccounts++;
		accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
		cout << countNumOfAccounts << ". Account Type: " << accountType << ", Account Number : " << account.getAccountNum() << ", Balance : " << account.getBalance() << endl;
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
	auto screen = ScreenInteractive::TerminalOutput();
	
	string name, username, pin;
	string error_message = "";

	auto nameInput = Input(&name, "Full Legal Name: ");
	auto usernameInput = Input(&username, "Username: ");
	auto pinInput = Input(&pin, "PIN: ");

	auto submitButton = Button("Register", [&] {
		if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
			error_message = "Error: PIN must be exactly 4 digits.";
			return;
		}

		if (bank.registerCustomer(name, username, pin)) {
			Customer* customer = bank.login(username, pin);
			if (customer) {
				screen.Exit();
				cout << "You've been registered " << customer->getName() << "! Thanks for signing up!" << endl;
				newCustomer(customer, bank);
			}
		}
		else {
			error_message = "Error: Failed to register user. Username already exists.";
		}

	});

	auto exitButton = Button("Cancel", [&] { screen.Exit(); });

	auto layout = Container::Vertical({
		nameInput,
		usernameInput,
		pinInput,
		Container::Horizontal({
			submitButton,
			exitButton
			}),
		});

	auto renderer = Renderer(layout, [&] {
		return vbox({
			text("Register New Account") | bold | center,
			separator(),
			text("Enter your details below:"),
			nameInput->Render(),
			usernameInput->Render(),
			pinInput->Render(),
			hbox({
				submitButton->Render() | center,
				exitButton->Render() | center,
				}),
			error_message.empty() ? text("") : text(error_message) | color(Color::Red),
			}) | border;
		});

	screen.Loop(renderer);
	system("cls");
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
	int accountNumber = bank.generateAccountNumber(1);
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
		cout << "1. Open Regular Account\n";
		cout << "2. Open Savings Account\n";
		cout << "3. View Accounts\n";
		cout << "4. Deposit\n";
		cout << "5. Withdraw\n";
		cout << "6. Transfer Funds\n";
		cout << "7. Close Account\n";
		cout << "8. Logout\n";
		cout << "What would you like to do today? ";

		int choice;
		cin >> choice;

		switch (choice) {
		case REG_ACCOUNT: {
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
			int accountNumber = bank.generateAccountNumber(1);
			if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance)) {
				cout << "Your Account has been created! Your account number is: " + to_string(accountNumber) << endl;
			}
			break;
		}
		
		case SAVINGS_ACC: {
			double initialBalance;
			double interestRate = 0.3;
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
			int accountNumber = bank.generateAccountNumber(SAVINGS_ACCOUNT);
			if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance)) {
				SavingsAccount savingsAccount(accountNumber, to_string(accountNumber), initialBalance, interestRate);
				cout << "Your Saviongs account has been created! Your account number is: " + to_string(accountNumber) << endl;
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
					transaction = "Deposit successful! New balance: " + to_string(account->getBalance());
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
			string transaction;

			string prompt = "Select an account by entering the corresponding number : ";
			selectedAccount = promptForAccountSelection(bank, customer, prompt);

			Account* account = bank.getAccountByNumber(selectedAccount); // Fetch the account from database

			if (account) {
				cout << "Enter amount to withdraw: ";
				cin >> amount;
				account->withdraw(amount); // Adjust balance in memory
				if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
					transaction = "Withdraw successful! New balance: " + to_string(account->getBalance());
					cout << transaction << endl;
					customer->generateTransactionReceipt(transaction);
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
			string transaction;

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
						transaction = "Transfer successful! New balance of your account is: " + to_string(recieverAccount->getBalance());
						cout << transaction << endl;
						customer->generateTransactionReceipt(transaction);

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
			string transaction;

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
					transaction = "Transfer successful! New balance of your account is: " + to_string(recieverAccount->getBalance());
					cout << transaction << endl;
					customer->generateTransactionReceipt(transaction);
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