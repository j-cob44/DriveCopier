// DriveFuncs.cpp
// Jacob Burton - Jan 2023

#include "DriveFuncs.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>

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
                driveRoot += ":\\";

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

