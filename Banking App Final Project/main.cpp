// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include "SQLiteFunctions.h"
#include "UIHelpers.h"
#include "CustomerMenu.h"
#include "Bank.h"
#include "Admin.h"

// Include files for FTXUI
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
//#include <memory>

using namespace std;
using namespace ftxui;

// Merge Sort Functions
void merge (vector<Bank::acc>& accounts, int low, int mid, int high) { 
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

// Merge function to merge two halves of the array
void merge_recurse(vector<Bank::acc>& accounts, int low, int high) {
	if (low >= high) {
		return;
	}
	
	int mid = (low + high) / 2;
	merge_recurse(accounts, low, mid);
	merge_recurse(accounts, mid + 1, high);
	merge(accounts, low, mid, high);
	
}

// Merge Sort function to sort the array
void mergeSort(vector<Bank::acc>& accounts, int length) {
	merge_recurse(accounts, 0, length - 1);
}

// Binary Search Function
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

// Function to filter accounts based on balance range using merge sort and	binary search
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

// FTXUI: Function to create a window with a title and component
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

	auto screen = ScreenInteractive::TerminalOutput(); // FTXUI: Initialize the screen
	vector<string> MainMenuEntries = { // Main menu entries
		"1. Register",
		"2. Login",
		"3. Exit",
		"4. Admin"
	};

	int selectedMenuEntry = 0; // FTXUI: Selected menu entry variable
	auto menuOption = MenuOption(); // FTXUI: Menu options	
	menuOption.on_enter = screen.ExitLoopClosure(); // FTXUI: Exit loop on enter
	auto menu = Menu(&MainMenuEntries, &selectedMenuEntry, menuOption); // FTXUI: Main Menu	component

	// Display Main menu options
	while (true) {

		clearScreen(); // Clear the terminal screen
		printDollarSign(); // Print the static dollar sign
		cout << "\n" << WELCOME_MESSAGE << "\n" << endl; // Display welcome message
		printMU(); // Print the MU logo

		screen.Loop(Window("Mercer Bank", menu)); // FTXUI: Display the menu
		selectedMenuEntry += 1; // FTXUI: Increment the selected menu entry for switch case selection

		switch (selectedMenuEntry) {
		case REGISTER:
			clearScreen();
			registerCustomer(MercerBank);
			break;

		case LOGIN: {
			if (!loginCustomer(MercerBank)) {
				auto screen = ScreenInteractive::TerminalOutput(); // FTXUI: Initialize the screen
				string userChoice; // FTXUI: User choice variable
				bool proceedToRegister = false; // FTXUI: Proceed to register variable

				auto messageText = text("Login failed. Would you like to register (Y/N)?") | bold | center; // FTXUI: Message text

				auto yesButton = Button("Yes", [&] { // FTXUI: Yes button action
					proceedToRegister = true;
					screen.Exit();
					});

				auto noButton = Button("No", [&] { // FTXUI: No button action
					proceedToRegister = false;
					screen.Exit();
					});

				auto layout = Container::Vertical({ // FTXUI: Layout of the message and buttons
					Container::Horizontal({
					yesButton,
					noButton,
					}),
					});

				auto renderer = Renderer(layout, [&] { // FTXUI: Renderer to display the message and buttons
					return vbox({ // FTXUI: Vertical box layout of message and buttons
							messageText,
							hbox({
								yesButton->Render() | center, // FTXUI: Render yes button
								noButton->Render() | center, // FTXUI: Render no button
							}),
						}) | border;
					});

				screen.Loop(renderer); // FTXUI: Loop to display the screen until the user clicks yes or no

				// After user clicks yes or no, proceed to register or exit
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

		// Admin Login
		case ADMIN: {
			system("cls");
			auto screen = ScreenInteractive::TerminalOutput(); // FTXUI: Initialize the screen

			string username, password; // FTXUI: Username and password variables
			bool loginStatus = false; // FTXUI: Login status variable
			string error_message = ""; // FTXUI: Error message variable
			string loginMessage = "Please enter your admin credentials:"; // FTXUI: Login message variable

			auto usernameInput = Input(&username, "Username (admin): "); // FTXUI: Username input box
			auto passwordInput = Input(&password, "Password (admin): "); // FTXUI: Password input box


			auto submitButton = Button("Login", [&] { // FTXUI: Submit button action
				if (username.empty() || password.empty()) { // FTXUI: Check if username or password is empty
					error_message = "Error: Please enter both username and password.";
					return;
				}
				else if (username == "admin" && password == "admin") { // FTXUI: Check if username and password are correct
					loginStatus = true;
					screen.Exit();
				}
				else { // FTXUI: Invalid username or password
					error_message = "Login failed. Invalid username or password.";
					loginStatus = false;
					return;
				}
				}); // FTXUI: Submit button action

			// Cancel button to exit without logging in
			auto backButton = Button("Back to Main Menu", [&] { loginStatus = false; screen.Exit(); });

			// Layout of inputs and buttons
			auto layout = Container::Vertical({
				usernameInput | flex,
				passwordInput | flex,
				Container::Horizontal({
					submitButton | flex,
					backButton | flex
				}) | flex
				});

			// Renderer to display layout and inputs
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

			screen.Loop(renderer); // FTXUI: Loop to display the screen until the user clicks submit or cancel

			if (loginStatus) { // FTXUI: If login is successful
				string adminMenuChoice; // FTXUI: Admin menu choice variable

				auto adminMenuEntries = vector<string>{ // FTXUI: Admin menu entries
					"1. View All Accounts",
					"2. Search for Accounts By Balance Range",
					"3. Search for Accounts By Name",
					"4. Dummy Account",
					"5. Exit"
				};

				int selectedAdminMenuEntry = 0; // FTXUI: Selected admin menu entry variable
				auto adminMenuOption = MenuOption(); // FTXUI: Admin menu options
				adminMenuOption.on_enter = screen.ExitLoopClosure(); // FTXUI: Exit loop on enter

				auto adminMenu = Menu(&adminMenuEntries, &selectedAdminMenuEntry, adminMenuOption); // FTXUI: Admin menu component
				bool flag = true; // FTXUI: Flag to control the loop
			while (flag) {
				screen.Loop(Window("Admin Menu", adminMenu)); // FTXUI: Display the admin menu
				selectedAdminMenuEntry += 1; // FTXUI: Increment the selected admin menu entry for switch case selection
				switch (selectedAdminMenuEntry) { // FTXUI: Switch case for admin menu options
				case 1: { //FTXUI: View all accounts
					system("cls");
					showAllTables(databaseDir);
					system("pause");
					clearScreen();
					break;
				}
				case 2: { // FTXUI: Search for accounts by balance range
					system("cls");

					// Prepare the screen and inputs
					auto screen = ScreenInteractive::TerminalOutput(); // FTXUI: Initialize the screen
					string minBalanceString, maxBalanceString; // FTXUI: String variables for balance range
					double minBalance = 0.0, maxBalance = 0.0; // FTXUI: Double variables for balance range
					string error_message = ""; // FTXUI: Error message variable

					// Inputs for balance range
					auto minBalanceInput = Input(&minBalanceString, "Enter minimum balance: "); // FTXUI: Minimum balance input box
					auto maxBalanceInput = Input(&maxBalanceString, "Enter maximum balance: "); // FTXUI: Maximum balance input box

					bool submitClicked = false; // FTXUI: Flag to check if submit button is clicked

					// Submit button to confirm the balance range
					auto submitButton = Button("Submit", [&] { // FTXUI: Submit button action
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
						catch (const std::invalid_argument&) { // FTXUI: Handle invalid input
							error_message = "Invalid input. Please enter valid numbers for balance.";
						}
						catch (const std::out_of_range&) { // FTXUI: Handle out of range error
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
						if (filteredAccounts.empty()) { // FTXUI: No accounts found in the specified balance range
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
				case 3: { // FTXUI: Search for accounts by name
					system("cls");
					vector<Bank::acc> accounts = MercerBank.fetchAccountsFromDatabase(); // Fetch accounts from database
					AccountBST bst; // Create a binary search tree
					bst.buildTree(accounts); // Build the tree with the fetched accounts

					// Display all
					//bst.displayAllAccounts();

					string inputAccNumber; // FTXUI: Input variable for account number
					cout << "Enter the account number to find: "; // FTXUI: Prompt for account number
					cin >> inputAccNumber; // FTXUI: Read account number from user input
					Bank::acc* result = bst.searchAccount(inputAccNumber); // FTXUI: Search for the account in the tree
					if (result) { // FTXUI: If account is found
						cout << "Account found:\n";
						bst.printAccount(*result);
					}
					else { // FTXUI: If account is not found
						cout << "Account not found.\n";
					}
					system("pause");
					system("cls");
					break;
				}

				case 4: { // FTXUI: Dummy account creation

					string name = "dummy_account";
					string username = "username";
					string pin = "1234";
					MercerBank.registerCustomer(name, username, pin);
					Customer* customer = MercerBank.login(username, pin);
					customerMenu<double>(customer, MercerBank);
					break;
				}

				case 5: { // FTXUI: Exit admin menu
					flag = false;// FTXUI: Set flag to false to exit the loop
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