#pragma once
#include <string>
// Color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define ORANGE "\033[38;2;255;165;0m"

using namespace std;

// Functions for animated dollar sign logo
void displayWelcomeAnimation(void);
void clearScreen(void);
void moveToPosition(int row, int col);
void printAnimatedDollarSign(int frame, int row, int col);
// Function for static dollar sign
void printDollarSign(void);
// Function for MU logo
void printMU();
string getRandomFruitEmoji(void); // Function to get a random fruit emoji
