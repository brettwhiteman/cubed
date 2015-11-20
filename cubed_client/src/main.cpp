#include <Windows.h>
#include "game.h"
#include <exception>
#include <codecvt>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, char* cmdLine, int nCmdShow)
{
	try
	{
		game g;
		g.run();
	}
	catch (const std::exception& e)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		MessageBox(0, converter.from_bytes(e.what()).c_str(), L"Error", MB_OK | MB_ICONERROR);
	}

	return 0;
}