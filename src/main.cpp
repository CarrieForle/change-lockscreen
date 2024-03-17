#include "ChangeLockscreenDaemon.hpp"
#include "ChangeLockscreenData.hpp"
#include "Log.hpp"
#include <windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *pCmdLine, int nCmdShow)
{
    ChangeLockscreenDaemon daemon;
    if (!daemon.Create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

    daemon.logger.log(L"Daemon Initialized");

    MSG msg = {};

    daemon.changeLockscreen();
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}