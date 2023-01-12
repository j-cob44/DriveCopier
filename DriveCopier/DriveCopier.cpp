// DriveCopier.cpp : Main Function
// Jacob Burton - Jan 2023

#include <Windows.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cctype>

#include "DriveFuncs.h"

std::vector< DriveInfo > validDrives;
std::vector<char> validLetters;

// User Action 1: List Drive Info
void listDriveInfo() {
    std::vector< DriveInfo > PresentDriveInfo = DF_DrivesInfo();

    validDrives.clear();
    validLetters.clear();

    std::cout << "\nLetter\tDrive Type\tTotal Size \tFree Space\n";
    std::cout << "----------------------------------------------------\n";
    for (u_int i = 0; i < PresentDriveInfo.size(); i++) {
        // letter
        printf("%c\t", PresentDriveInfo[i].driveLetter);
        validLetters.push_back(PresentDriveInfo[i].driveLetter);

        // type
        if (PresentDriveInfo[i].driveType == 2) {
            printf("Removeable\t");
        }
        else if (PresentDriveInfo[i].driveType == 3) {
            printf("Disk Drive\t");
        }

        // total size
        printf("%llu MB\t", PresentDriveInfo[i].totalSize);

        // free space
        printf("%llu MB\t", PresentDriveInfo[i].freeSpace);

        // end
        validDrives.push_back(PresentDriveInfo[i]);
        std::cout << "\n";
    }
}

// User Action 2: List all Data on Drive
void listDriveData() {
    std::string input = "0";

    listDriveInfo();
    std::cout << "\nSelect which drive to list data from:\n";
    std::cin >> input;

    char fromDrive = toupper(input[0]);
    __int64 fromDriveSize = 0;

    // Find Drive by Letter
    if (std::find(validLetters.begin(), validLetters.end(), fromDrive) != validLetters.end()) {
        // found
        printf("\n%c Drive selected.\n", fromDrive);

    }
    else {
        // invalid drive letter
        std::cout << "\nInvalid Drive selected.\n";
        return;
    }

    std::cout << "\nIs this correct? (y/n)\n";
    std::cin >> input;

    char confirmation = toupper(input[0]);
    if (confirmation == 'Y') {
        // Create root string
        char fromDriveRoot[5] = { fromDrive, ':', '/', '*', '\0' };

        bool result = DF_ListNextDirectory(fromDriveRoot);
        return;
    }
    else if (confirmation == 'N') {
        // abort
        std::cout << "\nAction Aborted.\n";
        return;
    }
    else {
        // error
        std::cout << "\nInvalid Input.\n";
        return;
    }
}

// User Action 3: Copy Drive to New Drive
void copyDrive() {
    std::string input = "0";

    listDriveInfo();
    std::cout << "\nSelect which drive to copy FROM:\n";
    std::cin >> input;

    char fromDrive = toupper(input[0]);
    __int64 fromDriveSize = 0;

    // Find Drive by Letter
    if (std::find(validLetters.begin(), validLetters.end(), fromDrive) != validLetters.end()) {
        // found
        printf("\n%c Drive selected.\n", fromDrive);

        // Get used drive space for check later
        for (u_int i = 0; i < validDrives.size(); i++) {
            if (validDrives[i].driveLetter == fromDrive) {
                fromDriveSize = validDrives[i].totalSize - validDrives[i].freeSpace;
            }
        }
    }
    else {
        // invalid drive letter
        std::cout << "\nInvalid Drive selected.\n";
        return;
    }

    // Get TO drive from user
    std::cout << "\nSelect which drive to copy TO:\n";
    std::cin >> input;

    char toDrive = toupper(input[0]);
    __int64 toDriveSize = 0;

    // Ensure TO drive is not the same as FROM drive
    if (toDrive == fromDrive) {
        // invalid drive
        std::cout << "\nYou cannot copy a drive to itself.\n";
        return;
    }

    // Find Drive by Letter
    if (std::find(validLetters.begin(), validLetters.end(), toDrive) != validLetters.end()) {
        // found
        printf("\n%c Drive selected.\n", toDrive);

        // Get FROM drive free space for check
        for (u_int i = 0; i < validDrives.size(); i++) {
            if (validDrives[i].driveLetter == toDrive) {
                toDriveSize = validDrives[i].freeSpace;
            }
        }

        // check if FROM drive is larger than the TO drive
        if (toDriveSize < fromDriveSize) {
            // error new drive too small
            std::cout << "\nThe new drive is too small to hold the data from the old drive. Please select a drive with more total storage space than the old drive is using.\n";
            return;
        }
    }
    else {
        // invalid drive letter
        std::cout << "\nInvalid Drive selected.\n";
        return;
    }
    

    printf("\nYou are copying from Drive %c to Drive %c .\n", fromDrive, toDrive);
    std::cout << "\nIs this correct? (y/n)\n";
    std::cin >> input;

    char confirmation = toupper(input[0]);
    if (confirmation == 'Y') {
        // continue
        std::cout << "\nCopying the drives.\n";

        // Create root strings
        char fromDriveRoot[5] = { fromDrive , ':', '/', '*', '\0' };

        bool result = DF_CopyNextDirectory(fromDriveRoot, toDrive);
        return;
    }
    else if (confirmation == 'N') {
        // abort
        std::cout << "\nAction Aborted.\n";
        return;
    }
    else {
        // error
        std::cout << "\nInvalid Input.\n";
        return;
    }
}

int main()
{   
    std::cout << "JB's Drive Copier\n";
    std::string input = "0";

    while (input != "q") {
        std::cout << "\nChoose an Action:\n";
        std::cout   << "1. List Drive Info\n"
                    << "2. List All Data on Drive\n"
                    << "3. Copy Drive Data to New Drive\n"
                    << "q. Quit\n\n";

        std::cin >> input;

        switch (input[0]) {
            case '1':
                listDriveInfo();
                break;
            case '2':
                listDriveData();
                break;
            case '3':
                copyDrive();
                break;
            case 'q':
                break;
            default:
                std::cout << "\nInvalid Input.\n\n";
        }
    }

    

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu