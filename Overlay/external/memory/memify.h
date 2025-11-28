#pragma once
#include <Windows.h> // RPM + WPM
#include <TlHelp32.h> // getting processIds.
#include <Psapi.h> // enumprocessmodules
#include <string> // generally nicer to work with than chars, but that's your preference.
#include <vector> // std::vector
#include <locale>
#include <codecvt>

/*
Created By https://github.com/carlgwastaken/
Please Support Open Source and leave this message here if you're using in your own source
Besides that, enjoy!
*/

// the ReadProcessMemory is basically just a pointer to this function + some error handling, so we save some performance.
// 5% according to the post below:
// unknowncheats.me/forum/general-programming-and-reversing/230813-readprocessmemory-vs-ntreadvirtualmemory-performance-benchmark-comparison.html
typedef NTSTATUS(WINAPI* pNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
typedef NTSTATUS(WINAPI* pNtWriteVirtualMemory)(HANDLE Processhandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);


inline BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD targetPid = *(DWORD*)lParam;
	DWORD windowPid = 0;
	GetWindowThreadProcessId(hwnd, &windowPid);

	// Check if window belongs to the PID and is a top-level, visible window
	if (windowPid == targetPid && IsWindowVisible(hwnd)) {
		// Store the HWND and stop enumeration
		*(HWND*)lParam = hwnd;
		return FALSE; // Stop enumeration
	}

	return TRUE; // Continue enumeration
}

class memify
{
private:
	// initalize at 0 so we can check later
	HANDLE handle = 0;
	DWORD processID = 0;

	// define Virtual Read + Virtual Write
	pNtReadVirtualMemory VRead; 
	pNtWriteVirtualMemory VWrite;

	std::string WideToNarrow(const std::wstring& wideStr) {
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (size_needed <= 0) return "";

		std::string narrowStr(size_needed - 1, '\0'); // -1 to remove null terminator
		WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &narrowStr[0], size_needed, nullptr, nullptr);

		return narrowStr;
	}

	


	uintptr_t GetProcessId(std::string_view processName)
	{
		// define processentry32
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);

		// create a snapshot handle
		HANDLE ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		// loop through all process
		while (Process32Next(ss, &pe)) {
			std::string narrow = WideToNarrow(pe.szExeFile);
			
			// compare program names to processName
			if (processName == narrow) {
				processID = pe.th32ProcessID;
				return processID;
			}
		}
	}

	// uses already open handle to enumerate, you could also use PROCESSENTRY32 here.
	uintptr_t GetBaseModule(std::string_view moduleName)
	{
		HMODULE modules[1024];
		DWORD neededmodule; 

		if (EnumProcessModules(handle, modules, sizeof(modules), &neededmodule))
		{
			int moduleCount = neededmodule / sizeof(HMODULE);

			for (int i = 0; i < moduleCount; ++i)
			{
				char buffer[MAX_PATH];

				if (GetModuleBaseNameA(handle, modules[i], buffer, sizeof(buffer)))
				{
					if (!moduleName.compare(buffer)) {
						return reinterpret_cast<uintptr_t>(modules[i]);
					}
				}
			}
		}

		return 0;
	}
public:
	HWND hwnd = 0; // handle to the window, if you want to use it for something.

	// if you have an array of possible process names you want to loop through, and if one matches it will grab the handle for that.
	// this could be used if for example working with a mod that uses the base game for its gameplay, or a game that you usually play on multiple versions on.
	// keep in mind, this can get annoying with offsets VERY fast.
	memify(std::vector<std::string> processes) {
		VRead = (pNtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");
		VWrite = (pNtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");

		for (auto& name : processes) {
			processID = GetProcessId(name);

			if (processID != 0) {
				handle = OpenProcess(PROCESS_ALL_ACCESS, 0, processID);
				if (handle) {
					break;
				}
				else {
					// somehow we got a valid processID but not a valid handle?
					continue;
				}
			}
			continue;
		}

		// Use EnumWindows to iterate all top-level windows
		EnumWindows(EnumWindowsProc, (LPARAM)&processID);
		hwnd = (HWND)processID;
	}

	// constructor opens handle and you save one line!!!! (will make your spaghetti code 10x better)
	memify(std::string_view processName)
	{
		VRead = (pNtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");
		VWrite = (pNtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");

		processID = GetProcessId(processName);

		if (processID != 0)
		{
			handle = OpenProcess(PROCESS_ALL_ACCESS, 0, processID);
			if (!handle) {
				printf("[>>] Failed to open handle to process.");
			}
		}

		// Use EnumWindows to iterate all top-level windows
		EnumWindows(EnumWindowsProc, (LPARAM)&processID);
		hwnd = (HWND)processID;
	}

	// deconstructor, called automatically when closing.
	~memify()
	{
		if (handle)
			CloseHandle(handle);
	}

	// shorten name here
	uintptr_t GetBase(std::string_view moduleName)
	{
		return GetBaseModule(moduleName);
	}

	// read
	template <typename T> // use types which are defined later on, so it's compatible with alot of shit.
	T Read(uintptr_t address)
	{
		T buffer{ };
		bool result = VRead(handle, (void*)address, &buffer, sizeof(T), 0);

		if (result == false) {
			DWORD lastError = GetLastError();
			int hi = lastError;
		}
		
		return buffer;
	}

	template <typename T>
	T Write(uintptr_t address, T value)
	{
		VWrite(handle, (void*)address, &value, sizeof(T), NULL);
		return value;
	}

	// for reading structs and strings and shit
	bool ReadRaw(uintptr_t address, void* buffer, size_t size)
	{
		SIZE_T bytesRead;
		if (VRead(handle, (void*)address, buffer, static_cast<ULONG>(size), (PULONG)&bytesRead))
			return bytesRead = size;

		return false;
	}

	// utilities, shit that isn't required but nice to have

	bool ProcessIsOpen(const std::string_view processName)
	{
		return GetProcessId(processName) != 0;
	}

	bool InForeground(const std::string& windowName)
	{
		// just takes Counter-Strike 2 but change to whatever u want, or implement an input you can do that too
		// maybe get the foreground window and then compare it to your own window, use processID, anything u want
		HWND current = GetForegroundWindow();

		wchar_t title[256];
		GetWindowText(current, title, sizeof(title) / sizeof(wchar_t));

		if (strstr(WideToNarrow(title).c_str(), windowName.c_str()) != nullptr)
			return true;

		return false;
	}
};
