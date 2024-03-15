#ifndef CHANGE_LOCK_SCREEN_HPP
#define CHANGE_LOCK_SCREEN_HPP

#include "ChangeLockScreenData.hpp"
#include <string_view>
#include <windows.h>

class ChangeLockscreenDaemon;

template <class T>
class BaseChangelockscreenDaemon;

template <class DerivedType>
class BaseChangelockscreenDaemon
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

    static inline ChangeLockscreenDaemon *GetChangeLockScreenDaemon(HWND hwnd)
    {
        LONG_PTR data_ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ChangeLockscreenDaemon *data = reinterpret_cast<ChangeLockscreenDaemon *>(data_ptr);

        return data;
    }

    int WriteNewShuffle(std::fstream&, int);
    bool CopyFile(std::filesystem::path, std::filesystem::path);

    constexpr BaseChangelockscreenDaemon();
    constexpr HWND Windows();
    void Initialize();
    ~BaseChangelockscreenDaemon();

protected:
    virtual const wchar_t *ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND main_hwnd;
    ChangeLockScreenData data;
    BOOL initialized = FALSE;
};

class ChangeLockscreenDaemon : public BaseChangelockscreenDaemon<ChangeLockscreenDaemon>
{
public:
    virtual const wchar_t *ClassName() const;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void changeLockscreen();
};

#include "ChangeLockScreenDaemon.cpp"

#endif