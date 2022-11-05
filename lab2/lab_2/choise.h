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