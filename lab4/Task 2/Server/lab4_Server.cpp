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

void create_pipe();
void connect_pipe();
void disconnect_pipe();
void write_to_pipe();
void wait_write();

int main()
{
	system("chcp 437");
	system("cls");
	ZeroMemory(&overlapped, sizeof(overlapped));
	while (true)
	{
		string inp;
		cout
			<< "1. Create a channel.\n"
			<< "2. Connect a channel.\n"
			<< "3. Turn off the channel.\n"
			<< "4. Start recording to the channel.\n"
			<< "5. Wait for the end of the recording.\n"
			<< "6. Close programm.\n";
		cin >> inp;

		switch (inp[0])
		{
		case '1':
			cout << "Enter the channel name: ";
			wcin >> pipe_name;
			create_pipe();
			break;
		case '2':
			connect_pipe();
			break;
		case '3':
			disconnect_pipe();
			break;
		case '4':
			write_to_pipe();
			break;
		case '5':
			wait_write();
			break;
		case '6':
			CloseHandle(pipe);
			return 0;
		}
		system("pause");
	}
}

void create_pipe()
{
	wstring temp_string = L"\\\\.\\pipe\\";
	if (pipe)
		CloseHandle(pipe);
	pipe = CreateNamedPipe(
		(temp_string + pipe_name).c_str(),
		PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_MESSAGE,
		2,
		MAX_PATH,
		MAX_PATH,
		0,
		NULL
	);

	if (pipe == INVALID_HANDLE_VALUE)
	{
		pipe = NULL;
		print_winapi_error();
	}
	else
		cout << "The channel was successfully created.\n";
}

void connect_pipe()
{
	if (ConnectNamedPipe(pipe, &overlapped))
		cout << "The connection was successful.\n";
	else
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while (HasOverlappedIoCompleted(&overlapped));
			cout << "Error Pending.\n";
		}
		else
			print_winapi_error();
	}
}

void disconnect_pipe()
{
	if (DisconnectNamedPipe(pipe))
		cout << "The channel was successfully disconnected.\n";
	else
		print_winapi_error();
}

VOID WINAPI after_write(DWORD error, DWORD bytes, LPOVERLAPPED lpov) {
	if (error)
		print_winapi_error();
	else
		cout << "The write operation has ended.\n";
}

void write_to_pipe()
{
	string s;
	cout << "Enter a line to write to the channel: ";
	cin >> s;

	ZeroMemory(&overlapped, sizeof(overlapped));
	if (!WriteFileEx(pipe, s.c_str(), s.length(), &overlapped, after_write))
		print_winapi_error();
	else
	{
		cout << "Start of the write operation.\n";
	}
}

void wait_write()
{
	if (!SleepEx(2000, TRUE))
		cout << "The write operation failed.\n";
}