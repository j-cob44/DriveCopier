// UserActions.h : Global function declarations for user actions
// Jacob Burton - Jan 2023

#pragma once
#ifndef USERACTIONS_H_
#define USERACTIONS_H_

#include <vector>

#include "DriveFunctions.h"
#include "DriveInfo.h"

namespace std {

// Global Functions for User Actions
void PrintDriveInfo(vector<DriveInfo> driveData);
void PrintDataOnDrive(DriveFunctions DriveManipulator);
void CopyDriveToNewDrive(DriveFunctions DriveManipulator);

} // end namespace

#endif  // USERACTIONS_H_