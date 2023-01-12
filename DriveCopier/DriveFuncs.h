// DriveFuncs.h
// Jacob Burton - Jan 2023

#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <fstream>
#include <filesystem>

// Global Drive Functions

#ifndef DriveFuncs
#define DriveFuncs
	typedef struct DriveInfo {
		char driveLetter;
		int driveType;
		__int64 totalSize;
		__int64 freeSpace;
	} dInfo;

	std::vector<DriveInfo> DF_DrivesInfo();
	bool DF_DriveList(char fromDriveLetter);
	bool DF_DriveCopy(char fromDriveLetter, char toDriveLetter);
#endif // !DiscFuncs
