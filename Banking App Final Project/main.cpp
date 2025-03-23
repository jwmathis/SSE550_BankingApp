// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "SQLiteFunctions.h"
#include "UIHelpers.h"
#include "CustomerMenu.h"

// Include files for FTXUI
#include <functional>
#include <vector>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <memory>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define ORANGE "\033[38;2;255;165;0m"

using namespace std;
using namespace ftxui;

// Global variables
char option[1];
bool userInput;
int choice;
const char* databaseDir = "MercerBank.db";

Component Window(string title, Component component) {
	return Renderer(component, [component, title] {
		return window(text(title), component->Render()) | flex;
	});
}
int main() {
	Bank MercerBank(databaseDir); //initialize bank and database
	bool exitFlag = false;

	cout << "Press enter to continue...\n";
	displayWelcomeAnimation();

	auto screen = ScreenInteractive::TerminalOutput(); // Initialize the screen
	vector<string> MainMenuEntries = { // Main menu
		"1. Register",
		"2. Login",
		"3. Exit",
		"4. Admin"
	};

	int selectedMenuEntry = 0; // Selected menu entry
	auto menuOption = MenuOption(); // Menu options
	menuOption.on_enter = screen.ExitLoopClosure(); // Exit loop
	auto menu = Menu(&MainMenuEntries, &selectedMenuEntry, menuOption); // Menu

	// Display Main menu options
	while (true) {

		clearScreen(); // Clear the console
		printDollarSign(); // Print the static dollar sign
		cout << "\n" << WELCOME_MESSAGE << "\n" << endl; // Display welcome message
		printMU(); // Print the MU logo

		screen.Loop(Window("Mercer Bank", menu)); // Display the menu
		selectedMenuEntry += 1; // Increment the selected menu entry for switch case selection

		switch (selectedMenuEntry) {
		case REGISTER:
			clearScreen();
			registerCustomer(MercerBank);
			break;

		case LOGIN: {
			if (!loginCustomer(MercerBank)) {
				auto screen = ScreenInteractive::TerminalOutput();
				string userChoice;
				bool proceedToRegister = false;

				auto messageText = text("Login failed. Would you like to register (Y/N)?") | bold | center;

				auto yesButton = Button("Yes", [&] {
					proceedToRegister = true;
					screen.Exit();
				});

				auto noButton = Button("No", [&] {
					proceedToRegister = false;
					screen.Exit();
				});

				auto layout = Container::Vertical({
					Container::Horizontal({
					yesButton,
					noButton,
					}),
				});

				auto renderer = Renderer(layout, [&] {
					return vbox({
							messageText,
							hbox({
								yesButton->Render() | center,
								noButton->Render() | center,
							}),
					}) | border;
				});

				screen.Loop(renderer);

				if (proceedToRegister) {
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
		case 9: {// secret menu to view databases
			showAllTables(databaseDir);
			break;
		}

		default:
			cout << "Enter your choice: " << endl;
			cin >> selectedMenuEntry;

			if (selectedMenuEntry == 99) {
				cout << "Accessing secret menu...\n";
				showAllTables(databaseDir);
				system("pause");
				clearScreen();
			}
			else {
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
				default:
					cout << INVALID_OPTION << endl;
				}
			}

		}
	}

	return 0;
}