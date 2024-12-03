#include <gtest/gtest.h>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"

// Test Case 1
TEST(SystemTest, RegisterCustomer) {
	Bank testBank("test.db");

	cout << "Step 1: Registering a new customer...\n";
	// Register a new customer
	ASSERT_TRUE(testBank.registerCustomer("John Mathis", "jmathis7", "1234"));

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
	int accNum = testBank.generateAccountNumber();
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));
	int accNum2 = testBank.generateAccountNumber();
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
//TEST(SystemTest, DepositFunds) {}

//// Test Case 5
//TEST(SystemTest, WithdrawFunds) {}
//
//// Test Case 6
//TEST(SystemTest, TransferFunds) {}
//
////  Test Case 7
//TEST(SystemTest, CheckBalances) {}
// 
//// Test Case 8
//TEST(SystemTest, InvalidAccountNumber) {
//	Bank testBank("test.db");
//	
//	int accountNumber = testBank.generateAccountNumber();
//	// Check if account exists
//	ASSERT_FALSE(testBank.accountNumberExists(to_string(accountNumber)));
//}

// Test Case 9
TEST(SystemTest, DatabasePersistance) {
	Bank testBank("test.db");

	// Register and create accounts
	ASSERT_TRUE(testBank.registerCustomer("John Mathis", "jwmathis7", "1113"));
	Customer* customer = testBank.login("jwmathis7", "1113");
	int accNum = testBank.generateAccountNumber();
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));

	// Restart the system
	delete customer;
	Bank newTestBank("test.db");

	// Verify data persistance in database
	Customer* reloadedCustomer = newTestBank.login("jwmathis7", "1113");
	ASSERT_NE(reloadedCustomer, nullptr);
	Account* reloadedAccount = newTestBank.getAccountByNumber(to_string(accNum));
	ASSERT_NE(reloadedAccount, nullptr);
	EXPECT_EQ(reloadedAccount->getBalance(), 1000.00);
}

// Main defined for testing execution
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}