// UserActions.cpp

#include <Windows.h>

#include <vector>
#include <iostream>

#include "DriveFunctions.h"
#include "DriveInfo.h"

namespace std {

// User Action 1: List Drive Info
void PrintDriveInfo(vector<DriveInfo> driveData) {
    cout << "\nLetter\tDrive Type\tTotal Size \tFree Space\n";
    cout << "----------------------------------------------------\n";
    for (u_int i = 0; i < driveData.size(); i++) {
        // Letter
        printf("%c\t", driveData[i].driveLetter);

        // Type
        if (driveData[i].driveType == 2) {
            printf("Removeable\t");
        }
        else if (driveData[i].driveType == 3) {
            printf("Disk Drive\t");
        }

        // Total size
        printf("%llu MB\t", driveData[i].totalSize);

        // Free space
        printf("%llu MB\t", driveData[i].freeSpace);

        // end/next
        cout << "\n";
    }
}

// User Action 2: List all Data on Drive
void PrintDataOnDrive(DriveFunctions DriveManipulator) {
    string input = "0";

    vector<char> validLetters = DriveManipulator.validLetters;

    PrintDriveInfo(DriveManipulator.validDrives);
    cout << "\nSelect which drive to list data from:\n";
    cin >> input;

    char sourceDriveLetter = toupper(input[0]);
    __int64 sourceDriveSize = 0;

    // Find Drive by Letter
    if (find(validLetters.begin(), validLetters.end(), sourceDriveLetter) != validLetters.end()) {
        // found
        printf("\n%c Drive selected.\n", sourceDriveLetter);

    }
    else {
        // invalid drive letter
        cout << "\nInvalid Drive selected.\n";
        return;
    }

    cout << "\nIs this correct? (y/n)\n";
    cin >> input;

    char confirmation = toupper(input[0]);
    if (confirmation == 'Y') {
        // Create root string
        char driveRoot[5] = { sourceDriveLetter, ':', '/', '*', '\0' };

        bool result = DriveManipulator.ListAllDirectories(driveRoot);
        return;
    }
    else if (confirmation == 'N') {
        // abort
        cout << "\nAction Aborted.\n";
        return;
    }
    else {
        // error
        cout << "\nInvalid Input.\n";
        return;
    }
}

// User Action 3: Copy Drive to New Drive
void CopyDriveToNewDrive(DriveFunctions DriveManipulator) {
    string input = "0";

    vector<DriveInfo> validDrives = DriveManipulator.validDrives;
    vector<char> validLetters = DriveManipulator.validLetters;

    PrintDriveInfo(DriveManipulator.validDrives);
    // Get Source drive from user
    cout << "\nSelect which drive to copy FROM:\n";
    cin >> input;

    char sourceDriveLetter = toupper(input[0]);
    __int64 sourceDriveSize = 0;

    // Find Drive by Letter
    if (find(validLetters.begin(), validLetters.end(), sourceDriveLetter) != validLetters.end()) {
        // found
        printf("\n%c Drive selected.\n", sourceDriveLetter);

        // Get used drive space for check later
        for (u_int i = 0; i < validDrives.size(); i++) {
            if (validDrives[i].driveLetter == sourceDriveLetter) {
                sourceDriveSize = validDrives[i].totalSize - validDrives[i].freeSpace;
            }
        }
    }
    else {
        // Invalid drive letter
        cout << "\nInvalid Drive selected.\n";
        return;
    }

    // Get Destination drive from user
    cout << "\nSelect which drive to copy TO:\n";
    cin >> input;

    char destinationDriveLetter = toupper(input[0]);
    __int64 destinationDriveSize = 0;

    // Ensure Destination drive is not the same as Source drive
    if (destinationDriveLetter == sourceDriveLetter) {
        // Invalid drive
        cout << "\nYou cannot copy a drive to itself.\n";
        return;
    }

    // Find Drive by Letter
    if (find(validLetters.begin(), validLetters.end(), sourceDriveLetter) != validLetters.end()) {
        // Found
        printf("\n%c Drive selected.\n", sourceDriveLetter);

        // Get Destination drive free space for check
        for (u_int i = 0; i < validDrives.size(); i++) {
            if (validDrives[i].driveLetter == sourceDriveLetter) {
                destinationDriveSize = validDrives[i].freeSpace;
            }
        }

        // Check if Source drive is larger than the Destination drive
        if (destinationDriveSize < sourceDriveSize) {
            // Error new drive too small
            cout << "\nThe new drive is too small to hold the data from the old drive. Please select a drive with more total storage space than the old drive is using.\n";
            return;
        }
    }
    else {
        // Invalid drive letter
        cout << "\nInvalid Drive selected.\n";
        return;
    }


    printf("\nYou are copying from Drive %c to Drive %c .\n", sourceDriveLetter, destinationDriveLetter);
    cout << "\nIs this correct? (y/n)\n";
    cin >> input;

    char confirmation = toupper(input[0]);
    if (confirmation == 'Y') {
        cout << "\nCopying the drives.\n";

        // Create root string
        char sourceDriveRoot[5] = { sourceDriveLetter , ':', '/', '*', '\0' };

        bool result = DriveManipulator.CopyAllDirectories(sourceDriveRoot, destinationDriveLetter);
        return;
    }
    else if (confirmation == 'N') {
        // Abort
        cout << "\nAction Aborted.\n";
        return;
    }
    else {
        // Error
        cout << "\nInvalid Input.\n";
        return;
    }
}

} // end namespace