#ifndef CHANGE_LOCKSCREEN_HPP
#define CHANGE_LOCKSCREEN_HPP

#include "ChangeLockscreenDataBuilder.hpp"
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
    bool create(const wchar_t *lpWindowName,
                DWORD dwExStyle = 0,
                int x = 0,
                int y = 0,
                int nWidth = 1,
                int nHeight = 1,
                HWND hWndParent = NULL);

    static inline ChangeLockscreenDaemon *getChangeLockScreenDaemon(HWND hwnd)
    {
        LONG_PTR data_ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ChangeLockscreenDaemon *data = reinterpret_cast<ChangeLockscreenDaemon *>(data_ptr);

        return data;
    }

    int writeNewShuffle(std::fstream&, int);
    bool copyFile(std::filesystem::path, std::filesystem::path);

    BaseChangelockscreenDaemon() = default;
    constexpr HWND windows();
    void initialize();
    virtual ~BaseChangelockscreenDaemon();
    Log logger{"log.txt"};

protected:
    virtual const wchar_t *className() const = 0;
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND main_hwnd = NULL;
    HMENU tray_menu = CreatePopupMenu();
    ChangeLockscreenData data = ChangeLockscreenDataBuilder{}.build();
};

class ChangeLockscreenDaemon : public BaseChangelockscreenDaemon<ChangeLockscreenDaemon>
{
public:
    virtual const wchar_t *className() const;
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void changeLockscreen();
};

#include "ChangeLockscreenDaemon.cpp"

#endif