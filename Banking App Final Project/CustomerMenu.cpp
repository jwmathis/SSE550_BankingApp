#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include "CustomerMenu.h"
#include "Customer.h"
#include "SavingsAccount.h"
#include <ftxui/component/component.hpp> // For button, input, renderer, container, etc
#include <ftxui/component/screen_interactive.hpp> // For ScreenInteractive
#include <ftxui/dom/elements.hpp> // For bold, border, text, separator, etc


using namespace ftxui;

// Bank function declarations
void displayCustomerAccountsMenu(Bank& bank, Customer* customer) {
	auto screen = ScreenInteractive::TerminalOutput();

	auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
	vector<Component> accountComponents;
	int countNumOfAccounts = 0;
	for (const auto& account : displayAccounts) {
		countNumOfAccounts++;
		string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
		string accountInfo = to_string(countNumOfAccounts) +
			". Account Type: " + accountType +
			", Account Number: " + account.getAccountNum() +
			", Balance: $" + to_string(account.getBalance());

		accountComponents.push_back(Renderer([=] {
			return text(accountInfo);
			}));
	}

	auto backButton = Button("Back", [&] {
		screen.Exit();
		});

	auto layout = Container::Vertical(accountComponents);
	layout->Add(backButton);

	auto renderer = Renderer(layout, [&] {
		vector<Element> accountElements;
		for (const auto& account : accountComponents) {
			accountElements.push_back(account->Render());
		}

		return vbox({
			text("Customer Accounts") | bold | center,
			separator(),
			vbox(accountElements),
			separator(),
			backButton->Render() | center,
			}) | border;
		});

	screen.Loop(renderer);

	/*int countNumOfAccounts = 0;
	string accountType;
	cout << "Accounts:\n";
	auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
	for (const auto& account : displayAccounts) {
		countNumOfAccounts++;
		accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
		cout << countNumOfAccounts << ". Account Type: " << accountType << ", Account Number : " << account.getAccountNum() << ", Balance : " << account.getBalance() << endl;
	}*/
}

string selectCustomerAccount(Bank& bank, Customer* customer, int userSelection) {
	int countNumOfAccounts = 0;

	// Get list of customer's accounts
	auto customerAccounts = bank.getAccountsForCustomer(customer->getId());

	for (const auto& account : customerAccounts) {
		countNumOfAccounts++;

		// Check if selected account is customer selection
		if (userSelection == countNumOfAccounts) {
			return account.getAccountNum(); // Fetch the account from database
		}
	}

	return ""; // If no valid selection
}

string promptForAccountSelection(Bank& bank, Customer* customer, const string& promptMessage) {
	auto screen = ScreenInteractive::TerminalOutput();

	// Retrieve customer accounts.
	auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
	std::vector<std::string> accountOptions;
	std::string selectedAccount;
	int selectedOption = 0; // Index of the selected account in the Radiobox.

	// Create account options for the Radiobox.
	for (const auto& account : displayAccounts) {
		std::string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
		std::string accountInfo = accountType +
			" (Account Number: " + account.getAccountNum() +
			", Balance: $" + std::to_string(account.getBalance()) + ")";
		accountOptions.push_back(accountInfo);
	}

	// Radiobox for selecting an account.
	auto accountSelection = Radiobox(&accountOptions, &selectedOption);

	// Message for invalid selection.
	std::string error_message;

	// Submit button to confirm the selection.
	auto submitButton = Button("Submit", [&] {
		if (selectedOption >= 0 && selectedOption < (int)displayAccounts.size()) {
			selectedAccount = displayAccounts[selectedOption].getAccountNum();
			screen.Exit();
		}
		else {
			error_message = "Invalid selection. Please try again.";
		}
		});

	// Back button to cancel the selection.
	auto backButton = Button("Cancel", [&] {
		selectedAccount.clear();
		screen.Exit();
		});

	// Layout for the menu.
	auto layout = Container::Vertical({
		accountSelection,
		submitButton,
		backButton,
		});

	// Renderer for the interface.
	auto renderer = Renderer(layout, [&] {
		return vbox({
				   text(promptMessage) | bold | center,
				   separator(),
				   accountSelection->Render(),
				   separator(),
				   hbox({
					   submitButton->Render() | center,
					   backButton->Render() | center,
				   }),
				   error_message.empty() ? text("") : text(error_message) | color(Color::Red),
			}) |
			border;
		});

	// Display the menu.
	screen.Loop(renderer);

	return selectedAccount;


	/*string selectedAccount;
	int userSelection;

	displayCustomerAccountsMenu(bank, customer);

	do {
		cout << promptMessage;
		cin >> userSelection;

		selectedAccount = selectCustomerAccount(bank, customer, userSelection);

		if (selectedAccount.empty()) {
			cout << "Invalid selection. Please try again." << endl;
		}
	} while (selectedAccount.empty());

	return selectedAccount;*/
}

void registerCustomer(Bank& bank) {
	auto screen = ScreenInteractive::TerminalOutput();

	string name, username, pin;
	string error_message = "";

	auto nameInput = Input(&name, "Full Legal Name: ");
	auto usernameInput = Input(&username, "Username: ");
	auto pinInput = Input(&pin, "PIN: ");

	auto submitButton = Button("Register", [&] {
		if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
			error_message = "Error: PIN must be exactly 4 digits.";
			return;
		}

		if (bank.registerCustomer(name, username, pin)) {
			Customer* customer = bank.login(username, pin);
			if (customer) {
				screen.Exit();
				text("You've been registered " + customer->getName() + "! Thanks for signing up!");
				newCustomer(customer, bank); //New registered customers will go straight to Option 1 to open an account
				screen.Exit(); // Exit the registration screen
			}
		}
		else {
			error_message = "Error: Failed to register user. Username already exists.";
		}

		});

	auto exitButton = Button("Cancel", [&] { screen.Exit(); });

	auto layout = Container::Vertical({
		nameInput,
		usernameInput,
		pinInput,
		Container::Horizontal({
			submitButton,
			exitButton
			}),
		});

	auto renderer = Renderer(layout, [&] {
		return vbox({
			text("Register New Account") | bold | center,
			separator(),
			text("Enter your details below:"),
			nameInput->Render(),
			usernameInput->Render(),
			pinInput->Render(),
			hbox({
				submitButton->Render() | center,
				exitButton->Render() | center,
				}),
			error_message.empty() ? text("") : text(error_message) | color(Color::Red),
			}) | border;
		});

	screen.Loop(renderer);
	system("cls");
}

//New registered customers will go straight to Option 1 to open an account
void newCustomer(Customer* customer, Bank& bank) {
	system("cls");
	auto screen = ScreenInteractive::TerminalOutput();

	string initialBalanceString;
	double initialBalance;
	bool conversion_error = false;
	string error_message = "";

	auto initialBalanceInput = Input(&initialBalanceString, "Initial Balance: ");
	auto submitButton = Button("Submit", [&] {
		try {
			initialBalance = stod(initialBalanceString);
			if (initialBalance < 0) {
				error_message = "Invalid input. Your account cannot be created with a negative balance. "
					"Please enter a positive balance or 0 to create your account.\n";
				return;
			}
		}
		catch (const std::invalid_argument&) {
			error_message = "Invalid input. Please enter a valid number.\n";
			return;
		}
		catch (const std::out_of_range&) {
			error_message = "Invalid input. Please enter a smaller number.\n";
			return;
		}

		int accountNumber = bank.generateAccountNumber(1);
		if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance)) {
			text("Your Account has been created! Your account number is: " + to_string(accountNumber));
			system("pause");
			screen.Exit();
		}
		else {
			error_message = "Error: Failed to create account. Please try again.";
		}
	});

	auto exitButton = Button("Cancel", [&] { screen.Exit(); });

	auto layout = Container::Vertical({
		initialBalanceInput,
		Container::Horizontal({
			submitButton,
			exitButton
			}),
		});

	auto renderer = Renderer(layout, [&] {
		return vbox({
			text("Open New Account") | bold | center,
			separator(),
			text("Enter the inital balance for your new account below:"),
			initialBalanceInput->Render(),
			hbox({
				submitButton->Render() | center,
				exitButton->Render() | center,
				}),
			error_message.empty() ? text("") : text(error_message) | color(Color::Red),
			}) | border;
		});

	screen.Loop(renderer);
	system("cls");
}

bool loginCustomer(Bank& bank) {
	auto screen = ScreenInteractive::TerminalOutput();

	string username, pin;
	bool loginStatus = false;
	string error_message = "";
	Customer *customer = nullptr;

	auto usernameInput = Input(&username, "Username: ");
	auto pinInput = Input(&pin, "PIN: ");

	auto submitButton = Button("Login", [&] {
		if (username.empty() || pin.empty()) {
			error_message = "Error: Please enter both username and PIN.";
			return;
		}

		customer = bank.login(username, pin);
		if (!customer) {
			error_message = "Login failed. Invalid username or PIN.";
			loginStatus = false;
			return;
		} else {
			loginStatus = true;
			screen.Exit();
		}
	});
	auto exitButton = Button("Cancel", [&] { loginStatus = false; screen.Exit(); });

	auto layout = Container::Vertical({
		usernameInput,
		pinInput,
		Container::Horizontal({
			submitButton,
			exitButton
			}),
		});

	auto renderer = Renderer(layout, [&] {
		return vbox({
			text("Customer Login") | bold | center,
			separator(),
			text("Enter your username and PIN below:"),
			usernameInput->Render(),
			pinInput->Render(),
			hbox({
				submitButton->Render() | center,
				exitButton->Render() | center,
			}),
			error_message.empty() ? text("") : text(error_message) | color(Color::Red),
		}) 
		| border;
	});

	screen.Loop(renderer);
	system("cls");

	if (loginStatus) {
		customerMenu(customer, bank);
	}

	return loginStatus; // Login successful
}

void customerMenu(Customer* customer, Bank& bank) {
	auto screen = ScreenInteractive::TerminalOutput();

	vector<string> menu_entries = {
		"1. Open Regular Account",
		"2. Open Savings Account",
		"3. View Accounts",
		"4. Deposit",
		"5. Withdraw",
		"6. Transfer Funds",
		"7. Close Account",
	};

	bool whileFlag = true;
	bool isLoggedOut = false; // Flag for logout
	int selected = 0;
	string content = "Select an option";
	string welcome_customer = "Welcome " + customer->getName() + "!";

	auto menuOption = MenuOption();
	menuOption.on_enter = screen.ExitLoopClosure();
	auto menu = Menu(&menu_entries, &selected, menuOption);

	auto menuRender = Renderer(menu, [&] {
		return window(text("Customer Menu"), menu->Render()) | flex;
		});
	auto logoutButton = Button("Logout", [&] {
		isLoggedOut = true;
		screen.Exit();
		});
	auto layout = Container::Vertical({
		menuRender,
		Container::Horizontal({
			logoutButton
		}),
		});

	// Main renderer
	auto renderer = Renderer(layout, [&] {
		return vbox({
			text(welcome_customer) | bold | center,
			separator(),
			layout->Render(),
			separator(),
			}) |
			border;
		});

	while (whileFlag) {
		system("cls");
		screen.Loop(renderer);

		if (isLoggedOut) {
			system("cls");
			auto logoutScreen = ScreenInteractive::TerminalOutput();

			// Generate the transaction receipt filename for the customer
			std::string filename = customer->generateTransactionReceiptFilename();

			// Attempt to remove the file
			bool fileRemoved = (remove(filename.c_str()) == 0);

			// Success or error message for file removal
			std::string logoutMessage;
			if (fileRemoved) {
				logoutMessage = "You've been logged out successfully.";
			}
			else {
				logoutMessage = "Error: Could not remove the transaction receipt file.";
			}

			auto layout = Container::Vertical({
				Renderer([=] { return text(logoutMessage) | center; }),
				Button("Back", [&] {
					logoutScreen.Exit(); // Exit logout screen
				}),
				});

			auto renderer = Renderer(layout, [&] {
				return vbox({
					text("Logout") | bold | center,
					separator(),
					text(logoutMessage) | color(Color::Green),
					separator(),
					}) | border;
				});

			logoutScreen.Loop(renderer);
			system("pause");
			whileFlag = false; // Break out of the loop after logout
		}

		selected += 1;
		switch (selected) {
			case REG_ACCOUNT: {
				system("cls");
				newCustomer(customer, bank);
				break;
			}
			case SAVINGS_ACC: {
				system("cls");
				newCustomer(customer, bank);
				break;
			}
			case BALANCE_INQUIRY: {
				system("cls");
				displayCustomerAccountsMenu(bank, customer);
				break;
			}
			case DEPOSIT_AMOUNT: {
				system("cls");
				auto screen = ScreenInteractive::TerminalOutput();

				// Retrieve accounts for the customer.
				auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
				std::vector<std::string> accountOptions;
				int selectedOption = 0; // Index of the selected account.
				std::string error_message;
				std::string success_message;
				std::string depositAmountString;
				double depositAmount = 0.0;

				// Populate account options.
				for (const auto& account : displayAccounts) {
					std::string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
					std::string accountInfo = accountType +
						" (Account Number: " + account.getAccountNum() +
						", Balance: $" + std::to_string(account.getBalance()) + ")";
					accountOptions.push_back(accountInfo);
				}

				// Radiobox for selecting an account.
				auto accountSelection = Radiobox(&accountOptions, &selectedOption);

				// Input field for the deposit amount.
				auto depositInput = Input(&depositAmountString, "Enter amount to deposit:");

				// Submit button to perform the deposit.
				auto submitButton = Button("Submit", [&] {
					if (selectedOption < 0 || selectedOption >= (int)displayAccounts.size()) {
						error_message = "Error: Invalid account selection.";
						return;
					}

					try {
						depositAmount = std::stod(depositAmountString);
						if (depositAmount <= 0) {
							error_message = "Error: Deposit amount must be greater than 0.";
							return;
						}
					}
					catch (const std::exception&) {
						error_message = "Error: Please enter a valid amount.";
						return;
					}

					Account* account = bank.getAccountByNumber(displayAccounts[selectedOption].getAccountNum());
					if (account) {
						account->deposit(depositAmount);
						if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
							success_message = "Deposit successful! New balance: $" + std::to_string(account->getBalance());
							customer->generateTransactionReceipt(success_message);
							screen.Exit();
						}
						else {
							error_message = "Error: Failed to update account balance.";
						}
					}
					else {
						error_message = "Error: Account not found.";
					}
					});

				// Back button to cancel the deposit.
				auto backButton = Button("Cancel", [&] {
					screen.Exit();
					});

				// Layout for the menu.
				auto layout = Container::Vertical({
					accountSelection,
					depositInput,
					submitButton,
					backButton,
					});

				// Renderer for the interface.
				auto renderer = Renderer(layout, [&] {
					return vbox({
							   text("Deposit Amount") | bold | center,
							   separator(),
							   text("Select an account:"),
							   accountSelection->Render(),
							   separator(),
							   depositInput->Render(),
							   separator(),
							   hbox({
								   submitButton->Render() | center,
								   backButton->Render() | center,
							   }),
							   success_message.empty() ? text("") : text(success_message) | color(Color::Green),
							   error_message.empty() ? text("") : text(error_message) | color(Color::Red),
						}) |
						border;
					});

				// Run the screen loop.
				screen.Loop(renderer);
				system("pause");
				break;
			}

			case WITHDRAW_AMOUNT: {
				system("cls");
				auto screen = ScreenInteractive::TerminalOutput();

				// Retrieve accounts for the customer.
				auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
				std::vector<std::string> accountOptions;
				int selectedOption = 0; // Index of the selected account.
				std::string error_message;
				std::string success_message;
				std::string withdrawAmountString;
				double withdrawAmount = 0.0;

				// Populate account options.
				for (const auto& account : displayAccounts) {
					std::string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
					std::string accountInfo = accountType +
						" (Account Number: " + account.getAccountNum() +
						", Balance: $" + std::to_string(account.getBalance()) + ")";
					accountOptions.push_back(accountInfo);
				}

				// Radiobox for selecting an account.
				auto accountSelection = Radiobox(&accountOptions, &selectedOption);

				// Input field for the deposit amount.
				auto withdrawInput = Input(&withdrawAmountString, "Enter amount to withdraw:");

				// Submit button to perform the deposit.
				auto submitButton = Button("Submit", [&] {
					if (selectedOption < 0 || selectedOption >= (int)displayAccounts.size()) {
						error_message = "Error: Invalid account selection.";
						return;
					}

					try {
						withdrawAmount = std::stod(withdrawAmountString);
						if (withdrawAmount <= 0) {
							error_message = "Error: Deposit amount must be greater than 0.";
							return;
						}
					}
					catch (const std::exception&) {
						error_message = "Error: Please enter a valid amount.";
						return;
					}

					Account* account = bank.getAccountByNumber(displayAccounts[selectedOption].getAccountNum());
					if (account) {
						account->withdraw(withdrawAmount);
						if (bank.updateAccountBalance(account->getId(), account->getBalance())) {
							success_message = "Withdrawal successful! New balance: $" + std::to_string(account->getBalance());
							customer->generateTransactionReceipt(success_message);
							screen.Exit();
						}
						else {
							error_message = "Error: Failed to update account balance.";
						}
					}
					else {
						error_message = "Error: Account not found.";
					}
					});

				// Back button to cancel the deposit.
				auto backButton = Button("Cancel", [&] {
					screen.Exit();
					});

				// Layout for the menu.
				auto layout = Container::Vertical({
					accountSelection,
					withdrawInput,
					submitButton,
					backButton,
					});

				// Renderer for the interface.
				auto renderer = Renderer(layout, [&] {
					return vbox({
							   text("Deposit Amount") | bold | center,
							   separator(),
							   text("Select an account:"),
							   accountSelection->Render(),
							   separator(),
							   withdrawInput->Render(),
							   separator(),
							   hbox({
								   submitButton->Render() | center,
								   backButton->Render() | center,
							   }),
							   success_message.empty() ? text("") : text(success_message) | color(Color::Green),
							   error_message.empty() ? text("") : text(error_message) | color(Color::Red),
						}) |
						border;
					});

				// Run the screen loop.
				screen.Loop(renderer);
				system("pause");
				break;
			}
			case TRANSFER_AMOUNT: {
				system("cls");
				auto screen = ScreenInteractive::TerminalOutput();

				// Retrieve accounts for the customer.
				auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
				std::vector<std::string> accountOptions;
				int senderOption = 0, receiverOption = 0; // Indexes for sender and receiver accounts.
				std::string error_message;
				std::string success_message;
				std::string transferAmountString;
				double transferAmount = 0.0;

				// Populate account options.
				for (const auto& account : displayAccounts) {
					std::string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
					std::string accountInfo = accountType +
						" (Account Number: " + account.getAccountNum() +
						", Balance: $" + std::to_string(account.getBalance()) + ")";
					accountOptions.push_back(accountInfo);
				}

				// Radiobox for selecting sender account.
				auto senderAccountSelection = Radiobox(&accountOptions, &senderOption);

				// Radiobox for selecting receiver account.
				auto receiverAccountSelection = Radiobox(&accountOptions, &receiverOption);

				// Input field for the transfer amount.
				auto amountInput = Input(&transferAmountString, "Enter amount to transfer:");

				// Submit button to perform the transfer.
				auto submitButton = Button("Submit", [&] {
					if (senderOption < 0 || senderOption >= (int)displayAccounts.size() ||
						receiverOption < 0 || receiverOption >= (int)displayAccounts.size()) {
						error_message = "Error: Invalid account selection.";
						return;
					}

					if (senderOption == receiverOption) {
						error_message = "Error: Cannot transfer to the same account.";
						return;
					}

					try {
						transferAmount = std::stod(transferAmountString);
						if (transferAmount <= 0) {
							error_message = "Error: Transfer amount must be greater than 0.";
							return;
						}
					}
					catch (const std::exception&) {
						error_message = "Error: Please enter a valid amount.";
						return;
					}

					Account* senderAccount = bank.getAccountByNumber(displayAccounts[senderOption].getAccountNum());
					Account* receiverAccount = bank.getAccountByNumber(displayAccounts[receiverOption].getAccountNum());

					if (!senderAccount || !receiverAccount) {
						error_message = "Error: Invalid account numbers.";
						return;
					}

					if (senderAccount->getBalance() >= transferAmount) {
						senderAccount->withdraw(transferAmount);
						receiverAccount->deposit(transferAmount);

						if (bank.updateAccountBalance(senderAccount->getId(), senderAccount->getBalance()) &&
							bank.updateAccountBalance(receiverAccount->getId(), receiverAccount->getBalance())) {
							success_message = "Transfer successful! New balance of sender account: $" +
								std::to_string(senderAccount->getBalance());
							customer->generateTransactionReceipt(success_message);
							screen.Exit();
						}
						else {
							error_message = "Error: Failed to update account balances.";
						}
					}
					else {
						error_message = "Error: Insufficient funds in sender's account.";
					}
					});

				// Back button to cancel the transfer.
				auto backButton = Button("Cancel", [&] {
					screen.Exit();
					});

				// Layout for the menu.
				auto layout = Container::Vertical({
					senderAccountSelection,
					receiverAccountSelection,
					amountInput,
					Container::Horizontal({
					submitButton,
					backButton,
					})
					});

				// Renderer for the interface.
				auto renderer = Renderer(layout, [&] {
					return vbox({
							   text("Transfer Amount") | bold | center,
							   separator(),
							   text("Select the account to transfer from:"),
							   senderAccountSelection->Render(),
							   separator(),
							   text("Select the account to transfer to:"),
							   receiverAccountSelection->Render(),
							   separator(),
							   amountInput->Render(),
							   separator(),
							   hbox({
								   submitButton->Render() | center,
								   backButton->Render() | center,
							   }),
							   success_message.empty() ? text("") : text(success_message) | color(Color::Green),
							   error_message.empty() ? text("") : text(error_message) | color(Color::Red),
						}) |
						border;
					});

				// Run the screen loop.
				screen.Loop(renderer);
				system("pause");
				break;
			}

			case CLOSE_ACCOUNT: {
				system("cls");
				auto screen = ScreenInteractive::TerminalOutput();

				// Retrieve the accounts for the customer.
				auto displayAccounts = bank.getAccountsForCustomer(customer->getId());
				std::vector<std::string> accountOptions;
				int senderOption = 0, receiverOption = 0; // Indexes for sender and receiver accounts.
				std::string error_message;
				std::string success_message;
				std::string transferAmountString;
				double transferAmount = 0.0;

				// Populate account options for selection.
				for (const auto& account : displayAccounts) {
					std::string accountType = (account.getAccountNum()[0] == '2') ? "Savings" : "Regular";
					std::string accountInfo = accountType +
						" (Account Number: " + account.getAccountNum() +
						", Balance: $" + std::to_string(account.getBalance()) + ")";
					accountOptions.push_back(accountInfo);
				}

				// Radiobox for selecting the account to close (sender account).
				auto senderAccountSelection = Radiobox(&accountOptions, &senderOption);

				// Radiobox for selecting the account to transfer money to (receiver account).
				auto receiverAccountSelection = Radiobox(&accountOptions, &receiverOption);

				// Submit button to handle the closing process.
				auto submitButton = Button("Close Account", [&] {
					if (senderOption < 0 || senderOption >= (int)displayAccounts.size() ||
						receiverOption < 0 || receiverOption >= (int)displayAccounts.size()) {
						error_message = "Error: Invalid account selection.";
						return;
					}

					if (senderOption == receiverOption) {
						error_message = "Error: Cannot transfer to the same account.";
						return;
					}

					Account* senderAccount = bank.getAccountByNumber(displayAccounts[senderOption].getAccountNum());
					Account* receiverAccount = bank.getAccountByNumber(displayAccounts[receiverOption].getAccountNum());

					if (!senderAccount || !receiverAccount) {
						error_message = "Error: Invalid account numbers.";
						return;
					}

					// Get the sender account balance.
					transferAmount = senderAccount->getBalance();

					Transfer transfer(*senderAccount, *receiverAccount);
					transfer.setAmount(transferAmount);
					senderAccount->withdraw(transferAmount);
					receiverAccount->deposit(transferAmount);

					// Update account balances.
					if (bank.updateAccountBalance(senderAccount->getId(), senderAccount->getBalance()) &&
						bank.updateAccountBalance(receiverAccount->getId(), receiverAccount->getBalance())) {
						success_message = "Account closed successfully! Transferred $" + std::to_string(transferAmount) +
							" to the selected account. New balance of the receiver account: $" + std::to_string(receiverAccount->getBalance());
						customer->generateTransactionReceipt(success_message);
						bank.accountRemoveAccount(senderAccount->getAccountNum());
						screen.Exit();
					}
					else {
						error_message = "Error: Failed to update account balances.";
					}
					});

				// Back button to cancel the operation.
				auto backButton = Button("Cancel", [&] {
					screen.Exit();
					});

				// Layout for the menu.
				auto layout = Container::Vertical({
					senderAccountSelection,
					receiverAccountSelection,
					submitButton,
					backButton,
					});

				// Renderer for the interface.
				auto renderer = Renderer(layout, [&] {
					return vbox({
							   text("Close Account") | bold | center,
							   separator(),
							   text("Select the account you wish to close:"),
							   senderAccountSelection->Render(),
							   separator(),
							   text("Select the account to transfer money to:"),
							   receiverAccountSelection->Render(),
							   separator(),
							   hbox({
								   submitButton->Render() | center,
								   backButton->Render() | center,
							   }),
							   success_message.empty() ? text("") : text(success_message) | color(Color::Green),
							   error_message.empty() ? text("") : text(error_message) | color(Color::Red),
						}) |
						border;
					});

				// Run the screen loop.
				screen.Loop(renderer);
				system("pause");
				break;
			}

			default: {
				system("cls");
				auto screen = ScreenInteractive::TerminalOutput();

				// Generate the transaction receipt filename for the customer
				std::string filename = customer->generateTransactionReceiptFilename();

				// Attempt to remove the file
				bool fileRemoved = (remove(filename.c_str()) == 0);

				// Success or error message for file removal
				std::string logoutMessage;
				if (fileRemoved) {
					logoutMessage = "You've been logged out successfully.";
				}
				else {
					logoutMessage = "Error: Could not remove the transaction receipt file.";
				}

				// Back button to exit or return to the previous screen
				auto backButton = Button("Back", [&] {
					screen.Exit();
					});

				// Layout for the logout screen
				auto layout = Container::Vertical({
					Renderer([=] { return text(logoutMessage) | center; }),
						backButton,
					});

				// Renderer for the interface
				auto renderer = Renderer(layout, [&] {
					return vbox({
							   text("Logout") | bold | center,
							   separator(),
							   text(logoutMessage) | color(Color::Green),
							   separator(),
							   backButton->Render() | center,
						}) |
						border;
					});

				// Run the screen loop
				screen.Loop(renderer);
				system("pause");
				break;
			}
			
		}
	}
}