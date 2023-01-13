// DriveInfo.h : Drive Info struct creation
// Jacob Burton - Jan 2023

#pragma once
#ifndef DRIVEINFO_H_
#define DRIVEINFO_H_

namespace std {

// Struct containing Drive Letter, Type, Size, and Free Space
typedef struct DriveInfo {
	char driveLetter;
	int driveType;
	__int64 totalSize;
	__int64 freeSpace;
} DriveInfo;

} // end namespace

#endif  // DRIVEINFO_H_