#include "Game/Main.h"

/* Game initialization */
void TheMain()
{
	GameInit();
}

/* System entry point */
#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nCmdShow;

	TheMain();
	return 0;
}

#else

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	TheMain();
	return 0;
}

#endif
