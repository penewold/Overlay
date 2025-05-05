#include <windows.h>
#include <iostream>
#include <string>

void initLogger() {
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);
	std::cout.clear();
	std::cerr.clear();
	std::cin.clear();
	std::wcout.clear();
	std::wcerr.clear();
	std::wcin.clear();
}

const char* getLogStrength(int id) {
	switch (id)
	{
	case 0:
		return "[INFO] ";
	case 1:
		return "[WARN] ";
	case 2:
		return "[ERRO] ";
	default:
		return "[INFO] ";
		break;

	}
}

void debug(double value, int id = 0) {
	std::cout << getLogStrength(id) << std::to_string(value) << std::endl;
}
void debug(char* value, int id = 0) {
	std::cout << getLogStrength(id) << value << std::endl;
}
void debug(int value, int id = 0) {
	std::cout << getLogStrength(id) << std::to_string(value) << std::endl;
}
void debug(float value, int id = 0) {
	std::cout << getLogStrength(id) << std::to_string(value) << std::endl;
}