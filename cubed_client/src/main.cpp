#include <Windows.h>
#include "game.h"
#include <exception>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, char* cmdLine, int nCmdShow)
{
	try
	{
		game g;
		g.run();
	}
	catch (const std::exception& e)
	{
		MessageBoxA(0, e.what(), "Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}