// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "SQLiteFunctions.h"
#include "UIHelpers.h"
#include "CustomerMenu.h"

using namespace std;

// Global variables
char option[1];
bool userInput;
int choice;
const char* databaseDir = "MercerBank.db";

int main() {
	Bank MercerBank(databaseDir); //initialize bank and database
	bool exitFlag = false;

	cout << "Press enter to continue...\n";
	displayWelcomeAnimation();

	// Display Main menu options
	while (true) {

		cout << "\n" << WELCOME_MESSAGE << "\n" << endl; // Display welcome message
		printMU();
		cout << "\nMain Menu:\n";
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "3. Exit\n";
		cout << "What would you like to do today? ";

		cin >> choice;

		switch (choice) {
		case REGISTER:
			registerCustomer(MercerBank);
			break;

		case LOGIN: {
			if (!loginCustomer(MercerBank)) {
				cout << "Would you like to register (Y/N)?" << endl;
				cin >> option;
				userInput = strcmp(option, "Y");
				if (!userInput) {
					registerCustomer(MercerBank);
				}
			}
			break;
		}
		case EXIT: {
			cout << "Thanks for banking with us!" << endl;
			return 0;
		}
		
		case 8: {
			string name = "dummy_account";
			string username = "username";
			string pin = "1234";
			MercerBank.registerCustomer(name, username, pin);
			Customer* customer = MercerBank.login(username, pin);
			customerMenu(customer, MercerBank);
		}
		case 9: {// secret menu to view databases
			showAllTables(databaseDir);
			break;
		}

		default:
			cout << INVALID_OPTION << endl;

		}
	}

	return 0;
}