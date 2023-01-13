// main.cpp : Main Function
// Jacob Burton - Jan 2023

#include <iostream>

#include "UserActions.h"
#include "DriveFunctions.h"

int main() {
    std::DriveFunctions DriveManipulator = std::DriveFunctions(); // DriveFunctions Object Initialization

    std::cout << "JB's Drive Copier\n";
    std::string input = "0";

    while (input != "q") {
        // Actions Display
        std::cout << "\nChoose an Action:\n";
        std::cout << "1. List Drive Info\n"
            << "2. List All Data on Drive\n"
            << "3. Copy Drive Data to New Drive\n"
            << "q. Quit\n\n";

        std::cin >> input;

        // User Input Switch Statement
        switch (input[0]) {
        case '1':
            PrintDriveInfo(DriveManipulator.validDrives);
            break;
        case '2':
            PrintDataOnDrive(DriveManipulator);
            break;
        case '3':
            CopyDriveToNewDrive(DriveManipulator);
            break;
        case 'q':
            break;
        default:
            std::cout << "\nInvalid Input.\n\n";
        }
    }

    return 0;
}