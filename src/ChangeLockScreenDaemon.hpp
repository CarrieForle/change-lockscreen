#ifndef CHANGE_LOCKSCREEN_HPP
#define CHANGE_LOCKSCREEN_HPP

#include <iostream>
#include "Log.hpp"
#include "ChangeLockscreenData.hpp"
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
    bool Create(const wchar_t *lpWindowName,
                DWORD dwExStyle = 0,
                int x = 0,
                int y = 0,
                int nWidth = 1,
                int nHeight = 1,
                HWND hWndParent = NULL);

    static inline ChangeLockscreenDaemon *GetChangeLockScreenDaemon(HWND hwnd)
    {
        LONG_PTR data_ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ChangeLockscreenDaemon *data = reinterpret_cast<ChangeLockscreenDaemon *>(data_ptr);

        return data;
    }

    int WriteNewShuffle(std::fstream&, int);
    bool CopyFile(std::filesystem::path, std::filesystem::path);

    BaseChangelockscreenDaemon() = default;
    constexpr HWND Windows();
    void Initialize();
    virtual ~BaseChangelockscreenDaemon();
    Log<wchar_t> logger{"log.txt"};

protected:
    virtual const wchar_t *ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND main_hwnd = NULL;
    HMENU tray_menu = CreatePopupMenu();
    ChangeLockscreenData data;
};

class ChangeLockscreenDaemon : public BaseChangelockscreenDaemon<ChangeLockscreenDaemon>
{
public:
    virtual const wchar_t *ClassName() const;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void changeLockscreen();
};

#include "ChangeLockscreenDaemon.cpp"

#endif