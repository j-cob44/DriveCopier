// DriveCopier.cpp : Main Function
// Jacob Burton - Jan 2023

#include <Windows.h>
#include <iostream>

#include "DriveFuncs.h"
#include <vector>
#include <stdio.h>

void listDriveInfo() {
    std::vector< DriveInfo > PresentDriveInfo = DF_DrivesInfo();

    std::cout << "\nLetter\tDrive Type\tTotal Size \tFree Space\n";
    std::cout << "----------------------------------------------------\n";
    for (int i = 0; i < PresentDriveInfo.size(); i++) {
        // letter
        printf("%c\t", PresentDriveInfo[i].driveLetter);

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
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main()
{   
    std::string input = "0";

    while (input != "q") {
        std::cout << "Choose an Action:\n";
        std::cout   << "1. List Drive Info\n"
                    << "q. Quit\n\n";

        std::cin >> input;

        switch (input[0]) {
            case '1':
                listDriveInfo();
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