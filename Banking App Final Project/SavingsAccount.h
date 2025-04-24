#pragma once

#include "Account.h"
#include <iostream>
#include <string>

/**
 * @file SavingsAccount.h
 * @brief Defines the SavingsAccount class, which extends the Account class with interest rate functionality.
 */

 /**
  * @class SavingsAccount
  * @brief Represents a savings account with an associated interest rate.
  * @tparam T The type used for the balance and interest rate (e.g., double, float).
  */
template <typename T>
class SavingsAccount : public Account<T> {
private:
    T interestRate; ///< The interest rate for the savings account.

public:
    /**
     * @brief Constructs a SavingsAccount object.
     * @param id The unique ID of the account.
     * @param accNum The account number.
     * @param initialBalance The initial balance of the account.
     * @param interestRate The interest rate for the account.
     */
    SavingsAccount(int id, const string& accNum, double initialBalance, double interestRate)
        : Account<T>(id, accNum, initialBalance), interestRate(interestRate) {}

    /**
     * @brief Applies interest to the account balance.
     *
     * This method calculates the interest based on the account balance and interest rate,
     * and adds the interest to the balance. If the interest rate is not greater than 0, an error message is displayed.
     */
    void applyInterest() override {
        if (this->interestRate > 0) {
            T interest = this->balance * this->interestRate; ///< Calculate interest.
            this->balance += interest; ///< Add interest to the balance.
        }
        else {
            cerr << "Error: Interest rate must be greater than 0." << endl; ///< Display error for invalid interest rate.
        }
    }
};