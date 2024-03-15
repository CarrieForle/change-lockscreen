#include "helper_functions.hpp"
#include <windows.h>

void WindowsException::ThrowIf(bool condition)
{
    if (condition)
    {
        throw WindowsException(GetLastError());
    }
}

/*
error should be GetLastError().
*/
WindowsException::WindowsException(DWORD error)
{
    if (!FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            error,
            0,
            (wchar_t*)&message,
            0, NULL))
    {
        wcscpy(message, L"Failed to retrieve error");
    }
}

WindowsException::~WindowsException()
{
    LocalFree(message);
}

// Don't use this method. Use WindowsException::what() instead.
char *WindowsException::what()
{
    return "Don't call what(). Use wwhat() instead.";
}

wchar_t* WindowsException::wwhat() {
    return message;
}