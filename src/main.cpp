#include "ChangeLockScreenDaemon.hpp"
#include "ChangeLockScreenData.hpp"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *pCmdLine, int nCmdShow)
{
    // FreeConsole();
    /*
    How to create daemon CORRECTLY:
    1. Initialize a ChangeLockScreenDaemon instance. Call its Create()
    2. Call ChangeLockScreenDaemon.Initialize()
    */

    ChangeLockscreenDaemon daemon;
    if (!daemon.Create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

    MSG msg = {};

    daemon.changeLockscreen();
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        // printf("%d, %d, %d, %d\n", bRet, msg.message, msg.wParam, msg.lParam);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}