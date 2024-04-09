#include "ChangeLockscreenDaemon.hpp"
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
    if (!daemon.create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

    std::wcout << "Here";

    daemon.logger.writeTime(L"[%F %T] Demon Initialized\n");

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