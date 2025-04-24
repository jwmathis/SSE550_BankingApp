#pragma once
#include <string>
#include <iostream>

using namespace std;

/**
 * @brief A template class representing a bank account.
 *
 * This class provides a basic implementation of a bank account with
 * operations for withdrawal, deposit, and applying interest.
 *
 * @tparam T The type of the balance (e.g., `double` for monetary values).
 */
template<typename T>
class Account {
protected:
	int id; ///< Unique identifier for the account
	string accountNum; ///< Account number
	T balance; ///< Account balance

public:
	/**
     * @brief Constructs an Account with the specified ID, account number, and initial balance.
     * 
     * @param id The unique identifier for the account.
     * @param accNum The account number.
     * @param initialBalance The initial balance of the account.
     */
	Account(int id, const string& accNum, double initialBalance)
		: id(id), accountNum(accNum), balance(initialBalance) {
	}

	/**
	 * @brief Constructs a default Account with ID 0, an empty account number, and a zero balance.
	 */
	Account() : id(0), accountNum(""), balance(0.0) {}

	/**
	 * @brief Retrieves the account's ID.
	 *
	 * @return The ID of the account.
	 */
	int getId() const {
		return id;
	}

	/**
	 * @brief Retrieves the account's number.
	 *
	 * @return A reference to the account number string.
	 */
	const string& getAccountNum() const {
		return accountNum;
	}

	/**
	 * @brief Retrieves the account's balance.
	 *
	 * @return The balance of the account.
	 */
	T getBalance() const {
		return balance;
	}

	/**
	 * @brief Withdraws a specified amount from the account.
	 *
	 * If the amount is greater than the balance or less than or equal to zero,
	 * an error message is displayed.
	 *
	 * @param amount The amount to withdraw.
	 */
	void withdraw(T amount) {
		if (amount > 0 && amount <= balance) {
			balance = balance - amount;
			cout << "Withdrew " << amount << ". New Balance: " << balance << endl;
		}
		else if (amount > balance) {
			cerr << "Error: Insufficient funds." << endl;
		}
		else {
			cerr << "Error: Withdrawal amount must be positive." << endl;
		}
	}

	/**
	 * @brief Deposits a specified amount into the account.
	 *
	 * If the amount is less than or equal to zero, an error message is displayed.
	 *
	 * @param amount The amount to deposit.
	 */
	void deposit(T amount) {
		if (amount > 0) {
			balance = balance + amount;
			cout << "Deposited " << amount << ". New Balance: " << balance << endl;
		}
		else {
			cerr << "Error: Deposit amount must be positive." << endl;
		}
	}

	/**
	 * @brief Applies interest to the account.
	 *
	 * This is a virtual method intended to be overridden by derived classes.
	 * By default, no interest is applied.
	 */
	virtual void applyInterest() {
		cout << "No interest" << endl;
	}

	/**
	 * @brief Virtual destructor for the Account class.
	 */
	virtual ~Account() = default;
};