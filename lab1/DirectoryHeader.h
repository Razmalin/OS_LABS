#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using namespace std;

int num_cin() {
    int choice = -1;
    char str[50];
    cin >> str;
    while (sscanf(str, "%d", &choice) != 1) {
        printf("Incorrect input! Try again use only numbers:");
        cin >> str;
    }
    return choice;
}

void createDirectory() {

    wchar_t path[50];
    fflush(stdout);

    cout << "Enter the full name of path (D:\\Users\\User\\NewFolder for example): ";
    wcin >> path;
    fflush(stdout);

	if (CreateDirectory(path, NULL))
		cout << "Directory succsessfully created." << endl;
	else
		cout << "Error. Make sure that the path is entered correctly." << endl;
}

void removeDirectory() {

    wchar_t path[50];
    fflush(stdout);

	cout << "Enter the full name of path (D:\\Users\\User\\NewFolder for example): ";
    wcin >> path;
    fflush(stdout);
	if (RemoveDirectory(path))
		cout << "Directory succsessfully removed." << endl;
	else
		cout << "Error. Make sure that the path is entered correctly or the folder is not empty." << endl;
}

void removeORdelete() {

    int selected = -1;
    while (selected != 2 && selected != 1) {
        cout << "\nSelect an option\n";
        cout << " '1' - create new directory\n";
        cout << " '2' - remove directory\n";
        cout << "Type an option and press enter: ";
        selected = num_cin();


        if (selected >= 1 && selected <= 2) {
            if (selected == 1)
                createDirectory();
            if (selected == 2)
                removeDirectory();
        }
        else cout << "\nWrong option!\n";

    }
}