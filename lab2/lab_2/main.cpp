#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "choise.h"
#include "SysInfo.h"
#include "MemStatus.h"
#include "MemState.h"
#include "ReserveRegion.h"
#include "ReserveRegionAndCommit.h"
#include "WriteData.h"
#include "ProtectRegion.h"
#include "FreeMem.h"


using namespace std;

void info() {
    cout << " '1' - get information about the computer system\n"
        << " '2' - get the status of virtual memory\n"
        << " '3' - define the state of a specific memory area\n"
        << " '4' - reserve a memory region\n"
        << " '5' - reserve a memory region and give physical memory to it\n"
        << " '6' - write data to memory area\n"
        << " '7' - protect a concrete memory region\n"
        << " '8' - to free a concrete memory region\n"
        << " '9' - exit\n\n"
        << "Please type only a number of an option without <' '>.\n" << endl;
}

int main() {

    system("chcp 437");
    int selected = -1;
    while (selected != 9) {
        system("cls");
        cout << "This is a console application by Artem Ivanov,\n";
        cout << "a student of group 0305, for laboratory work\n";
        cout << "on the study of Memory Management using the Win32 API.\n\n";
        info();
        cout << "Type an option and press enter: ";
        selected = num_cin();

        if (selected >= 1 && selected <= 8) {
            if (selected != 9)
                if (selected == 1)
                    SysInfo();
            if (selected == 2)
                MemStatus();
            if (selected == 3)
                MemState();
            if (selected == 4)
                ReserveRegion();
            if (selected == 5)
                ReserveRegionAndCommit();
            if (selected == 6)
                WriteData();
            if (selected == 7)
                ProtectRegion();
            if (selected == 8)
                FreeMem();

        }
        else cout << "\nWrong option!\n";

        cout << endl;
        system("pause");
    }

    cout << "\nProgram exited.\n";
    return 0;
}