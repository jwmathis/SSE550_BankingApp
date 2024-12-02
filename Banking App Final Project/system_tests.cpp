#include <gtest/gtest.h>
#include "Bank.h"
#include "Customer.h"
#include "Account.h"

TEST(BankTests, RegisterCustomer) {
	const char* databaseDir = "test.db";
	Bank mbank(databaseDir);

	ASSERT_TRUE(mbank.registerCustomer("John Doe", "johndoe", "1234"));
	ASSERT_FALSE(mbank.registerCustomer("John Doe", "johndoe", "1234"));
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}