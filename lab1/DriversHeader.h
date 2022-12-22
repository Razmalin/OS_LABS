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
	
	// SysFlags
	if (SysFlags & FILE_CASE_SENSITIVE_SEARCH)
		cout << "\nThe specified volume supports case-sensitive file names.";
	if (SysFlags & FILE_CASE_PRESERVED_NAMES)
		cout << "\nThe specified volume supports preserved case of file names when it places a name on disk.";
	if (SysFlags & FILE_UNICODE_ON_DISK)
		cout << "\nThe specified volume supports Unicode in file names as they appear on disk.";
	if (SysFlags & FILE_PERSISTENT_ACLS)
		cout << "\nThe specified volume preserves and enforces access control lists (ACL).";
	if (SysFlags & FILE_FILE_COMPRESSION)
		cout << "\nThe specified volume supports file-based compression.";
	if (SysFlags & FILE_VOLUME_QUOTAS)
		cout << "\nThe specified volume supports disk quotas.";
	if (SysFlags & FILE_SUPPORTS_SPARSE_FILES)
		cout << "\nThe specified volume supports sparse files.";
	if (SysFlags & FILE_SUPPORTS_REPARSE_POINTS)
		cout << "\nThe specified volume supports reparse points.";
	if (SysFlags & FILE_SUPPORTS_REMOTE_STORAGE)
		cout << "\nThe specified volume supports remote storage.";
	if (SysFlags & FILE_RETURNS_CLEANUP_RESULT_INFO)
		cout << "\nThe specified volume returns cleanup result info.";
	if (SysFlags & FILE_SUPPORTS_POSIX_UNLINK_RENAME)
		cout << "\nThe specified volume supports posix unlink rename.";
	if (SysFlags & FILE_VOLUME_IS_COMPRESSED)
		cout << "\nThe specified volume is a compressed volume, for example, a DoubleSpace volume.";
	if (SysFlags & FILE_SUPPORTS_OBJECT_IDS)
		cout << "\nThe specified volume supports object identifiers.";
	if (SysFlags & FILE_SUPPORTS_ENCRYPTION)
		cout << "\nThe specified volume supports the Encrypted File System (EFS).";
	if (SysFlags & FILE_NAMED_STREAMS)
		cout << "\nThe specified volume supports named streams.";
	if (SysFlags & FILE_READ_ONLY_VOLUME)
		cout << "\nThe specified volume is read-only.";
	if (SysFlags & FILE_SEQUENTIAL_WRITE_ONCE)
		cout << "\nThe specified volume supports a single sequential write.";
	if (SysFlags & FILE_SUPPORTS_TRANSACTIONS)
		cout << "\nThe specified volume supports transactions.";
	if (SysFlags & FILE_SUPPORTS_HARD_LINKS)
		cout << "\nThe specified volume supports hard links.";
	if (SysFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
		cout << "\nThe specified volume supports extended attributes.";
	if (SysFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
		cout << "\nThe file system supports open by FileID.";
	if (SysFlags & FILE_SUPPORTS_USN_JOURNAL)
		cout << "\nThe specified volume supports update sequence number (USN) journals.";
	if (SysFlags & FILE_SUPPORTS_INTEGRITY_STREAMS)
		cout << "\nThe specified volume supports integrity streams.";
	if (SysFlags & FILE_SUPPORTS_BLOCK_REFCOUNTING)
		cout << "\nThe specified volume supports sharing logical clusters between files on the same volume.";
	if (SysFlags & FILE_SUPPORTS_SPARSE_VDL)
		cout << "\nThe specified volume supports sparse VDL.";
	if (SysFlags & FILE_DAX_VOLUME)
		cout << "\nThe specified volume is a direct access (DAX) volume.";
	if (SysFlags & FILE_SUPPORTS_GHOSTING)
		cout << "\nThe specified volume supports ghosting";
	
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

