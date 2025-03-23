#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include "UIHelpers.h"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define ORANGE "\033[38;2;255;165;0m"

using namespace std;

void displayWelcomeAnimation() {
	const int frames = 6; // Total number of frames for the spin
	bool exitFlag = false;
	while (!exitFlag) {
		for (int i = 0; i < frames; ++i) {
			//clearScreen(); // Clear the console
			printAnimatedDollarSign(i, 5, 20); // Print the current frame
			std::this_thread::sleep_for(std::chrono::milliseconds(150)); // Pause for a moment

			if (_kbhit()) {
				char ch = _getch();
				if (ch == 13) { //ASCII for enter key
					exitFlag = true;
					break;
				}
			}
		}
	}
}
// Functions for animated dollar sign logo
void clearScreen() {
	// This works for Windows; for UNIX, use "clear"
	system("cls");
}

void moveToPosition(int row, int col) {
	cout << "\033[" << row << ";" << col << "H";
}

void printAnimatedDollarSign(int frame, int row, int col) {
	cout << GREEN;
	moveToPosition(row, col);
	switch (frame) {
	case 0: // Initial frame
		cout << "      $$$$       " << endl;
		moveToPosition(row + 1, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 2, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 3, col);
		cout << "   $$            " << endl;
		moveToPosition(row + 4, col);
		cout << "    $$$$$$       " << endl;
		moveToPosition(row + 5, col);
		cout << "         $$      " << endl;
		moveToPosition(row + 6, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 7, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 8, col);
		cout << "      $$$$       " << endl;
		break;
	case 1: // Slight rotation
		cout << "       $$        " << endl;
		moveToPosition(row + 1, col);
		cout << "      $$$$       " << endl;
		moveToPosition(row + 2, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 3, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 4, col);
		cout << "   $$            " << endl;
		moveToPosition(row + 5, col);
		cout << "    $$$$$$       " << endl;
		moveToPosition(row + 6, col);
		cout << "         $$      " << endl;
		moveToPosition(row + 7, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 8, col);
		cout << "    $$$  $$$     " << endl;
		break;
	case 2: // More rotation
		cout << "       $$        " << endl;
		moveToPosition(row + 1, col);
		cout << "       $$        " << endl;
		moveToPosition(row + 2, col);
		cout << "      $$$$       " << endl;
		moveToPosition(row + 3, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 4, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 5, col);
		cout << "   $$            " << endl;
		moveToPosition(row + 6, col);
		cout << "    $$$$$$       " << endl;
		moveToPosition(row + 7, col);
		cout << "         $$      " << endl;
		moveToPosition(row + 8, col);
		cout << "   $$      $$    " << endl;
		break;
	case 3: // Further rotation
		cout << "         $$      " << endl;
		moveToPosition(row + 1, col);
		cout << "       $$        " << endl;
		moveToPosition(row + 2, col);
		cout << "       $$        " << endl;
		moveToPosition(row + 3, col);
		cout << "      $$$$       " << endl;
		moveToPosition(row + 4, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 5, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 6, col);
		cout << "   $$            " << endl;
		moveToPosition(row + 7, col);
		cout << "    $$$$$$       " << endl;
		moveToPosition(row + 8, col);
		cout << "         $$      " << endl;
		break;
	case 4: // Almost back to original
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 1, col);
		cout << "         $$      " << endl;
		moveToPosition(row + 2, col);
		cout << "       $$        " << endl;
		moveToPosition(row + 3, col);
		cout << "       $$        " << endl;
		moveToPosition(row + 4, col);
		cout << "      $$$$       " << endl;
		moveToPosition(row + 5, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 6, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 7, col);
		cout << "   $$            " << endl;
		break;
	case 5: // Back to original position but flipped
		cout << "      $$$$       " << endl;
		moveToPosition(row + 1, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 2, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 3, col);
		cout << "         $$      " << endl;
		moveToPosition(row + 4, col);
		cout << "    $$$$$$       " << endl;
		moveToPosition(row + 5, col);
		cout << "   $$            " << endl;
		moveToPosition(row + 6, col);
		cout << "   $$      $$    " << endl;
		moveToPosition(row + 7, col);
		cout << "    $$$  $$$     " << endl;
		moveToPosition(row + 8, col);
		cout << "      $$$$       " << endl;
		break;
	}
	cout << RESET;
}

void printDollarSign() {
	cout << GREEN;
	cout << "                                          $$        " << endl;
	cout << "                                         $$$$       " << endl;
	cout << "                                       $$$  $$$     " << endl;
	cout << "                                      $$  $$  $$    " << endl;
	cout << "                                      $$  $$          " << endl;
	cout << "                                       $$$$$$$$$       " << endl;
	cout << "                                          $$   $$      " << endl;
	cout << "                                      $$  $$   $$    " << endl;
	cout << "                                       $$$  $$$     " << endl;
	cout << "                                         $$$$       " << endl;
	cout << "                                          $$        " << endl;
	cout << RESET;
}

// Function for MU logo
void printMU() {
	cout << ORANGE << endl;
	// Print the letter M and U side by side
	cout << "			     ||\\\\      //||   ||       || " << endl;  // Row 1
	cout << "			     || \\\\    // ||   ||       || " << endl;  // Row 2
	cout << "			     ||  \\\\  //  ||   ||       || " << endl;  // Row 3
	cout << "			     ||   \\\\//   ||   ||       || " << endl;  // Row 4
	cout << "			     ||          ||   ||       || " << endl;  // Row 5
	cout << "                             ||          ||   ===========" << endl;  // Row 6
	cout << RESET;
}

