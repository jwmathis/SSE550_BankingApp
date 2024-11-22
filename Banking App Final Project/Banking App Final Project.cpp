// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"
#include "Transfer.h"

using namespace std;


int main() {

	//--------------------------------------------------------------------------------------
	//  Constant Declaration:

	const int New_Account = 1;
	const int Balance_Inquiry = 2;
	const int Deposit_Amount = 3;
	const int Withdraw_Amount = 4;
	const int Transfer_Amount = 5;
	const int Close_Account = 6;
	const int EXIT = 7;

	string	userName, password, firstName, lastName, status;
	int userInput = 0;
	string input;
	//--------------------------------------------------------------------------------------
	Bank MercerBank;

	// Welcome Screen 
	cout << "Welcome to Mercer Money Banking!" << endl;

	while (true) {
		cout << "\nMenu:\n";
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "2. Exit\n";
		cout << "Enter option: ";
		cin >> input;

		if (input == "1") {
			string id, name;
			cout << "Enter Customer ID: ";
			cin >> id;
			cout << "Enter Name: ";
			cin >> name;
			MercerBank.registerCustomer(id, name);
		}
		else if (input == "2") {
			string id;
			cout << "Enter Customer ID: ";
			cin >> id;
			Customer* customer = MercerBank.login(id);
			if (customer == nullptr) {
				cout << "Sorry something went wrong!" << endl;
				continue;
			}
			cout << "Welcome, " << customer->getName() << "!" << endl;
			//Add menu for other actions after login: 1. Open account, 2. View Account, 3. Deposit, 4. Withdraw, 5. Transfer Funds, 6. Logout
			while (true)
		}
		else if (input == "3") {
			cout << "Goodbye!" << endl;
			break;
		}
		else {
			cout << "Invalid option. Please try again." << endl;
		}
	}
}