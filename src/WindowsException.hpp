#ifndef WINDOWS_EXCEPTION_HPP
#define WINDOWS_EXCEPTION_HPP

#include <cstdlib>
#include <iostream>
#include <exception>
#include <windows.h>

class WindowsException : public std::exception
{
public:
    static constexpr void throwIf(const bool condition)
    {
        if (condition)
        {
            throw WindowsException(GetLastError());
        }
    }

    static constexpr void throwIfNot(const bool condition)
    {
        WindowsException::throwIf(!condition);
    }
    WindowsException(const DWORD error)
    {
        if (!FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                error,
                0,
                (wchar_t *)&wmessage,
                0, NULL))
        {
            wcscpy(wmessage, L"Failed to retrieve error.");
            strcpy(message, "Failed to retrieve error.");
        }
        else {
            size_t size = std::wcstombs(message, wmessage, 512);
            if (size == 512) {
                message[511] = 0;
            }
        }
    }

    virtual constexpr ~WindowsException() { LocalFree(wmessage); }

    // message is truncated to 511 characters of wmessage. Use wwhat() if possible.
    const char *what() const noexcept override { return message; }
    constexpr wchar_t *wwhat() { return wmessage; }

private:
    DWORD error;
    char message[512];
    wchar_t *wmessage;
};

#endif