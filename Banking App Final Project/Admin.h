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
	Bank::acc account;
	TreeNode* left;
	TreeNode* right;

	TreeNode(Bank::acc acc) : account(acc), left(nullptr), right(nullptr) {}
};

class AccountBST {
private:
	TreeNode* root;

	TreeNode* insert(TreeNode* node, Bank::acc account) {
		if (!node) return new TreeNode(account);
		if (account.accountNumber < node->account.accountNumber) {
			node->left = insert(node->left, account);
		}
		else {
			node->right = insert(node->right, account);
		}
		return node;
	}

	void inorder(TreeNode* node) {
		if (!node) return;
		inorder(node->left);
		printAccount(node->account);
		inorder(node->right);
	}

	Bank::acc* search(TreeNode* node, const string& accNumber) {
		if (!node) return nullptr;
		if (node->account.accountNumber == accNumber) return &node->account;
		if (accNumber < node->account.accountNumber) {
			return search(node->left, accNumber);
		}
		else {
			return search(node->right, accNumber);
		}
	}
public:
	AccountBST() : root(nullptr) {}

	void printAccount(const Bank::acc& acc) {
		cout << "Account ID: " << acc.accountId << "\n";
		cout << "User ID: " << acc.userId << "\n";
		cout << "Account Number: " << acc.accountNumber << "\n";
		cout << "Balance: " << acc.balance << "\n";
		cout << "Account Type: " << acc.accountType << "\n";
	}

	void insert(Bank::acc account) {
		root = insert(root, account);
	}
	void buildTree(const vector<Bank::acc>& accounts) {
		for (const auto& account : accounts) {
			insert(account);
		}
	}

	void displayAllAccounts() {
		inorder(root);
	}
	Bank::acc* searchAccount(const string& accNumber) {
		return search(root, accNumber);
	}
};