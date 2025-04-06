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
#include "Bank.h"
#include "Admin.h"

using namespace std;
using namespace ftxui;

void merge (vector<Bank::acc>& accounts, int low, int mid, int high) { // Merge Sort
	vector<Bank::acc> temp(high - low + 1);

	int i = low, j = mid + 1, k = 0;

	while (i <= mid && j <= high) {
		if (accounts[i].balance < accounts[j].balance) {
			temp[k++] = accounts[i++];
		}
		else {
			temp[k++] = accounts[j++];
		}
	}

	while (i <= mid) {
		temp[k++] = accounts[i++];
	}

	while (j <= high) {
		temp[k++] = accounts[j++];
	}

	for (int i = low; i <= temp.size(); i++) {
		accounts[low + i] = temp[i];
	}
}

void merge_recurse(vector<Bank::acc>& accounts, int low, int high) {
	if (low >= high) {
		return;
	}
	
	int mid = (low + high) / 2;
	merge_recurse(accounts, low, mid);
	merge_recurse(accounts, mid + 1, high);
	merge(accounts, low, mid, high);
	
}

void mergeSort(vector<Bank::acc>& accounts, int length) {
	merge_recurse(accounts, 0, length - 1);
}

int findLowerBound(const vector<Bank::acc>& accounts, double minBalance) { // Binary Search: Returns the index of the first account with a balance greater than or equal to minBalance
	int low = 0, high = accounts.size() - 1;
	while (low < high) {
		int mid = low + (high - low) / 2;
		if (accounts[mid].balance < minBalance) {
			low = mid + 1;
		}
		else {
			high = mid;
		}
	}

	return (accounts[low].balance >= minBalance) ? low : -1;
}


vector<Bank::acc> getAccountsInBalanceRange(vector<Bank::acc>& accounts, double minBalance, double maxBalance) {
	std::sort(accounts.begin(), accounts.end(), [](const Bank::acc& a, const Bank::acc& b) {
		return a.balance < b.balance; // Compare based on balance
		});
	//mergeSort(accounts, accounts.size());

	int startIdx = findLowerBound(accounts, minBalance);
	if (startIdx == -1) return {};

	vector<Bank::acc> filteredAccounts;
	for (int i = startIdx; i < accounts.size() && accounts[i].balance <= maxBalance; i++) {
		filteredAccounts.push_back(accounts[i]);
	}

	return filteredAccounts;
}

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

		case ADMIN: {
			// Admin Login Process
			system("cls");
			auto screen = ScreenInteractive::TerminalOutput();

			string username, password;
			bool loginStatus = false;
			string error_message = "";
			string loginMessage = "Please enter your admin credentials:";

			auto usernameInput = Input(&username, "Username (admin): ");
			auto passwordInput = Input(&password, "Password (admin): ");

			auto submitButton = Button("Login", [&] {
				if (username.empty() || password.empty()) {
					error_message = "Error: Please enter both username and password.";
					return;
				}
				else if (username == "admin" && password == "admin") {
					loginStatus = true;
					screen.Exit();
				}
				else {
					error_message = "Login failed. Invalid username or password.";
					loginStatus = false;
					return;
				}
				});

			auto backButton = Button("Back to Main Menu", [&] { loginStatus = false; screen.Exit(); });

			auto layout = Container::Vertical({
				usernameInput | flex,
				passwordInput | flex,
				Container::Horizontal({
					submitButton | flex,
					backButton | flex
				}) | flex
				});


			auto renderer = Renderer(layout, [&] {
				return vbox({
					text(loginMessage) | bold | center,
					usernameInput->Render() | center,
					passwordInput->Render() | center,
					hbox({
						submitButton->Render() | center,
						backButton->Render() | center,
					}),
					}) | border;
				});

			screen.Loop(renderer);

			if (loginStatus) {
				string adminMenuChoice;

				auto adminMenuEntries = vector<string>{
					"1. View All Accounts",
					"2. Search for Accounts By Balance Range",
					"3. Search for Accounts By Name",
					"4. Dummy Account",
					"5. Exit"
				};

				int selectedAdminMenuEntry = 0;
				auto adminMenuOption = MenuOption();
				adminMenuOption.on_enter = screen.ExitLoopClosure();

				auto adminMenu = Menu(&adminMenuEntries, &selectedAdminMenuEntry, adminMenuOption);
				bool flag = true;
			while (flag) {
				screen.Loop(Window("Admin Menu", adminMenu));
				selectedAdminMenuEntry += 1;
				switch (selectedAdminMenuEntry) {
				case 1: {
					system("cls");
					showAllTables(databaseDir);
					system("pause");
					clearScreen();
					break;
				}
				case 2: {
					system("cls");

					// Prepare the screen and inputs
					auto screen = ScreenInteractive::TerminalOutput();
					string minBalanceString, maxBalanceString;
					double minBalance = 0.0, maxBalance = 0.0;
					string error_message = "";

					// Inputs for balance range
					auto minBalanceInput = Input(&minBalanceString, "Enter minimum balance: ");
					auto maxBalanceInput = Input(&maxBalanceString, "Enter maximum balance: ");

					bool submitClicked = false;

					// Submit button to confirm the balance range
					auto submitButton = Button("Submit", [&] {
						try {
							// Try to convert strings to double
							minBalance = stod(minBalanceString);
							maxBalance = stod(maxBalanceString);

							// Validate the balance range
							if (minBalance > maxBalance) {
								error_message = "Minimum balance cannot be greater than maximum balance!";
								return;
							}

							submitClicked = true; // Proceed to filter accounts after submitting
							screen.Exit();
						}
						catch (const std::invalid_argument&) {
							error_message = "Invalid input. Please enter valid numbers for balance.";
						}
						catch (const std::out_of_range&) {
							error_message = "Balance values are too large!";
						}
						});

					// Cancel button to exit without filtering
					auto cancelButton = Button("Cancel", [&] { screen.Exit(); });

					// Layout of inputs and buttons
					auto layout = Container::Vertical({
						minBalanceInput,
						maxBalanceInput,
						Container::Horizontal({
							submitButton,
							cancelButton
						})
						});

					// Renderer to display layout and inputs
					auto renderer = Renderer(layout, [&] {
						return vbox({
							text("Enter balance range to filter accounts:") | bold | center,
							separator(),
							minBalanceInput->Render(),
							maxBalanceInput->Render(),
							hbox({
								submitButton->Render() | center,
								cancelButton->Render() | center,
							}),
							error_message.empty() ? text("") : text(error_message) | color(Color::Red),
							}) | border;
						});

					// Loop to display the screen until the user clicks submit or cancel
					screen.Loop(renderer);

					// After submit, filter and display the accounts
					if (submitClicked) {
						// Fetch accounts from database
						vector<Bank::acc> accounts = MercerBank.fetchAccountsFromDatabase();

						// Filter the accounts based on the given balance range
						vector<Bank::acc> filteredAccounts = getAccountsInBalanceRange(accounts, minBalance, maxBalance);

						// Display the filtered accounts
						if (filteredAccounts.empty()) {
							error_message = "No accounts found in the specified balance range.";
							screen.Loop(renderer);
						}
						else {
							// Display accounts
							for (const auto& acc : filteredAccounts) {
								cout << "Account ID: " << acc.accountId << endl;
								cout << "User ID: " << acc.userId << endl;
								cout << "Account Number: " << acc.accountNumber << endl;
								cout << "Balance: $" << acc.balance << endl;
								cout << "Account Type: " << acc.accountType << endl;
								cout << endl;
							}
							system("pause");
							system("cls");
						}
					}

					break;
				}
				case 3: {
					system("cls");
					vector<Bank::acc> accounts = MercerBank.fetchAccountsFromDatabase();
					AccountBST bst;
					bst.buildTree(accounts);

					// Display all
					//bst.displayAllAccounts();

					string inputAccNumber;
					cout << "Enter the account number to find: ";
					cin >> inputAccNumber;
					Bank::acc* result = bst.searchAccount(inputAccNumber);
					if (result) {
						cout << "Account found:\n";
						bst.printAccount(*result);
					}
					else {
						cout << "Account not found.\n";
					}
					system("pause");
					system("cls");
					break;
				}

				case 4: {

					string name = "dummy_account";
					string username = "username";
					string pin = "1234";
					MercerBank.registerCustomer(name, username, pin);
					Customer* customer = MercerBank.login(username, pin);
					customerMenu<double>(customer, MercerBank);
					break;
				}

				case 5: {
					flag = false;
					break;
				}
				default:
					break;
				}
			}
				break;
			}
		}

		default:
			cout << "Thanks for banking with us!" << endl;
			return 0;

		return 0;
		}
	}
}