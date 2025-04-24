#pragma once

#include <string>
#include <vector>
#include "Transfer.h"
#include "Account.h"
#include "Customer.h"

using namespace std;

/**
 * @file Transfer.h
 * @brief Defines the Transfer class for managing account-to-account money transfers.
 */

 /**
  * @class Transfer
  * @brief Facilitates money transfers between two accounts.
  * @tparam T The type used for the account balances and transfer amount (e.g., double, float).
  */
template <typename T>
class Transfer {
private:
    Account<T>* accountFrom; ///< Pointer to the sender's account.
    Account<T>* accountTo;   ///< Pointer to the receiver's account.
    double amount;           ///< The amount to be transferred.
    vector<Account<T>*> accounts; ///< Collection of accounts (optional, unused in current logic).

public:
    /**
     * @brief Default constructor initializes a Transfer object with no accounts and zero amount.
     */
    Transfer()
        : accountFrom(nullptr), accountTo(nullptr), amount(0) {
    }

    /**
     * @brief Constructor initializes a Transfer object with specified sender and receiver accounts.
     * @param accountFrom Pointer to the sender's account.
     * @param accountTo Pointer to the receiver's account.
     */
    Transfer(Account<T>* accountFrom, Account<T>* accountTo)
    {
        this->accountFrom = accountFrom;
        this->accountTo = accountTo;
        this->amount = 0;
    }

    /**
     * @brief Gets the sender's account.
     * @return Pointer to the sender's account.
     */
    Account<T>* getAccountFrom() const
    {
        return this->accountFrom;
    }

    /**
     * @brief Gets the receiver's account.
     * @return Pointer to the receiver's account.
     */
    Account<T>* getAccountTo() const
    {
        return this->accountTo;
    }

    /**
     * @brief Sets the sender's account.
     * @param accountFrom Pointer to the sender's account.
     */
    void setAccountFrom(Account<T>* accountFrom)
    {
        this->accountFrom = accountFrom;
    }

    /**
     * @brief Sets the receiver's account.
     * @param accountTo Pointer to the receiver's account.
     */
    void setAccountTo(Account<T>* accountTo)
    {
        this->accountTo = accountTo;
    }

    /**
     * @brief Sets the amount to be transferred.
     * @param amount The amount to transfer.
     *
     * This method validates the amount to ensure it is positive and does not exceed
     * the balance of the sender's account. If the validation fails, an error message is displayed.
     */
    void setAmount(T amount)
    {
        if (amount <= 0 || amount > accountFrom->getBalance())
        {
            cout << "Error: Could not set the amount of transfer." << endl;
            return;
        }

        this->amount = amount;
    }
};