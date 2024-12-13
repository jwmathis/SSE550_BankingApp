#include <gtest/gtest.h>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"
#include "Transfer.h"

// Test Case 1
TEST(SystemTest, RegisterCustomer) {
	Bank testBank("test.db");

	cout << "Step 1: Registering a new customer...\n";
	// Register a new customer
	ASSERT_TRUE(testBank.registerCustomer("John Mathis", "jmathis7", "1113"));

	cout << "Step 2: Attempting to register a duplicate username...\n";
	// Try to register a customer with the same username
	ASSERT_FALSE(testBank.registerCustomer("Juan Mathis", "jmathis7", "1234"));
}

// Test Case 2
TEST(SystemTest, LoginCustomer) {
	Bank testBank("test.db");

	// Register and log in
	cout << "Step 1: Registering a new user and logging in...\n";
	testBank.registerCustomer("Jane Doe", "janedoe", "5678");
	Customer* customer = testBank.login("janedoe", "5678");

	cout << "Checking if customer was successfully logged in...\n";
	ASSERT_NE(customer, nullptr);
	EXPECT_EQ(customer->getName(), "Jane Doe");

	// Try logging in with incorrect password
	cout << "Attempting to log in with a wrong password...\n";
	Customer* failedCustomerLogin = testBank.login("janedoe", "wrongpassword");
	ASSERT_EQ(failedCustomerLogin, nullptr);
}

// Test Case 3
TEST(SystemTest, CreateAccount) {
	Bank testBank("test.db");

	cout << "Step 1: Register a new customer...\n";
	testBank.registerCustomer("Zachary C", "xXxZacAttacxXx", "0707");
	Customer* customer = testBank.login("xXxZacAttacxXx", "0707");

	// Create new accounts
	cout << "Step 2: Create two new accounts...\n";
	int accNum = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));
	int accNum2 = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum2), 500.00));

	// Check account creation
	cout << "Step 3: Check two new accounts using classes...\n";
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	Account* account2 = testBank.getAccountByNumber(to_string(accNum2));

	ASSERT_NE(account1, nullptr);
	ASSERT_NE(account2, nullptr);
	EXPECT_EQ(account1->getBalance(), 1000);
	EXPECT_EQ(account2->getBalance(), 500);
}

// Test Case 4
TEST(SystemTest, DepositFunds) {
	Bank testBank("test.db");
	cout << "Step 1: Login...\n";
	Customer* customer = testBank.login("xXxZacAttacxXx", "0707");

	// Create new accounts
	cout << "Step 2: Create a new account...\n";
	int accNum = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));

	// Check account creation
	cout << "Step 3: Check account balance...\n";
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	account1->deposit(500.00);
	EXPECT_EQ(account1->getBalance(), 1500.00);
}

//// Test Case 5
TEST(SystemTest, WithdrawFunds_SufficientFunds) {
	Bank testBank("test.db");
	cout << "Step 1: Login...\n";
	Customer* customer = testBank.login("jmathis7", "1113");

	// Create new accounts
	cout << "Step 2: Create a new account...\n";
	int accNum = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));

	// Check account creation
	cout << "Step 3: Check account balance...\n";
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	account1->withdraw(500.00);
	EXPECT_EQ(account1->getBalance(), 500.00);
}

// Test Case 6
TEST(SystemTest, WithdrawFunds_InsufficientFunds) {
	Bank testBank("test.db");
	cout << "Step 1: Login...\n";
	Customer* customer = testBank.login("jmathis7", "1113");

	// Create new accounts
	cout << "Step 2: Create a new account...\n";
	int accNum = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));

	// Check account creation
	cout << "Step 3: Check account balance...\n";
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	testing::internal::CaptureStderr();

	account1->withdraw(2000.00);
	string output = testing::internal::GetCapturedStderr();
	EXPECT_EQ(output, "Error: Insufficient funds.\n");
}

// Test Case 7
TEST(SystemTest, TransferFunds) {
	Bank testBank("test.db");
	testBank.registerCustomer("Colin", "colingreens", "1111");
	Customer* customer = testBank.login("colingreens", "1111");

	// Create new accounts
	cout << "Step 2: Create two new accounts...\n";
	int accNum = testBank.generateAccountNumber(1);
	testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00);
	int accNum2 = testBank.generateAccountNumber(1);
	testBank.addAccountForCustomer(customer->getId(), to_string(accNum2), 500.00);

	Account* senderAccount = testBank.getAccountByNumber(to_string(accNum)); // Fetch the sender account from database
	Account* recieverAccount = testBank.getAccountByNumber(to_string(accNum2)); // Fetch the recieverr account from database

	double amount = 300.00;
	Transfer transfer(*senderAccount, *recieverAccount);
	transfer.setAmount(amount);
	senderAccount->withdraw(amount);
	recieverAccount->deposit(amount);

	testBank.updateAccountBalance(senderAccount->getId(), senderAccount->getBalance());
	testBank.updateAccountBalance(recieverAccount->getId(), recieverAccount->getBalance());

	EXPECT_EQ(senderAccount->getBalance(), 700.00);
	EXPECT_EQ(recieverAccount->getBalance(), 800.00);
}

// Test Case 8
TEST(SystemTest, BalanceValidation) {
	Bank testBank("test.db");
	cout << "Step 1: Login...\n";
	Customer* customer = testBank.login("xXxZacAttacxXx", "0707");

	// Create new accounts
	cout << "Step 2: Create a new account...\n";
	int accNum = testBank.generateAccountNumber(1);
	testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00);
	cout << "Step 3: Withdrawal #1...\n";
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	account1->withdraw(500.00);
	cout << "Step 4: Deposit #1...\n";
	account1->deposit(12.00);
	cout << "Step 5: Deposit #2...\n";
	account1->deposit(180.00);
	EXPECT_EQ(account1->getBalance(), 692.00);
}

// Test Case 9
TEST(SystemTest, DatabasePersistance) {
	Bank testBank("test.db");

	// Register and create accounts
	ASSERT_TRUE(testBank.registerCustomer("Trish N", "trish123", "1234"));
	Customer* customer = testBank.login("trish123", "1234");
	int accNum = testBank.generateAccountNumber(1);
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));

	// Restart the system
	delete customer;
	Bank newTestBank("test.db");

	// Verify data persistance in database
	Customer* reloadedCustomer = newTestBank.login("trish123", "1234");
	ASSERT_NE(reloadedCustomer, nullptr);
	Account* reloadedAccount = newTestBank.getAccountByNumber(to_string(accNum));
	ASSERT_NE(reloadedAccount, nullptr);
	EXPECT_EQ(reloadedAccount->getBalance(), 1000.00);
}

// Test Case 10
TEST(SystemTest, AccountDeletion) {
	Bank testBank("test.db");

	cout << "Step 1: Register a new customer...\n";
	testBank.registerCustomer("Corbin Jones", "corb", "5678");
	Customer* customer = testBank.login("corb", "5678");
	cout << "Step 2: Create new account...\n";
	int accNum = testBank.generateAccountNumber(1);
	testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 3000.00);
	Account* account = testBank.getAccountByNumber(to_string(accNum));
	cout << "Step 3: Check if account exist...\n";
	EXPECT_EQ(account->getBalance(), 3000.00);

	cout << "Step 4: Delete account...\n";
	testBank.accountRemoveAccount(account->getAccountNum());
	cout << "Step 5: Check if account was deleted...\n";
	ASSERT_EQ(testBank.getAccountByNumber(to_string(accNum)), nullptr);
}

// Main defined for testing execution
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}