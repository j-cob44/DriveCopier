// DriveFuncs.cpp
// Jacob Burton - Jan 2023

#include "DriveFuncs.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <tchar.h>
#include <stdio.h>

#define Bytes_To_MB 1048576

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
            _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);

            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {

            }
            else {
                // Folder is not system files
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
            _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);

            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
                // system files, do not proceed

            }
            else {
                // Folder is not system files
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

