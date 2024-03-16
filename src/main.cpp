#include "ChangeLockscreenDaemon.hpp"
#include "ChangeLockscreenData.hpp"
#include "Log.hpp"
#include <windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *pCmdLine, int nCmdShow)
{
    FreeConsole();

    ChangeLockscreenDaemon daemon;
    if (!daemon.Create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

    daemon.logger.log(false, true, L"Initialized");

    MSG msg = {};

    daemon.changeLockscreen();
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}