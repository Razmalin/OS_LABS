#pragma once
#include <iostream>

#include <windows.h>
#include <fileapi.h>


using namespace std;

void printDrivesList() {
	int n;
	char dd[4];
	DWORD dr = GetLogicalDrives();

	for (int i = 0; i < 26; i++)
	{
		n = ((dr >> i) & 0x00000001);
		if (n == 1)
		{
			dd[0] = char(65 + i); dd[1] = ':'; dd[2] = '\\'; dd[3] = 0;
			cout << "Available disk drives : " << dd << endl;
		}
	}
}

void displayInfoAboutDisk(LPTSTR Path) {
	const DWORD volNameMaxLength = 50, fileSystemNameLength = 10;


	wchar_t volName[volNameMaxLength], fileSystemName[fileSystemNameLength];
	DWORD SecInClust, bytesInClust, FreeClustQua, ClustQua, NameSize, serialNumber, SysFlags;

	UINT driveType;
	driveType = GetDriveType(Path);
	switch (driveType) {
	case DRIVE_REMOVABLE:
		cout << "\nThe type of disk: ";
		cout << "Removable disk\n";
		break;
	case DRIVE_FIXED:
		cout << "\nThe type of disk: ";
		cout << "Fixed disk\n";
		break;
	case DRIVE_REMOTE:
		cout << "\nThe type of disk: ";
		cout << "Remote disk\n";
		break;
	case DRIVE_CDROM:
		cout << "\nThe type of disk: ";
		cout << "CD-ROM\n";
		break;
	case DRIVE_RAMDISK:
		cout << "\nThe type of disk: ";
		cout << "RAM-disk\n";
		break;
	default:
		cout << "\nError of disk type.\n";
	}

	// system names
	GetVolumeInformationW(Path, volName, volNameMaxLength, &serialNumber, &NameSize, &SysFlags, fileSystemName, fileSystemNameLength);
	wcout << "Disk name: " << Path
		<< "\nBuffer for disk name: " << volName
		<< "\nDisk size: " << volNameMaxLength
		<< "\nSerial number: " << serialNumber
		<< "\nMax file name size: " << NameSize
		<< "\nFile system flag: " << SysFlags
		<< "\nFile system: " << fileSystemName
		<< "\nBuffer size: " << fileSystemNameLength << endl;

	// FreeSpace
	GetDiskFreeSpace(Path, &SecInClust, &bytesInClust, &FreeClustQua, &ClustQua);
	cout << "\nQuantity of clusters: " << ClustQua
		<< "\nQuantity of free clusters: " << FreeClustQua
		<< "\nSectors in the cluster: " << SecInClust
		<< "\nBytes in the sector: " << bytesInClust << '\n' << endl;
}



void AboutDiskHelp() {
	DWORD drives = GetLogicalDrives();
	wchar_t selectedDisk;
	wchar_t path[] = L"A:\\";

		cout << "Please type only the name of the disk without <' '>\n";
		cout << "Enter the disk you want to see info about ('D' for example): ";
		wcin >> selectedDisk;
		selectedDisk = towupper(selectedDisk);
		std::cin.ignore(INT_MAX, '\n');

		if (selectedDisk >= 'A' && selectedDisk <= 'Z' && drives & (1 << (selectedDisk - 'A'))) {
			path[0] = selectedDisk;
			displayInfoAboutDisk(path);
		}
		else cout << "Incorrect disk name.\n";
}

