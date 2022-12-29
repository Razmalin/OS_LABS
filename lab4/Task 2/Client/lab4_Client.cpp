#include <Windows.h>
#include <iostream>


using namespace std;

void print_winapi_error() noexcept
{
	std::string message;
	DWORD error = GetLastError();

	if (error == 0)
		message = std::string();
	else
	{
		LPSTR message_buffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPSTR)&message_buffer, 0, NULL);
		message = std::string(message_buffer, size);
		LocalFree(message_buffer);
	}

	std::cout << "Fail: " << message << "(Error code: 0x" << std::hex << GetLastError() << ")\n";
}

HANDLE pipe = NULL;
OVERLAPPED overlapped;
wstring pipe_name;
UINT pipe_string_length;

void connect_pipe();
void read_from_pipe();
void wait_read();

int main()
{
	system("chcp 437");
	system("cls");
	ZeroMemory(&overlapped, sizeof(overlapped));
	while (true)
	{
		string inp;
		cout
			<< "1. Connect to a channel.\n"
			<< "2. Start reading from the channel.\n"
			<< "3. Output channel data.\n"
			<< "4. Close programm.\n";
		cin >> inp;

		switch (inp[0])
		{
		case '1':
			cout << "Enter the channel name: ";
			wcin >> pipe_name;
			connect_pipe();
			break;
		case '2':
			cout << "Enter the length of the line you want to read : ";
			cin >> pipe_string_length;
			read_from_pipe();
			break;
		case '3':
			wait_read();
			break;
		case '4':
			CloseHandle(pipe);
			return 0;
		}
		system("pause");
	}
}

void connect_pipe()
{
	wstring temp_string = L"\\\\.\\pipe\\";
	if (pipe)
		CloseHandle(pipe);
	pipe = CreateFile(
		(temp_string + pipe_name).c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
		NULL
	);
	if (pipe == INVALID_HANDLE_VALUE)
	{
		pipe = NULL;
		print_winapi_error();
	}
	else
		cout << "The connection was successful.\n";
}

VOID WINAPI after_read(DWORD error, DWORD bytes, LPOVERLAPPED lpov)
{
	char* str = (char*)lpov->hEvent;
	str[bytes] = '\0';
	if (error)
		print_winapi_error();
	else
		cout << "The reading is completed. The line read: " << str << "\n";
	delete[] str;
}

void read_from_pipe()
{
	ZeroMemory(&overlapped, sizeof(overlapped));
	char* pipe_string = new char[pipe_string_length + 1];
	overlapped.hEvent = pipe_string;
	if (ReadFileEx(pipe, pipe_string, pipe_string_length, &overlapped, after_read))
		cout << "The reading has begun.\n";
	else
		print_winapi_error();
}

void wait_read()
{
	if (!SleepEx(2000, TRUE))
		cout << "The read operation failed.\n";
}