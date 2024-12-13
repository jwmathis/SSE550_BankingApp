#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "UIHelpers.h"

using namespace std;

void displayWelcomeAnimation() {
	bool exitFlag = false;
	while (!exitFlag) {
		clearScreen(); // Clear the console
		printDollarSign(); // Print the current frame
		this_thread::sleep_for(chrono::milliseconds(200)); //Delay
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 13) { //ASCII for enter key
				exitFlag = true;
				break;
			}
		}
	}
}
// Functions for animated dollar sign logo
void clearScreen() {
	// This works for Windows; for UNIX, use "clear"
	system("cls");
}

void printDollarSign() {
// Initial frame
		cout << "      $$$$       " << endl;
		cout << "    $$$  $$$     " << endl;
		cout << "   $$      $$    " << endl;
		cout << "   $$            " << endl;
		cout << "    $$$$$$       " << endl;
		cout << "         $$      " << endl;
		cout << "   $$      $$    " << endl;
		cout << "    $$$  $$$     " << endl;
		cout << "      $$$$       " << endl;
}

// Function for MU logo
void printMU() {
	// Print the letter M and U side by side
	cout << "			     ||\\\\      //||   ||       || " << endl;  // Row 1
	cout << "			     || \\\\    // ||   ||       || " << endl;  // Row 2
	cout << "			     ||  \\\\  //  ||   ||       || " << endl;  // Row 3
	cout << "			     ||   \\\\//   ||   ||       || " << endl;  // Row 4
	cout << "			     ||          ||   ||       || " << endl;  // Row 5
	cout << "                             ||          ||   ===========" << endl;  // Row 6
}

