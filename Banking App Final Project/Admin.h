#pragma once
#include <string>
#include <vector>
#include "Transfer.h"
#include "Account.h"
#include "Customer.h"
#include "Bank.h"
#include "SQLiteFunctions.h"
#include <iostream>
#include <algorithm>
using namespace std;

void merge(std::vector<Bank::acc>& accounts, int low, int mid, int high);
void merge_recurse(std::vector<Bank::acc>& accounts, int low, int high);
void mergeSort(std::vector<Bank::acc>& accounts, int length);
int findLowerBound(const std::vector<Bank::acc>& accounts, double minBalance);
vector<Bank::acc> getAccountsInBalanceRange(std::vector<Bank::acc>& accounts, double minBalance, double maxBalance);

// Binary Search Tree for storing accounts
struct TreeNode {
    Bank::acc account; // The account stored in this node
    TreeNode* left; // The left child node
    TreeNode* right; // The right child node

    TreeNode(Bank::acc acc) : account(acc), left(nullptr), right(nullptr) {}
};

class AccountBST {
private:
    TreeNode* root; // The root node of the tree

    TreeNode* insert(TreeNode* node, Bank::acc account) {
        if (!node) return new TreeNode(account); // Create a new node if none exists
        if (account.accountNumber < node->account.accountNumber) {
            node->left = insert(node->left, account); // Recursively insert into left subtree
        } else {
            node->right = insert(node->right, account); // Recursively insert into right subtree
        }
        return node;
    }

    void inorder(TreeNode* node) {
        if (!node) return; // Base case: empty tree
        inorder(node->left); // Recursively traverse left subtree
        printAccount(node->account); // Print the current node's account
        inorder(node->right); // Recursively traverse right subtree
    }

    Bank::acc* search(TreeNode* node, const string& accNumber) {
        if (!node) return nullptr; // Account not found
        if (node->account.accountNumber == accNumber) return &node->account; // Account found, return it
        if (accNumber < node->account.accountNumber) {
            return search(node->left, accNumber); // Recursively search left subtree
        } else {
            return search(node->right, accNumber); // Recursively search right subtree
        }
    }

public:
    AccountBST() : root(nullptr) {} // Initialize an empty tree

    void printAccount(const Bank::acc& acc) {
        cout << "Account ID: " << acc.accountId << "\n";
        cout << "User ID: " << acc.userId << "\n";
        cout << "Account Number: " << acc.accountNumber << "\n";
        cout << "Balance: " << acc.balance << "\n";
        cout << "Account Type: " << acc.accountType << "\n";
    }

    void insert(Bank::acc account) {
        root = insert(root, account); // Insert a new account into the tree
    }

    void buildTree(const vector<Bank::acc>& accounts) {
        for (const auto& account : accounts) {
            insert(account); // Insert each account into the tree
        }
    }

    void displayAllAccounts() {
        inorder(root); // Print all accounts in the tree
    }

    Bank::acc* searchAccount(const string& accNumber) {
        return search(root, accNumber); // Search for an account by account number
    }

};

// Merge Sort Functions
inline void merge(vector<Bank::acc>& accounts, int low, int mid, int high) {
    vector<Bank::acc> temp(high - low + 1);

    int i = low, j = mid + 1, k = 0;

    while (i <= mid && j <= high) {
        if (accounts[i].balance < accounts[j].balance) {
            temp[k++] = accounts[i++];
        }
        else {
            temp[k++] = accounts[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = accounts[i++];
    }

    while (j <= high) {
        temp[k++] = accounts[j++];
    }

    for (int i = low; i <= temp.size(); i++) {
        accounts[low + i] = temp[i];
    }
}

// Merge function to merge two halves of the array
inline void merge_recurse(vector<Bank::acc>& accounts, int low, int high) {
    if (low >= high) {
        return;
    }

    int mid = (low + high) / 2;
    merge_recurse(accounts, low, mid);
    merge_recurse(accounts, mid + 1, high);
    merge(accounts, low, mid, high);

}

// Merge Sort function to sort the array
inline void mergeSort(vector<Bank::acc>& accounts, int length) {
    merge_recurse(accounts, 0, length - 1);
}

// Binary Search Function
inline int findLowerBound(const vector<Bank::acc>& accounts, double minBalance) { // Binary Search: Returns the index of the first account with a balance greater than or equal to minBalance
    int low = 0, high = accounts.size() - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (accounts[mid].balance < minBalance) {
            low = mid + 1;
        }
        else {
            high = mid;
        }
    }

    return (accounts[low].balance >= minBalance) ? low : -1;
}

// Function to filter accounts based on balance range using merge sort and	binary search
inline vector<Bank::acc> getAccountsInBalanceRange(vector<Bank::acc>& accounts, double minBalance, double maxBalance) {
    std::sort(accounts.begin(), accounts.end(), [](const Bank::acc& a, const Bank::acc& b) {
        return a.balance < b.balance; // Compare based on balance
        });
    //mergeSort(accounts, accounts.size());

    int startIdx = findLowerBound(accounts, minBalance);
    if (startIdx == -1) return {};

    vector<Bank::acc> filteredAccounts;
    for (int i = startIdx; i < accounts.size() && accounts[i].balance <= maxBalance; i++) {
        filteredAccounts.push_back(accounts[i]);
    }

    return filteredAccounts;
}