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
template <typename T>
void displayCustomerAccountsMenu(Bank& bank, Customer* customer) {
	auto screen = ScreenInteractive::TerminalOutput();

	auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId());
	vector<Component> accountComponents;
	int countNumOfAccounts = 0;
	for (const auto& account : displayAccounts) {
		countNumOfAccounts++;
		string accountType;

		if (dynamic_cast<SavingsAccount<T>*>(account)) {
			accountType = "Savings";
		}
		else {
			accountType = "Regular";
		}

		string accountInfo = to_string(countNumOfAccounts) +
			". Account Type: " + accountType +
			", Account Number: " + account->getAccountNum() +
			", Balance: $" + to_string(account->getBalance());

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
	vector<string> accountType = { "Regular", "Savings" };
	int selectedAccountType = 0;

	auto initialBalanceInput = Input(&initialBalanceString, "Initial Balance: ");
	auto accountTypeRadiobox = Radiobox(&accountType, &selectedAccountType);

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
		string selectedType = accountType[selectedAccountType];
		int accountNumber = bank.generateAccountNumber(1);
		if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance, selectedType)) {
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
		accountTypeRadiobox,
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
			accountTypeRadiobox->Render(),
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
	Customer* customer = nullptr;

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
		}
		else {
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
		customerMenu<double>(customer, bank);
	}

	return loginStatus; // Login successful
}

template <typename T>
void customerMenu(Customer* customer, Bank& bank) {
	auto screen = ScreenInteractive::TerminalOutput();

	vector<string> menu_entries = {
		"1. Open New Account",
		"2. View Accounts",
		"3. Deposit",
		"4. Withdraw",
		"5. Transfer Funds",
		"6. Close Account",
		"7. Display Transaction History",
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
		case NEW_ACCOUNT: {
			system("cls");
			newCustomer(customer, bank);
			break;
		}
		case BALANCE_INQUIRY: {
			system("cls");
			displayCustomerAccountsMenu<T>(bank, customer);
			break;
		}
		case DEPOSIT_AMOUNT: {
			system("cls");
			auto screen = ScreenInteractive::TerminalOutput();

			// Retrieve accounts for the customer.
			auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId());
			std::vector<std::string> accountOptions;
			int selectedOption = 0; // Index of the selected account.
			std::string error_message;
			std::string success_message;
			std::string depositAmountString;
			double depositAmount = 0.0;

			// Populate account options.
			for (const auto& account : displayAccounts) {
				string accountType;

				if (dynamic_cast<SavingsAccount<T>*>(account)) {
					accountType = "Savings";
				}
				else {
					accountType = "Regular";
				}

				std::string accountInfo = accountType +
					" (Account Number: " + account->getAccountNum() +
					", Balance: $" + std::to_string(account->getBalance()) + ")";
				accountOptions.push_back(accountInfo);
			}

			// Radiobox for selecting an account.
			auto accountSelection = Radiobox(&accountOptions, &selectedOption);

			// Input field for the deposit amount.
			auto depositInput = Input(&depositAmountString, "Enter amount to deposit:");

			// Submit button to perform the deposit.
			auto submitButton = Button("Submit", [&] {

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
				string accountNum;
				Account<T>* accountSelected = displayAccounts[selectedOption];
				if (accountSelected) {
					accountNum = accountSelected->getAccountNum();
				}
				else {
					error_message = "Error: Account not found.";
				}

				if (accountSelected) {
					accountSelected->deposit(depositAmount);
					if (bank.updateAccountBalance(accountSelected->getId(), accountSelected->getBalance())) {
						success_message = "Deposit successful! New balance: $" + std::to_string(accountSelected->getBalance());
						customer->addTransaction("Deposit", depositAmount, "9999-99-99");
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
			auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId());
			std::vector<std::string> accountOptions;
			int selectedOption = 0; // Index of the selected account.
			std::string error_message;
			std::string success_message;
			std::string withdrawAmountString;
			double withdrawAmount = 0.0;

			// Populate account options.
			for (const auto& account : displayAccounts) {
				string accountType;

				if (dynamic_cast<SavingsAccount<T>*>(account)) {
					accountType = "Savings";
				}
				else {
					accountType = "Regular";
				}
				std::string accountInfo = accountType +
					" (Account Number: " + account->getAccountNum() +
					", Balance: $" + std::to_string(account->getBalance()) + ")";
				accountOptions.push_back(accountInfo);
			}

			// Radiobox for selecting an account.
			auto accountSelection = Radiobox(&accountOptions, &selectedOption);

			// Input field for the deposit amount.
			auto withdrawInput = Input(&withdrawAmountString, "Enter amount to withdraw:");

			// Submit button to perform the deposit.
			auto submitButton = Button("Submit", [&] {
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

				string accountNum;
				Account<T>* accountSelected = displayAccounts[selectedOption];
				if (accountSelected) {
					accountSelected->withdraw(withdrawAmount);
					if (bank.updateAccountBalance(accountSelected->getId(), accountSelected->getBalance())) {
						success_message = "Withdrawal successful! New balance: $" + std::to_string(accountSelected->getBalance());
						customer->addTransaction("Withdrawal", withdrawAmount, "9999-99-99");
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
			auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId());
			std::vector<std::string> accountOptions;
			int senderOption = 0, receiverOption = 0; // Indexes for sender and receiver accounts.
			std::string error_message;
			std::string success_message;
			std::string transferAmountString;
			double transferAmount = 0.0;

			// Populate account options.
			for (const auto& account : displayAccounts) {
				string accountType;

				if (dynamic_cast<SavingsAccount<T>*>(account)) {
					accountType = "Savings";
				}
				else {
					accountType = "Regular";
				}
				std::string accountInfo = accountType +
					" (Account Number: " + account->getAccountNum() +
					", Balance: $" + std::to_string(account->getBalance()) + ")";
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
				string senderAccountNum, receiverAccountNum;
				Account<T>* senderAccount = displayAccounts[senderOption];
				Account<T>* receiverAccount = displayAccounts[receiverOption];

				if (!senderAccount || !receiverAccount) {
					error_message = "Error: Invalid account numbers.";
					return;
				}

				if (senderAccount->getBalance() >= transferAmount) {
					senderAccount->withdraw(transferAmount);
					receiverAccount->deposit(transferAmount);
					customer->addTransaction("Transfer", transferAmount, "9999-99-99");

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
			auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId());
			std::vector<std::string> accountOptions;
			int senderOption = 0, receiverOption = 0; // Indexes for sender and receiver accounts.
			std::string error_message;
			std::string success_message;
			std::string transferAmountString;
			double transferAmount = 0.0;

			// Populate account options for selection.
			for (const auto& account : displayAccounts) {
				string accountType;

				if (dynamic_cast<SavingsAccount<T>*>(account)) {
					accountType = "Savings";
				}
				else {
					accountType = "Regular";
				}
				std::string accountInfo = accountType +
					" (Account Number: " + account->getAccountNum() +
					", Balance: $" + std::to_string(account->getBalance()) + ")";
				accountOptions.push_back(accountInfo);
			}

			// Radiobox for selecting the account to close (sender account).
			auto senderAccountSelection = Radiobox(&accountOptions, &senderOption);

			// Radiobox for selecting the account to transfer money to (receiver account).
			auto receiverAccountSelection = Radiobox(&accountOptions, &receiverOption);

			// Submit button to handle the closing process.
			auto submitButton = Button("Close Account", [&] {
				if (senderOption == receiverOption) {
					error_message = "Error: Cannot transfer to the same account.";
					return;
				}

				string senderAccountNum, receiverAccountNum;
				Account<T>* senderAccount = displayAccounts[senderOption];
				Account<T>* receiverAccount = displayAccounts[receiverOption];

				if (!senderAccount || !receiverAccount) {
					error_message = "Error: Invalid account numbers.";
					return;
				}

				// Get the sender account balance.
				transferAmount = senderAccount->getBalance();

				Transfer<T>  transfer(senderAccount, receiverAccount);
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
		case HISTORY: {
			customer->displayTransactionHistory();
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
