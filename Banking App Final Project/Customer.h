#pragma once
#include <string>
#include <vector>
#include "Account.h"
#include "sqlite3.h"

using namespace std;

class Customer {
private:
	int id;				// Unique ID in the database
	string name;		// Customer' name for accounts's full name on the account
	string username;	// Unique username for login
	string pin;			// PIN for validation

public:
	Customer(int id, const string& name, const string& username, const string& pin);

	// Getters
	int getId() const;
	const string& getName() const;
	const string& getUsername() const;

	// Behavior methods
	bool verifyPin(const string& inputPin) const; // Method to validate PIN
};
