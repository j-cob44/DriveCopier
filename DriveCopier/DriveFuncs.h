// DriveFuncs.h
// Jacob Burton - Jan 2023

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>

// Drive Functions

#ifndef DriveFuncs
#define DriveFuncs
	typedef struct DriveInfo {
		char driveLetter;
		int driveType;
		__int64 totalSize;
		__int64 freeSpace;
	} dInfo;

	std::vector<DriveInfo> DF_DrivesInfo();
#endif // !DiscFuncs
