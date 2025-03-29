#pragma once
#include <string>
#include <vector>
#include "Account.h"
#include "sqlite3.h"
#include <stack>
#include <queue>
using namespace std;

class Customer {
private:
	int id;				// Unique ID in the database
	string name;		// Customer' name for accounts's full name on the account
	string username;	// Unique username for login
	string pin;			// PIN for validation
	struct Transaction {
		string type; // e.g. "Deposit" or "Withdrawal"
		double amount;
		string timestamp;
		Transaction* next; // Pointer to the next transaction

		Transaction(const string& type, double amount, const string& timestamp)
			: type(type), amount(amount), timestamp(timestamp), next(nullptr) {}
	};

	Transaction* transactionHead; // Header of the linked list
	stack<Transaction*> undoStack; // Stack for undoing transactions
	queue<string> helpRequestQueue; // Queue for help requests

public:
	Customer();
	Customer(int id, const string& name, const string& username, const string& pin);
	~Customer();

	// Getters
	int getId() const;
	const string& getName() const;
	const string& getUsername() const;

	// Behavior methods
	bool verifyPin(const string& inputPin) const; // Method to validate PIN
	string generateTransactionReceiptFilename();
	void generateTransactionReceipt(const string& transaction);

	// Transaction methods
	void addTransaction(const string& type, double amount, const string& timestamp);
	void undoTransaction();
	void addHelpRequest(const string& request);
	bool removeHelpRequest();
	queue<std::string> getHelpRequests();
	void displayTransactionHistory(); // To integrate with FTXUI
};
