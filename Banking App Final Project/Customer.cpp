#include "Customer.h"
#include <string>
#include <iostream>
#include <fstream>

Customer::Customer(int id, const string& name, const string& username, const string& pin) 
	: id(id), name(name), username(username), pin(pin) {}

int Customer::getId() const {
	return id;
}

const string& Customer::getName() const {
	return name;
}

const string& Customer::getUsername() const {
	return username;
}

bool Customer::verifyPin(const string& inputPin) const {
	return inputPin == pin;
}

string Customer::generateTransactionReceiptFilename() {
	string filename = name + "_transaction_reciept.txt";
	return filename;
}

void Customer::generateTransactionReceipt(const string& transaction) {
	try {
		string filename = generateTransactionReceiptFilename();
		ofstream outputFile(filename, ios::app);
		// Check if file opened successfully
		if (!outputFile.is_open()) {
			throw ios_base::failure("Failed to open file: " + filename);
		}

		// Write to file
		outputFile << "Transaction: " << transaction << "\n";
		outputFile << "------------------------------------------\n";
		cout << "Transaction logged to reciept." << endl;
		outputFile.close();
	}
	catch (const ios_base::failure& e) {
		cerr << "Unexpected error occurred: " << e.what() << endl;
	}
}