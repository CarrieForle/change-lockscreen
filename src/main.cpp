#include "ChangeLockscreenDaemon.hpp"
#include "ChangeLockscreenData.hpp"
#include "Log.hpp"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *pCmdLine, int nCmdShow)
{

    // Prevent 2 instances of daemon running.
    HANDLE single_instance_mutex = CreateMutex(NULL, TRUE, L"SINGLE INSTANCE");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    ChangeLockscreenDaemon daemon;
    if (!daemon.Create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

    daemon.logger.writeTime(L"[%F %T]");
    daemon.logger.write(L"Daemon Initialized\n");

    MSG msg = {};

    daemon.changeLockscreen();
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseMutex(single_instance_mutex);

    return 0;
}