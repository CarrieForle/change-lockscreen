#ifndef CHANGE_LOCK_SCREEN_HPP
#define CHANGE_LOCK_SCREEN_HPP

#include "ChangeLockScreenData.hpp"
#include <string_view>
#include <windows.h>

class ChangeLockScreenDaemon;

template <class T>
class BaseChangeLockScreenDaemon;

template <class DerivedType>
class BaseChangeLockScreenDaemon
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL Create(const wchar_t *lpWindowName,
                DWORD dwExStyle = 0,
                int x = CW_USEDEFAULT,
                int y = CW_USEDEFAULT,
                int nWidth = CW_USEDEFAULT,
                int nHeight = CW_USEDEFAULT,
                HWND hWndParent = NULL,
                HMENU hMenu = NULL);

    static inline ChangeLockScreenDaemon *GetChangeLockScreenDaemon(HWND hwnd)
    {
        LONG_PTR data_ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ChangeLockScreenDaemon *data = reinterpret_cast<ChangeLockScreenDaemon *>(data_ptr);

        return data;
    }

    int WriteNewShuffle(std::fstream&, int);
    bool CopyFile(std::filesystem::path, std::filesystem::path);

    constexpr BaseChangeLockScreenDaemon();
    constexpr HWND Windows();
    void Initialize();
    ~BaseChangeLockScreenDaemon();

protected:
    virtual const wchar_t *ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND main_hwnd;
    ChangeLockScreenData data;
    BOOL initialized = FALSE;
};

class ChangeLockScreenDaemon : public BaseChangeLockScreenDaemon<ChangeLockScreenDaemon>
{
public:
    virtual const wchar_t *ClassName() const;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void changeLockscreen();
};

#include "ChangeLockScreenDaemon.cpp"

#endif