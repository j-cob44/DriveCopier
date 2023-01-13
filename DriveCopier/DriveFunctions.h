// DriveFunctions.h : Header file for DriveFunctions Class
// Jacob Burton - January 2023

#pragma once
#ifndef DRIVEFUNCTIONS_H_
#define DRIVEFUNCTIONS_H_

#include <vector>

#include "DriveInfo.h"

namespace std {

// Drive Functions Class which stores Valid Drives and Valid Drive Letters along with multiple Manipulation Functions
class DriveFunctions {
	public:
		vector<DriveInfo> validDrives;
		vector<char> validLetters;

		DriveFunctions();

		// Retrieve all Drive Info of Computer
		vector<DriveInfo> GetDrivesInfo();

		// List all files and folders starting at a Root Directory
		bool ListAllDirectories(string directoryName);

		// Copy all files and folders starting from a source to a destination, starting at a Root Directory
		bool CopyAllDirectories(string directoryName, char destinationDriveLetter);
};

} // end namespace

#endif  // DRIVEFUNCTIONS_H_