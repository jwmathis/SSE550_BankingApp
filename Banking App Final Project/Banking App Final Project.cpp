// Banking App Final Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;


int main() {

	//--------------------------------------------------------------------------------------
	//  Constant Declaration:

	const int New_Account = 1;
	const int Balance_Inquiry = 2;
	const int Deposit_Amount = 3;
	const int Withdraw_Amount = 4;
	const int Transfer_Amount = 5;
	const int Close_Account = 6;
	const int EXIT = 7;

	string	userName, password, firstName, lastName, Status;
	int fileStat, numDep;
	double income, currMon;
	char option[1];
	bool userInput;
	double* bracket;
	//--------------------------------------------------------------------------------------

	cout << "*************** Hello, Welcome to MercerMoney! ***************\n\n";

	cout << "What can we do for you today?" << endl;
	cout << "	1.) Open a new account\n"
		<< "	2.) Balance Inquiry\n"
		<< "	3.) Deposit Amount\n"
		<< "	4.) Withdraw Amount\n"
		<< "	5.) Transfer Amount\n"
		<< "	6.) Close an Account\n"
		<< "	7.) EXIT\n";

	cin >> userInput;
	switch (userInput) {
	case New_Account:
		// Load sign in screen
		break;
	case Balance_Inquiry:
		// 
		break;
	case Deposit_Amount:

		break;
	case Withdraw_Amount:

		break;
	case Transfer_Amount:

		break;
	case Close_Account:

		break;
	case EXIT:
		return 0;
		break;
	default:
		cout << "Wrong number inputted, try again";
		cin >> userInput; 
	}

	cout << "***************************************************************************\n";
	cout << "Hello " << firstName << " " << lastName << " let's get you started!" << endl;
	cout << "***************************************************************************\n";

	return 0;
}