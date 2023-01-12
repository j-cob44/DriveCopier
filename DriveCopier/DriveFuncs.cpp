// DriveFuncs.cpp
// Jacob Burton - Jan 2023

#include "DriveFuncs.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include <filesystem>


#define Bytes_To_MB 1048576

// Global Drive Info Retrieval Function
std::vector<DriveInfo> DF_DrivesInfo()
{
    std::vector< DriveInfo > AllDriveInfo;


    DWORD driveMask = GetLogicalDrives(); // Bitmask, LSB = Drive A

    if (driveMask == 0) {
        // error
    }
    else {
        char driveLetter = 'A';
        while (driveMask) {
            // BITWISE AND
            if (driveMask & 1) {
                // Drive present
                DriveInfo curDrive;

                // Get Letter
                curDrive.driveLetter = driveLetter;

                // Get Type
                std::string driveRoot;
                driveRoot.push_back(driveLetter);
                driveRoot += ":/";

                std::wstring temp = std::wstring(driveRoot.begin(), driveRoot.end());
                LPCWSTR lp_root = temp.c_str();
                curDrive.driveType = GetDriveTypeW(lp_root);

                if (curDrive.driveType == DRIVE_REMOVABLE || curDrive.driveType == DRIVE_FIXED) {
                    // Get Total Space
                    ULARGE_INTEGER lpTotalBytes;
                    ULARGE_INTEGER lpTotalFreeBytes;

                    lpTotalBytes.QuadPart = 1;
                    lpTotalFreeBytes.QuadPart = 1;

                    GetDiskFreeSpaceExW(lp_root, NULL, &lpTotalBytes, &lpTotalFreeBytes);

                    __int64 driveTotalSpaceInMB = __int64(lpTotalBytes.QuadPart / Bytes_To_MB); // MB
                    curDrive.totalSize = driveTotalSpaceInMB;

                    // Get Free Space
                    __int64 driveTotalFreeSpaceInMB = __int64(lpTotalFreeBytes.QuadPart / Bytes_To_MB); // MB
                    curDrive.freeSpace = driveTotalFreeSpaceInMB;

                    // end
                    AllDriveInfo.push_back(curDrive);
                }
            }

            driveLetter++;
            // Shift bitmask right 1
            driveMask >>= 1;
        }
    }

    return AllDriveInfo;
}

// Local Recursive Function
bool OpenNextDirectory(std::string dirName) {
    LPCSTR lp_fromRoot = dirName.c_str();

    printf("\n %s \n", lp_fromRoot);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(lp_fromRoot, &FindFileData);

    // error
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure
    }

    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {

            }
            else {
                // Folder is not system files
                _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);

                std::string rootDir = lp_fromRoot; // Copy root
                LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

                std::string thisDir = filename; // Get foldername as String

                if (thisDir.compare(".") == 0 || thisDir.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    // continue

                    thisDir += '/';

                    rootDir.insert(rootDir.length() - 1, thisDir); // combine root and foldername

                    OpenNextDirectory(rootDir); // Recursive
                }
            }
        }
        else
            _tprintf(TEXT("  %s \n"), FindFileData.cFileName);
    } while (FindNextFile(hFind, &FindFileData) != 0);

    return true;
}

// Global List Function
bool DF_DriveList(char fromDriveLetter) {
    
    // Create root string
    char fromDriveRoot[5] = { fromDriveLetter , ':', '/', '*', '\0'};

    LPCSTR lp_fromRoot = fromDriveRoot;

    printf("\n %s \n", lp_fromRoot);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(lp_fromRoot, &FindFileData);

    // Check if File Found,
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure (error)
    }

    // Iterate through Drive
    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system files, do not proceed

            }
            else {
                // Folder is not system files
                _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);
                
                std::string rootDir = lp_fromRoot; // Copy root
                LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR
                
                std::string thisDir = filename; // Get foldername as String

                if (thisDir.compare(".") == 0 || thisDir.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    // continue

                    thisDir += '/';

                    rootDir.insert(rootDir.length() - 1, thisDir); // combine root and foldername

                    OpenNextDirectory(rootDir); // Begin Recursion
                }
            }
        }
        else
            _tprintf(TEXT("  %s \n"), FindFileData.cFileName);
    } while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);


    return true; // success
}

// Local Function for Recursion
bool CopyNextDirectory(std::string dirName, char toDriveLetter) {
    LPCSTR lp_fromRoot = dirName.c_str();

    printf("\n %s \n", lp_fromRoot);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(lp_fromRoot, &FindFileData);

    // error
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure
    }

    // Iterate through Drive
    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system files, do not proceed

            }
            else {
                // Folder is not system files
                _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);

                std::string rootDir = lp_fromRoot; // Copy root
                LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

                std::string thisDir = filename; // Get foldername as String

                if (thisDir.compare(".") == 0 || thisDir.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    // continue

                    // continue

                    // create directory
                    std::string newDir = rootDir;
                    std::string folderName = "\\" + thisDir;
                    newDir.replace(newDir.length() - 2, folderName.length(), folderName);
                    newDir[0] = toDriveLetter;

                    //throw std::string("error");

                    LPCSTR lp_Path = newDir.c_str();
                    CreateDirectory(lp_Path, NULL); // create directory , NULL = no security attributes

                    //throw std::string("error");

                    // add /* to end of 
                    thisDir += '/';

                    rootDir.insert(rootDir.length() - 1, thisDir); // combine root and foldername

                    CopyNextDirectory(rootDir, toDriveLetter); // Continue Recursion
                }
            }
        }
        else {
            _tprintf(TEXT("  %s \n"), FindFileData.cFileName);

            // get sourcefile name
            std::string filePath = lp_fromRoot; // Copy file path
            LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

            std::string thisDir = filename; // Get foldername as String

            filePath.replace(filePath.length() - 1, thisDir.length(), thisDir); // combine root and filename

            // get destination file name
            std::string destinationPath = filePath;
            destinationPath[0] = toDriveLetter;

            // Copy file to new drive
            std::ifstream sourceFile;
            std::ofstream destinationFile;

            // open source
            sourceFile.open(filePath, std::ifstream::in | std::ifstream::binary);
            char c = sourceFile.get();

            if (sourceFile.bad()) {
                throw std::string("error");
            }

            // open destination
            destinationFile.open(destinationPath, std::ofstream::out | std::ifstream::binary);

            while (sourceFile.good()) {
                if (destinationFile.is_open()) {
                    // write data
                    destinationFile << c;

                    // read next data bit
                    c = sourceFile.get();
                }
                else {
                    // error writing
                }
            }

            // close both
            destinationFile.close();
            sourceFile.close();
        }

    } while (FindNextFile(hFind, &FindFileData) != 0);

    return true;
}

// Global Copy Function
bool DF_DriveCopy(char fromDriveLetter, char toDriveLetter) {

    // Create root strings
    char fromDriveRoot[5] = { fromDriveLetter , ':', '/', '*', '\0' };

    LPCSTR lp_fromRoot = fromDriveRoot;

    printf("\n %s \n", lp_fromRoot);

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(lp_fromRoot, &FindFileData);

    // Check if File Found,
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return false; // failure (error)
    }

    // Iterate through Drive
    do {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system files, do not proceed

            }
            else {
                // Folder is not system files
                _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);
                
                std::string rootDir = lp_fromRoot; // Copy root
                LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

                std::string thisDir = filename; // Get foldername as String

                if (thisDir.compare(".") == 0 || thisDir.compare("..") == 0) {
                    // do not proceed
                }
                else {
                    // continue

                    // create directory
                    std::string newDir = rootDir;
                    std::string folderName = "\\" + thisDir;
                    newDir.replace(newDir.length() - 2, folderName.length(), folderName);
                    newDir[0] = toDriveLetter;

                    //throw std::string("error");

                    LPCSTR lp_Path = newDir.c_str();
                    CreateDirectory(lp_Path, NULL); // create directory , NULL = no security attributes

                    //throw std::string("error");

                    // add /* to end of 
                    thisDir += '/';

                    rootDir.insert(rootDir.length() - 1, thisDir); // combine root and foldername

                    CopyNextDirectory(rootDir, toDriveLetter); // Begin Recursion
                }
            }
        }
        else {
            _tprintf(TEXT("  %s \n"), FindFileData.cFileName);

            // get sourcefile name
            std::string filePath = lp_fromRoot; // Copy file path
            LPCSTR filename = FindFileData.cFileName; // Get foldername (WCHAR) to LPCSTR

            std::string thisDir = filename; // Get foldername as String

            filePath.replace(filePath.length() - 1, thisDir.length(), thisDir); // combine root and filename

            // get destination file name
            std::string destinationPath = filePath;
            destinationPath[0] = toDriveLetter;

            // Copy file to new drive
            std::ifstream sourceFile;
            std::ofstream destinationFile;

            // open source
            sourceFile.open(filePath, std::ifstream::in | std::ifstream::binary);
            char c = sourceFile.get();

            if (sourceFile.bad()) {
                throw std::string("error");
            }

            // open destination
            destinationFile.open(destinationPath, std::ofstream::out | std::ifstream::binary);

            while (sourceFile.good()) {
                if (destinationFile.is_open()) {
                    // write data
                    destinationFile << c;

                    // read next data bit
                    c = sourceFile.get();
                }
                else {
                    // error writing
                }
            }

            // close both
            destinationFile.close();
            sourceFile.close();
        }
            
    } while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);


    return true; // success
}
