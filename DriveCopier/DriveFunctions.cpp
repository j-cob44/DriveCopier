// DriveFunctions.cpp :
// Jacob Burton - January 2023

#include <Windows.h>
#include <tchar.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "DriveFunctions.h"
#include "DriveInfo.h"

namespace std {

#define Bytes_To_GB 1073741824
#define Bytes_To_MB 1048576
#define Bytes_To_KB 1024

// GetDriveInfo : Returns Drive Info stucts from all drives (Letter, Type, Size, Free Space)
vector<DriveInfo> DriveFunctions::GetDrivesInfo() {
	vector<DriveInfo> driveInfoResults;

    DWORD driveMask = GetLogicalDrives(); // Bitmask, LSB = Drive A

    if (driveMask == 0) {
        // error
    }
    else {
        char driveLetter = 'A';
        while (driveMask) {
            // Check using Bitwise AND for presence of drive
            if (driveMask & 1) {
                // Create Struct
                DriveInfo currentDrive;

                // Get Letter
                currentDrive.driveLetter = driveLetter;

                // Get Type
                string driveRoot;
                driveRoot.push_back(driveLetter);
                driveRoot += ":/";

                wstring temp = wstring(driveRoot.begin(), driveRoot.end());
                LPCWSTR lpRoot = temp.c_str();
                currentDrive.driveType = GetDriveTypeW(lpRoot);

                // If Type is removeable or fixed HDD, continue
                if (currentDrive.driveType == DRIVE_REMOVABLE || currentDrive.driveType == DRIVE_FIXED) {
                    // Get Total Space
                    ULARGE_INTEGER lpTotalBytes;
                    ULARGE_INTEGER lpTotalFreeBytes;

                    // Initialize Large Integers
                    lpTotalBytes.QuadPart = 1;
                    lpTotalFreeBytes.QuadPart = 1;

                    GetDiskFreeSpaceExW(lpRoot, NULL, &lpTotalBytes, &lpTotalFreeBytes);

                    // Calculate Total Space
                    __int64 driveTotalSpaceInMB = __int64(lpTotalBytes.QuadPart / Bytes_To_MB); // MB
                    currentDrive.totalSize = driveTotalSpaceInMB;

                    // Get Free Space
                    __int64 driveTotalFreeSpaceInMB = __int64(lpTotalFreeBytes.QuadPart / Bytes_To_MB); // MB
                    currentDrive.freeSpace = driveTotalFreeSpaceInMB;

                    // Finished
                    driveInfoResults.push_back(currentDrive); // Save to Results
                }
            }

            driveLetter++; // Increment Letter
            driveMask >>= 1; // Shift bitmask right 1
        }
    }

    return driveInfoResults;  // Return Vector of Structs
}

// List All Directory : iterate through all directories listing folder and file names, recursively traversing folders
bool DriveFunctions::ListAllDirectories(string directoryName) {
    // Change string into Long Pointer to Constant String
    LPCSTR lpSourceRoot = directoryName.c_str();

    printf("\n %s \n", lpSourceRoot); // Print Current Directory

    WIN32_FIND_DATA findFileData;
    HANDLE handleFind = FindFirstFile(lpSourceRoot, &findFileData);

    // Check for Error
    if (handleFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure, error
    }

    // Iterate over all items in directory
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system folder, do not proceed
            }
            else {
                string rootDirectoryectory = lpSourceRoot; // Copy root
                LPCSTR filename = findFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

                string thisDirectoryectory = filename; // Get foldername as String

                if (thisDirectoryectory.compare(".") == 0 || thisDirectoryectory.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    _tprintf(TEXT("\t%s    <DIR>\n"), findFileData.cFileName); // List Folder Name

                    // Correct Formatting
                    thisDirectoryectory += '/';
                    rootDirectoryectory.insert(rootDirectoryectory.length() - 1, thisDirectoryectory); // combine root path and foldername

                    ListAllDirectories(rootDirectoryectory); // Recursive
                }
            }
        }
        else { 
            _tprintf(TEXT("\t%s\n"), findFileData.cFileName); // List Filenames
        }
    } while (FindNextFile(handleFind, &findFileData) != 0);

    return true; // Successful
}

// Copy All Directory : iterate through all directories copying folders and file to a new root drive, recursively traversing folders
bool DriveFunctions::CopyAllDirectories(string dirName, char destinationDriveLetter) {
    LPCSTR lpSourceRoot = dirName.c_str();

    printf("\n %s \n", lpSourceRoot); // Display Current Directory

    WIN32_FIND_DATA findFileData;
    HANDLE handleFind = FindFirstFile(lpSourceRoot, &findFileData);

    // Check for Invalid Directory
    if (handleFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure, error
    }

    // Iterate through all items in current Directory
    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system folder, do not proceed

            }
            else {
                string rootDirectory = lpSourceRoot; // Copy root
                LPCSTR filename = findFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

                string thisDirectory = filename; // Get foldername as String

                if (thisDirectory.compare(".") == 0 || thisDirectory.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    _tprintf(TEXT("\t%s    <DIR>\n"), findFileData.cFileName); // Display Foldername

                    // Correct Formatting of Path for new Destination Directory
                    string newDirectory = rootDirectory;
                    string folderName = "\\" + thisDirectory;
                    newDirectory.replace(newDirectory.length() - 2, folderName.length(), folderName);
                    newDirectory[0] = destinationDriveLetter;

                    LPCSTR lp_Path = newDirectory.c_str();
                    CreateDirectory(lp_Path, NULL); // create directory on destination drive, NULL = no security attributes

                    // Correct Formatting of Path
                    thisDirectory += '/';
                    rootDirectory.insert(rootDirectory.length() - 1, thisDirectory); // combine root and foldername

                    CopyAllDirectories(rootDirectory, destinationDriveLetter); // Continue Recursion
                }
            }
        }
        else {
            _tprintf(TEXT("\t%s\n"), findFileData.cFileName); // Display Current Filename

            // Get sourcefile name
            string filePath = lpSourceRoot; // Copy file path
            LPCSTR filename = findFileData.cFileName; // Get foldername (WCHAR) to LPCSTR
            string thisFilename = filename; // Get filename as String
            filePath.replace(filePath.length() - 1, thisFilename.length(), thisFilename); // Combine root and filename

            // Get destination filepath
            string destinationPath = filePath;
            destinationPath[0] = destinationDriveLetter;

            // Copy file to new drive
            ifstream sourceFile;
            ofstream destinationFile;

            // Open source file
            sourceFile.open(filePath, ifstream::in | ifstream::binary);
            char bit = sourceFile.get(); // Read one bit

            if (sourceFile.bad()) {
                throw string("error");
            }

            // Open destination file
            destinationFile.open(destinationPath, ofstream::out | ifstream::binary);

            while (sourceFile.good()) {
                if (destinationFile.is_open()) {
                    // Write data to destination File
                    destinationFile << bit;

                    // Read next data bit
                    bit = sourceFile.get();
                }
                else {
                    // error
                }
            }

            // Close both files
            destinationFile.close();
            sourceFile.close();
        }

    } while (FindNextFile(handleFind, &findFileData) != 0);

    return true; // Successful
}

DriveFunctions::DriveFunctions() {
    this->validDrives = GetDrivesInfo();

    for (u_int i = 0; i < validDrives.size(); i++) {
        this->validLetters.push_back(validDrives[i].driveLetter); 
    }
}


} // end namespace