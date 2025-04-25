#include <windows.h>
#include <ctime>

#include "Calculator.h"
#include "MainMenu.h"

#pragma comment(lib, "user32")

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);

	Screen main_screen;
	main_screen.ProgramStart();
}