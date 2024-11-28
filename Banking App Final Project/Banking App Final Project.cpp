// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <algorithm>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"
#include "Transfer.h"
#include "sqlite3.h" // For database management

using namespace std;

/*TODO:
Current Issues: SQL throws exception when there is a duplicate username
1. Update classes to look for database file to read user login information
2. Potentially add table rows to store account information for each user*/

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
	LOGOUT
};

// Constants
const string WELCOME_MESSAGE = "Welcome to Mercer Money Banking!";
const string INVALID_OPTION = "Invalid option. Please try again.";

// Functions prototypes for modularization
void registerCustomer(Bank& bank, sqlite3* db);
void loginCustomer(Bank& bank);
void customerMenu(Customer* customer, Bank& bank);

// SQLite Database management functions
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void createTable(sqlite3* db) {
	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS USERS ( "
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"USERNAME TEXT NOT NULL UNIQUE, "
		"PIN TEXT NOT NULL, "
		"NAME TEXT NOT NULL);";

	char* errMsg = 0;
	int rc = sqlite3_exec(db, createTableSQL, callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cerr << "SQL error: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
	else {
		cout << "Table created successfully" << endl;
	}
}

int insertUser(sqlite3* db, const string& username, const string& password, const string& name) {
	string insertSQL = "INSERT INTO USERS (USERNAME, PIN, NAME) VALUES ( '" + username + "', '" + password + "', '" + name + "'); ";
	char* errMsg = 0;
	int rc = sqlite3_exec(db, insertSQL.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK) {
		if (string(errMsg).find("UNIQUE constraint failed") != string::npos) {
			cout << "Error: Username already exists." << endl;
			return 0;
		}
		else {
			cerr << "SQL error: " << errMsg << endl;
			return 0;
		}
		sqlite3_free(errMsg);
	}
	else {
		cout << "User " << username << " registered successfully." << endl;
		return 1;
	}
}

sqlite3* openDatabase(const string& dbName) {
	sqlite3* db;
	int rc = sqlite3_open(dbName.c_str(), &db);
	if (rc) {
		cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
		return nullptr;
	}
	else {
		cout << "Opened database successfully." << endl;
		return db;
	}
}

int main() {
	// Open/Create the database
	sqlite3* db = openDatabase("bank.db");
	if (!db) return -1;

	// Create users table if not exists
	createTable(db);

	Bank MercerBank; //initialize bank

	cout << WELCOME_MESSAGE << endl; // Display welcome message

	// Display Main menu options
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
			registerCustomer(MercerBank, db);
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

void registerCustomer(Bank& bank, sqlite3* db) {
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

	int userDoesNotExist = insertUser(db, username, pin, name); // Add user to the database
	if (userDoesNotExist) { bank.registerCustomer(name, username, pin); }
	sqlite3_close(db); // Close the database
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

		case BALANCE_INQUIRY: {
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




