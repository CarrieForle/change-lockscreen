#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

#include <exception>
#include <windows.h>

class WindowsException : public std::exception {
public:
    static void ThrowIf(bool);
    static constexpr void ThrowIfNot(bool);
    WindowsException(DWORD);
    ~WindowsException();
    char *what();
    wchar_t *wwhat();

private:
    DWORD error;
    wchar_t *message;
};

#endif