#include <gtest/gtest.h>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"

// Test Case 1
TEST(SystemTest, RegisterCustomer) {
	Bank testBank("test.db");

	// Register a new customer
	ASSERT_TRUE(mbank.registerCustomer("John Doe", "johndoe", "1234"));

	// Try to register a customer with the same username
	ASSERT_FALSE(mbank.registerCustomer("John Doe", "johndoe", "1234"));
}

// Test Case 2
TEST(SystemTest, LoginCustomer) {
	Bank testBank("test.db");

	// Register and log in
	testBank.registerCustomer("Jane Doe", "janedoe", "5678");
	Customer* customer = testBank.login("janedoe", "5678");

	ASSERT_NE(customer, nullptr);
	EXPECT_EQ(customer->getName(), "Jane Doe");

	// Try logging in with incorrect password
	Customer* failedCustomerLogin = testBank.login("janedoe", "wrongpassword");
	ASSERT_EQ(failedCustomerLogin, nullptr);
}

// Test Case 3
TEST(SystemTest, CreateAccount) {
	Bank testBank("test.db");

	testBank.registerCustomer("Alice", "alice123", "5678");
	Customer* customer = testBank.login("alice123", "5678");

	// Create new accounts
	int accNum = testBank.generateAccountNumber();
	int accNum2 = testBank.generateAccountNumber();
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum), 1000.00));
	ASSERT_TRUE(testBank.addAccountForCustomer(customer->getId(), to_string(accNum2), 500.00));

	// Check account creation
	Account* account1 = testBank.getAccountByNumber(to_string(accNum));
	Account* account2 = testBank.getAccountByNumber(to_string(accNum2));

	ASSERT_NE(account1, nullptr);
	ASSERT_NE(account2, nullptr);
	EXPECT_EQ(account1->getBalance(), 1000.00);
	EXPECT_EQ(accout2->getBalance(), 500.00);
}

// Test Case 4
TEST(SystemTest, DepositFunds) {}

// Test Case 5
TEST(SystemTest, WithdrawFunds) {}

// Test Case 6
TEST(SystemTest, TransferFunds) {}

//  Test Case 7
TEST(SystemTest, CheckBalances) {}

//  Test Case 8
TEST(SystemTest, InvalidAccountNumber) {
	Bank testBank("test.db");
	
	// Check if account exists
	ASSERT_FALSE(testBank.accountNumberExists("0000000"));

}

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