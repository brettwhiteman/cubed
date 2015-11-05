#include "game.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, char* cmdLine, int nCmdShow)
{
	Game game();
	game.run();
}