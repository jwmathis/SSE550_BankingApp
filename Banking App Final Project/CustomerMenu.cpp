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

// Function to display customer accounts menu
template <typename T> 
void displayCustomerAccountsMenu(Bank& bank, Customer* customer) {
	auto screen = ScreenInteractive::TerminalOutput(); // FTXUI: Initialize the screen

	auto displayAccounts = bank.getAccountsForCustomer<T>(customer->getId()); // Retrieve accounts for the customer
	vector<Component> accountComponents; // FTXUI: Vector to hold account components
	int countNumOfAccounts = 0; // FTXUI: Counter for number of accounts
	for (const auto& account : displayAccounts) { // Loop through each account
		countNumOfAccounts++;
		string accountType;

		if (dynamic_cast<SavingsAccount<T>*>(account)) { // Check if the account is a SavingsAccount
			accountType = "Savings";
		}
		else { // Otherwise, it's a Regular account
			accountType = "Regular";
		}

		// Create a text string with account information
		string accountInfo = to_string(countNumOfAccounts) +
			". Account Type: " + accountType +
			", Account Number: " + account->getAccountNum() +
			", Balance: $" + to_string(account->getBalance());

		// Create a renderer for the account information
		accountComponents.push_back(Renderer([=] {
			return text(accountInfo);
			}));
	}

	// Create a button to go back to the main menu
	auto backButton = Button("Back", [&] {
		screen.Exit();
		});

	// Create a renderer for the back button
	auto layout = Container::Vertical(accountComponents);
	layout->Add(backButton);

	// Create a renderer for the entire menu
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

	screen.Loop(renderer); // FTXUI: Loop to display the screen until the user clicks back
}

// Function to register a new customer
void registerCustomer(Bank& bank) {
  // FTXUI: Initialize the screen
  auto screen = ScreenInteractive::TerminalOutput();

  // Variables to store customer details
  string name, username, pin;

  // FTXUI: Error message variable
  string error_message = "";

  // FTXUI: Input for full legal name
  auto nameInput = Input(&name, "Full Legal Name: ");

  // FTXUI: Input for username
  auto usernameInput = Input(&username, "Username: ");

  // FTXUI: Input for PIN
  auto pinInput = Input(&pin, "PIN: ");

  // FTXUI: Submit button action
  auto submitButton = Button("Register", [&] {
    // Check if PIN is exactly 4 digits
    if (pin.length() != 4 || !all_of(pin.begin(), pin.end(), ::isdigit)) {
      // Display error message if PIN is invalid
      error_message = "Error: PIN must be exactly 4 digits.";
      return;
    }

    // Attempt to register the customer with the provided details
    if (bank.registerCustomer(name, username, pin)) {
      // If registration is successful, log the customer in and create a new account
      Customer* customer = bank.login(username, pin);
      if (customer) {
        // Exit the registration screen
        screen.Exit();
        // Display success message
        text("You've been registered " + customer->getName() + "! Thanks for signing up!");
        // Create a new account for the registered customer
        newCustomer(customer, bank);
        // Exit the registration screen again (not sure why this is needed)
        screen.Exit();
      }
    } else {
      // Display error message if registration fails
      error_message = "Error: Failed to register user. Username already exists.";
    }
  });

  // FTXUI: Cancel button action
  auto exitButton = Button("Cancel", [&] { screen.Exit(); });

  // FTXUI: Layout for the registration form
  auto layout = Container::Vertical({
    nameInput,
    usernameInput,
    pinInput,
    Container::Horizontal({
      submitButton,
      exitButton
    }),
  });

  // FTXUI: Renderer for the registration form
  auto renderer = Renderer(layout, [&] {
    return vbox({
      // Display registration form title
      text("Register New Account") | bold | center,
      // Display separator
      separator(),
      // Display input prompt
      text("Enter your details below:"),
      // Display input fields
      nameInput->Render(),
      usernameInput->Render(),
      pinInput->Render(),
      // Display submit and cancel buttons
      hbox({
        submitButton->Render() | center,
        exitButton->Render() | center,
      }),
      // Display error message if any
      error_message.empty() ? text("") : text(error_message) | color(Color::Red),
    }) | border;
  });

  // FTXUI: Loop to display the screen until the user clicks submit or cancel
  screen.Loop(renderer);
  // Clear the console
  system("cls");
}

//	Function to create a new customer account
/**
 * Creates a new customer account.
 *
 * @param customer The customer object to create an account for.
 * @param bank The bank object to interact with.
 */
void newCustomer(Customer* customer, Bank& bank) {
  // Clear the console
  system("cls");

  // Initialize the screen for user interaction
  auto screen = ScreenInteractive::TerminalOutput();

  // Variables for user input
  string initialBalanceString; // Input variable for initial balance
  double initialBalance; // Double variable for initial balance
  bool conversion_error = false; // Flag for conversion error
  string error_message = ""; // Error message variable

  // Account type options
  vector<string> accountType = { "Regular", "Savings" };
  int selectedAccountType = 0; // Default account type selection

  // Create input field for initial balance
  auto initialBalanceInput = Input(&initialBalanceString, "Initial Balance: ");

  // Create radio box for account type selection
  auto accountTypeRadiobox = Radiobox(&accountType, &selectedAccountType);

  // Create submit button to create the account
  auto submitButton = Button("Submit", [&] {
    try {
      // Attempt to convert initial balance string to double
      initialBalance = stod(initialBalanceString);

      // Check if initial balance is negative
      if (initialBalance < 0) {
        // Display error message
        error_message = "Invalid input. Your account cannot be created with a negative balance. "
          "Please enter a positive balance or 0 to create your account.\n";
        return;
      }
    } catch (const std::invalid_argument&) {
      // Display error message if input is not a valid number
      error_message = "Invalid input. Please enter a valid number.\n";
      return;
    } catch (const std::out_of_range&) {
      // Display error message if input is too large
      error_message = "Invalid input. Please enter a smaller number.\n";
      return;
    }

    // Get selected account type
    string selectedType = accountType[selectedAccountType];

    // Generate a new account number
    int accountNumber = bank.generateAccountNumber(1);

    // Attempt to add the account to the customer
    if (bank.addAccountForCustomer(customer->getId(), to_string(accountNumber), initialBalance, selectedType)) {
      // Display success message and account number
      text("Your Account has been created! Your account number is: " + to_string(accountNumber));
      system("pause");
      screen.Exit();
    } else {
      // Display error message if account creation fails
      error_message = "Error: Failed to create account. Please try again.";
    }
  });

  // Create cancel button to exit the screen
  auto exitButton = Button("Cancel", [&] { screen.Exit(); });

  // Create layout for the screen
  auto layout = Container::Vertical({
    initialBalanceInput,
    accountTypeRadiobox,
    Container::Horizontal({
      submitButton,
      exitButton
    }),
  });

  // Create renderer for the screen
  auto renderer = Renderer(layout, [&] {
    return vbox({
      text("Open New Account") | bold | center,
      separator(),
      text("Enter the initial balance for your new account below:"),
      initialBalanceInput->Render(),
      accountTypeRadiobox->Render(),
      hbox({
        submitButton->Render() | center,
        exitButton->Render() | center,
      }),
      error_message.empty() ? text("") : text(error_message) | color(Color::Red),
    }) | border;
  });

  // Loop the screen until the user exits
  screen.Loop(renderer);

  // Clear the console
  system("cls");
}

/**
 * Attempts to log in a customer to the banking system.
 *
 * @param bank The bank object to interact with.
 * @return True if the login is successful, false otherwise.
 */
bool loginCustomer(Bank& bank) {
  // Initialize the screen for user interaction
  auto screen = ScreenInteractive::TerminalOutput();

  // Variables for user input
  string username, pin;
  bool loginStatus = false; // Flag to track login status
  string error_message = ""; // Error message variable
  Customer* customer = nullptr; // Customer object to store logged-in customer

  // Create input fields for username and PIN
  auto usernameInput = Input(&username, "Username: ");
  auto pinInput = Input(&pin, "PIN: ");

  // Create submit button to attempt login
  auto submitButton = Button("Login", [&] {
    // Check if both username and PIN are entered
    if (username.empty() || pin.empty()) {
      // Display error message
      error_message = "Error: Please enter both username and PIN.";
      return;
    }

    // Attempt to log in the customer
    customer = bank.login(username, pin);
    if (!customer) {
      // Display error message if login fails
      error_message = "Login failed. Invalid username or PIN.";
      loginStatus = false;
      return;
    } else {
      // Set login status to true and exit the screen
      loginStatus = true;
      screen.Exit();
    }
  });

  // Create cancel button to exit the screen
  auto exitButton = Button("Cancel", [&] { loginStatus = false; screen.Exit(); });

  // Create layout for the screen
  auto layout = Container::Vertical({
    usernameInput,
    pinInput,
    Container::Horizontal({
      submitButton,
      exitButton
    }),
  });

  // Create renderer for the screen
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

  // Loop the screen until the user exits
  screen.Loop(renderer);

  // Clear the console
  system("cls");

  // If login is successful, display the customer menu
  if (loginStatus) {
    customerMenu<double>(customer, bank);
  }

  // Return the login status
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
		"8. Undo Last Transaction",
		"9. Submit Help Request",
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

		case 8: {
			// Submit request to undo transaction
			system("cls");
			auto screen = ScreenInteractive::TerminalOutput();
			string confirmationMessage = "Would you like to undo the last transaction?";
			string statusMessage = "";
			bool confirmed = false;

			auto confirmButton = Button("Submit", [&] {
				customer->undoTransaction();
				statusMessage = "Your request has been sent. It will be processed in the next few days.";
				confirmed = true;
				});

			auto cancelButton = Button("Cancel", [&] { screen.Exit(); });
			auto layout = Container::Vertical({
				confirmButton,
				cancelButton,
				});

			auto renderer = Renderer(layout, [&] {
				return vbox({
					text(confirmationMessage) | bold | center,
					separator(),
					hbox({
						confirmButton->Render() | center,
						cancelButton->Render() | center,
						}),
					separator(),
					statusMessage.empty() ? text("") : text(statusMessage) | color(Color::Green),
					});
				});
			screen.Loop(renderer);
			break;
		}

		case 9: {
			system("cls");
			auto screen = ScreenInteractive::TerminalOutput();
			std::string helpRequest = "";
			std::string statusMessage = "";
			int selectedIndex = 0; // Index for selecting requests to delete

			// Fetch help requests from the customer
			auto fetchHelpRequests = [&]() -> std::vector<std::string> {
				std::vector<std::string> requests;
				std::queue<std::string> tempQueue = customer->getHelpRequests();
				while (!tempQueue.empty()) {
					requests.push_back(tempQueue.front());
					tempQueue.pop();
				}
				return requests;
				};

			auto helpRequests = fetchHelpRequests();

			// Input for submitting new help requests
			auto submitRequestInput = Input(&helpRequest, "Enter your help request: ");

			// Buttons
			auto submitRequestButton = Button("Submit Request", [&] {
				if (!helpRequest.empty()) {
					customer->addHelpRequest(helpRequest);
					statusMessage = "Your request has been submitted.";
					helpRequest = ""; // Clear input
					helpRequests = fetchHelpRequests(); // Refresh the request list
				}
				else {
					statusMessage = "Error: Request cannot be empty.";
				}
				});

			auto deleteRequestButton = Button("Delete Selected Request", [&] {
				if (!helpRequests.empty() && selectedIndex >= 0 && selectedIndex < helpRequests.size()) {
					customer->removeHelpRequest();
					statusMessage = "Request deleted successfully.";
					helpRequests = fetchHelpRequests(); // Refresh the request list
					selectedIndex = 0; // Reset selection
				}
				else {
					statusMessage = "Error: No request selected or invalid selection.";
				}
				});

			auto exitButton = Button("Exit", [&] { screen.Exit(); });

			// Layout and rendering
			auto layout = Container::Vertical({
				submitRequestInput,
				submitRequestButton,

				Renderer([&] {
					std::vector<Element> requestElements;
					for (size_t i = 0; i < helpRequests.size(); ++i) {
						bool isSelected = (i == selectedIndex);
						auto style = isSelected ? bgcolor(Color::Blue) | color(Color::White) : nothing;
						requestElements.push_back(text(helpRequests[i]) | style);
					}
					return vbox(requestElements) | frame | vscroll_indicator | size(HEIGHT, LESS_THAN, 10);
				}),

				Button("Select Next", [&] {
					if (!helpRequests.empty()) {
						selectedIndex = (selectedIndex + 1) % helpRequests.size();
					}
				}),
				Button("Select Previous", [&] {
					if (!helpRequests.empty()) {
						selectedIndex = (selectedIndex - 1 + helpRequests.size()) % helpRequests.size();
					}
				}),

				deleteRequestButton,
				exitButton,
				});

			auto renderer = Renderer(layout, [&] {
				return vbox({
					text("Help Request System") | bold | center,
					separator(),
					vbox({
						text("Submit a new help request:"),
						submitRequestInput->Render(),
						submitRequestButton->Render() | hcenter,
						separator(),
						text("Select a request to delete:"),
						layout->Render(),
						separator(),
						statusMessage.empty() ? text("") : text(statusMessage) | color(Color::Green),
						exitButton->Render() | hcenter,
					}) | border
					});
				});

			screen.Loop(renderer);
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
