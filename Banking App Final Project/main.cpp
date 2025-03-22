// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "SQLiteFunctions.h"
#include "UIHelpers.h"
#include "CustomerMenu.h"

// include files for FTXUI
#include <functional>
#include <vector>
#include <ftxui\component\captured_mouse.hpp>
#include <ftxui\component\component.hpp>
#include <ftxui\component\component_base.hpp>
#include <ftxui\component\component_options.hpp>
#include <ftxui\component\screen_interactive.hpp>
#include <ftxui\dom\elements.hpp>
#include <memory>
using namespace std;
using namespace ftxui;
// Global variables
char option[1];
bool userInput;
int choice;
const char* databaseDir = "MercerBank.db";
const string green = "\033[32m";
const string reset = "\033[0m";

Component Window(std::string title, Component component) {
	return Renderer(component, [component, title] {  //
		return window(text(title), component->Render()) | flex;
		});
}
int main() {
	Bank MercerBank(databaseDir); //initialize bank and database
	bool exitFlag = false;

	cout << "Press enter to continue...\n";
	displayWelcomeAnimation();
	clearScreen();
	cout << green;
	cout << "                                          $$        " << endl;
	cout << "                                         $$$$       " << endl;
	cout << "                                       $$$  $$$     " << endl;
	cout << "                                      $$  $$  $$    " << endl;
	cout << "                                      $$  $$          " << endl;
	cout << "                                       $$$$$$$$$       " << endl;
	cout << "                                          $$   $$      " << endl;
	cout << "                                      $$  $$   $$    " << endl;
	cout << "                                       $$$  $$$     " << endl;
	cout << "                                         $$$$       " << endl;
	cout << "                                          $$        " << endl;
	cout << reset;
	cout << "\n" << WELCOME_MESSAGE << "\n" << endl; // Display welcome message
	printMU();

	//auto screen = ScreenInteractive::TerminalOutput();
	auto screen = ScreenInteractive::TerminalOutput();
	vector<string> MainMenuEntries = {
		"1. Register",
		"2. Login",
		"3. Exit",
		"4. Testing"
	};

	int selectedMenuEntry = 0;
	auto menuOption = MenuOption();
	menuOption.on_enter = screen.ExitLoopClosure();
	auto menu = Menu(&MainMenuEntries, &selectedMenuEntry, menuOption);

	// Display Main menu options
	while (true) {
		screen.Loop(Window("Mercer Bank", menu));
		selectedMenuEntry += 1;

		switch (selectedMenuEntry) {
		case REGISTER:
			system("cls");
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
			break;
		}
		case 4: {// secret menu to view databases
			showAllTables(databaseDir);
			system("pause");
			system("cls");
			break;
		}

		default:
			cout << INVALID_OPTION << endl;

		}
	}

	return 0;
}