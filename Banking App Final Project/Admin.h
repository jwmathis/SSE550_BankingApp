#pragma once
#include <string>
#include <vector>
#include "Transfer.h"
#include "Account.h"
#include "Customer.h"
#include "Bank.h"
#include "SQLiteFunctions.h"
#include <iostream>

using namespace std;

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