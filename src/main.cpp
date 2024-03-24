#include "ChangeLockscreenDaemon.hpp"
#include "ParsedData.hpp"
#include "CommandLineParser.hpp"
#include "ParserException.hpp"
#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *pCmdLine, int nCmdShow)
{
    // Prevent 2 instances of daemon running.
    HANDLE single_instance_mutex = CreateMutex(NULL, TRUE, L"SINGLE INSTANCE");

    ParsedData parsed_data;

    try {
        parsed_data = CommandLineParser::Parse(pCmdLine);
    } catch(ParserException& e) {
        std::wcout << e.what() << "\n";
        return 0;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 0;
    }

    std::wcout << "rheurihaueruaw\n";

    ChangeLockscreenDaemon daemon{parsed_data};

    std::wcout << "lol\n";

    if (!daemon.create(L"CF Lockscreen image changer")) {
        return ErrorChangeLockscreen::build_daemon;
    }

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