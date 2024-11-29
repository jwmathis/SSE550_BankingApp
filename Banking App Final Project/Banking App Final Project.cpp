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
#include <iomanip>

using namespace std;
bool printedHeaders = false;
/*TODO:
Current Issues:
1. Database balances don't update when using withdraw, transfer, deposit functions
2. After using withdraw, transfer, and deposit functions, balances in view accounts does not update*/

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
void registerCustomer(Bank& bank);
void loginCustomer(Bank& bank);
void customerMenu(Customer* customer, Bank& bank);

// SQLite Database management functions
static int createDB(const char* s);
static int createTable(const char* s);
static int showAllData(const char* s);
static int showAllTables(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
//static int insertUser(const char* s, const string& name, const string& username, const string& pin);
//static int addAccount(const char* s, const string& username, const string& accountNumber, double balance);
//static int selectUserData(const char* s, const string& username);
//static int selectAccounts(const char* s, const string& username);


int main() {
	// Open/Create the database
	const char* databaseDir = "MercerBank.db"; 
	createDB(databaseDir);
	createTable(databaseDir);
	Bank MercerBank(databaseDir); //initialize bank and database

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
			registerCustomer(MercerBank);
			break;

		case LOGIN:
			loginCustomer(MercerBank);
			break;

		case EXIT:
			cout << "Thanks for banking with us!" << endl;
			return 0;

		case 9: // secret menu to view databases
			//printedHeaders = false;
			showAllTables(databaseDir);
			break;

		default:
			cout << INVALID_OPTION << endl;

		}
	}
	
	return 0;
}

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
		cout << "You've been registered! Thanks for signing up!" << endl;
	}
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
			if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance)) {
				cout << "Your Account has been created! Your account number is: " +  to_string(accountNumber) << endl;
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
					cout << "Deposit successful. New balance: " << account->getBalance() << endl;
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

// SQLite Database management functions
static int createDB(const char* s) {
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);
	return 0;
}

static int createTable(const char* s) {
	sqlite3* DB;
	string sqlUsersTable = "CREATE TABLE IF NOT EXISTS users ( "
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name TEXT NOT NULL, "
		"username TEXT NOT NULL UNIQUE, "
		"pin TEXT NOT NULL);";

	string sqlAccountsTable = "CREATE TABLE IF NOT EXISTS accounts ( "
		"account_id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"user_id INTEGER NOT NULL, "
		"account_number TEXT NOT NULL, "
		"balance DOUBLE NOT NULL DEFAULT 0.0, "
		"FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE);";
	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);
		char* messageError;
		exit = sqlite3_exec(DB, sqlUsersTable.c_str(), NULL, 0, &messageError);

		if (exit != SQLITE_OK) {
			cerr << "Error creating users table" << endl;
			sqlite3_free(messageError);
		} 
		else {
			cout << "Users table created successfully" << endl;
		}

		exit = sqlite3_exec(DB, sqlAccountsTable.c_str(), NULL, 0, &messageError);

		if (exit != SQLITE_OK) {
			cerr << "Error creating accounts table" << endl;
			sqlite3_free(messageError);
		}
		else {
			cout << "Accounts table created successfully" << endl;
		}

		sqlite3_close(DB);
	}
	catch (const exception& e)
	{
		cerr << e.what();
	}

	return 0;
}

static int showAllData(const char* s) {
	sqlite3* DB;

	int exit = sqlite3_open(s, &DB);

	string sql = "SELECT * FROM users";

	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

	return 0;
}

static int showAllTables(const char* s) {
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);

	if (exit != SQLITE_OK) {
		cerr << "Error opening databases: " << sqlite3_errmsg(DB) << endl;
		return 1;
	}
	printedHeaders = false; // Flag for formatting table data
	string sqlUsers = "SELECT * FROM users;";
	cout << "Users Table: " << endl;
	sqlite3_exec(DB, sqlUsers.c_str(), callback, (void*)"Users", NULL);

	printedHeaders = false; // Flag for formatting table data
	string sqlAccounts = "SELECT * FROM accounts;";
	cout << "\nAccounts Table:" << endl;
	sqlite3_exec(DB, sqlAccounts.c_str(), callback, (void*)"Accounts", NULL);

	sqlite3_close(DB);
	return 0;
}

static int callback(void* data, int argc, char** argv, char** azColName) {
	const char* table = (const char*)data;

	if (argv != NULL && !printedHeaders) {
		if (table != NULL) {
			cout << "\n" << table << " Data: " << endl;
			cout << "------------------------------------------------------------------------------------" << endl;
		}
		for (int i = 0; i < argc; i++) {
			cout << left << setw(20) << azColName[i] << "|";
		}
		cout << "\n------------------------------------------------------------------------------------" << endl;
		printedHeaders = true;
	}

	for (int i = 0; i < argc; i++) {
		cout << left << setw(20) << (argv[i] ? argv[i] : "NULL") << "|";
	}
	cout << endl;
	return 0;
}

//static int insertUser(const char* s, const string& name, const string& username, const string& pin) {
//	sqlite3* DB;
//	char* messageError;
//	int num = -99;
//	int exit = sqlite3_open(s, &DB);
//
//	string sql("INSERT INTO users (name, username, pin) VALUES ( '" + name + "', '" + username + "', '" + pin + "');");
//
//	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
//	if (exit != SQLITE_OK) {
//		cerr << "Error inserting" << endl;
//		sqlite3_free(messageError);
//		num = 1;
//	}
//	else {
//		cout << "User added to database successfully" << endl;
//		num = 0;
//	}
//
//	return num;
//}

//static int addAccount(const char* s, const string& username, const string& accountNumber, double balance) {
//	sqlite3* DB;
//	char* messageError;
//
//	int exit = sqlite3_open(s, &DB);
//
//	string userIdSQL = "SELECT id FROM users WHERE username = '" + username + "';";
//	sqlite3_stmt* stmt;
//	exit = sqlite3_prepare_v2(DB, userIdSQL.c_str(), -1, &stmt, 0);
//
//	if (exit != SQLITE_OK) {
//		cerr << "SQL error (prepare): " << sqlite3_errmsg(DB) << endl;
//		return 1;
//	}
//
//	int userId = -1;
//	if (sqlite3_step(stmt) == SQLITE_ROW) {
//		userId = sqlite3_column_int(stmt, 0);
//	}
//	else {
//		cerr << "Error: User not found" << endl;
//		sqlite3_finalize(stmt);
//		return 1;
//	}
//	sqlite3_finalize(stmt);
//
//	string insertSQL = "INSERT INTO accounts (user_id, account_number, balance) VALUES (" +
//		to_string(userId) + ", '" + accountNumber + "', " + to_string(balance) + ");";
//	exit = sqlite3_exec(DB, insertSQL.c_str(), NULL, 0, &messageError);
//
//	if (exit != SQLITE_OK) {
//		cerr << "Error inserting account" << endl;
//		sqlite3_free(messageError);
//		return 1;
//	}
//
//	cout << "Account added successfully" << endl;
//	sqlite3_close(DB);
//	return 0;
//}

//static int selectUserData(const char* s, const string& username) {
//	sqlite3* DB;
//
//	int exit = sqlite3_open(s, &DB);
//
//	string sql = "SELECT * FROM users WHERE username = '" + username + "';";
//
//	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
//	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
//
//	return 0;
//}
//
//static int selectAccounts(const char* s, const string& username) {
//	sqlite3* DB;
//	int exit = sqlite3_open(s, &DB);
//
//	// Fetch user ID for the given username
//	string userIdSQL = "SELECT id FROM users WHERE username = '" + username + "';";
//	sqlite3_stmt* stmt;
//	exit = sqlite3_prepare_v2(DB, userIdSQL.c_str(), -1, &stmt, 0);
//
//	if (exit != SQLITE_OK) {
//		cerr << "SQL error (prepare): " << sqlite3_errmsg(DB) << endl;
//		return 1;
//	}
//
//	int userId = -1;
//	if (sqlite3_step(stmt) == SQLITE_ROW) {
//		userId = sqlite3_column_int(stmt, 0);
//	}
//	else {
//		cerr << "Error: User not found" << endl;
//		sqlite3_finalize(stmt);
//		return 1;
//	}
//	sqlite3_finalize(stmt);
//
//	// Query accounts for the user
//	string sql = "SELECT * FROM accounts WHERE user_id = " + to_string(userId) + ";";
//	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);
//	sqlite3_close(DB);
//
//	return 0;
//}



