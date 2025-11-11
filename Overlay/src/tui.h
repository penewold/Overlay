#pragma once
#include <iostream>
#include <Windows.h>

void enableAnsi() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}

namespace tui {
	inline void resetScreen() { std::cout << "\033[H"; }
	inline std::ostream& red(std::ostream& os) { return os << "\033[31m"; }
	inline std::ostream& green(std::ostream& os) { return os << "\033[32m"; }
	inline std::ostream& bold(std::ostream& os) { return os << "\033[1m"; }
	inline std::ostream& reset(std::ostream& os) { return os << "\033[0m"; }
}