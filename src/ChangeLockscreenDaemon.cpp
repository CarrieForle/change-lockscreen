#include "ParsedData.hpp"
#include "TrayMenuItems.hpp"
#include "ErrorMessageBox.hpp"
#include "ErrorChangeLockscreen.hpp"
#include "WindowsException.hpp"
#include "ChangeLockscreenDaemon.hpp"
#include "ChangeLockscreenData.hpp"
#include <windows.h>
#include <wtsapi32.h>
#include <string>
#include <format>
#include <random>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <vector>

template <class T>
LRESULT CALLBACK BaseChangelockscreenDaemon<T>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    T *daemon = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        daemon = reinterpret_cast<T *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)daemon);

        daemon->main_hwnd = hwnd;
    }
    else
    {
        daemon = getChangeLockScreenDaemon(hwnd);
    }

    if (daemon)
    {
        return daemon->handleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

template <class DerivedType>
bool BaseChangelockscreenDaemon<DerivedType>::create(
    const wchar_t *lpWindowName,
    DWORD dwExStyle,
    int x,
    int y,
    int width,
    int height,
    HWND hwnd_parent)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = DerivedType::WindowProc;
    wc.hInstance = GetModuleHandle(NULL); // get the handle of current instance (EXE)
    wc.lpszClassName = className();

    RegisterClass(&wc);

    main_hwnd = CreateWindowEx(
        dwExStyle,
        className(),
        lpWindowName,
        WS_DISABLED,
        x, y, width, height,
        hwnd_parent,
        NULL,
        GetModuleHandle(NULL),
        this);

    if (!main_hwnd)
    {
        const wchar_t *err_msg = L"Failed to build daemon. The program will exit";
        ErrorMessageBox::errorMessageBox(err_msg, MB_OK);
        logger.log(err_msg);

        PostQuitMessage(ErrorChangeLockscreen::build_daemon);

        return false;
    }

    NOTIFYICONDATA notification_data = {};
    notification_data.cbSize = sizeof(notification_data);
    notification_data.hWnd = main_hwnd;
    notification_data.uID = 0;
    notification_data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
    notification_data.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcscpy(notification_data.szTip, L"CF Lockscreen Daemon");
    notification_data.uVersion = NOTIFYICON_VERSION_4;
    notification_data.uCallbackMessage = WM_APP;

    Shell_NotifyIcon(NIM_ADD, &notification_data);
    Shell_NotifyIcon(NIM_SETVERSION, &notification_data);
    Shell_NotifyIcon(NIM_SETFOCUS, &notification_data);

    AppendMenu(
        tray_menu,
        MF_STRING | MF_ENABLED,
        TrayMenuItems::check_next_lockscreen_image,
        L"Cannot obtain next image.");

    AppendMenu(
        tray_menu,
        MF_STRING | MF_ENABLED,
        TrayMenuItems::terminate,
        L"Terminate");

    SetMenu(main_hwnd, tray_menu);

    if (!WTSRegisterSessionNotification(main_hwnd, NOTIFY_FOR_THIS_SESSION))
    {
        const wchar_t *err_msg = L"Failed to set up session disconnection detection. The program will exit.";
        ErrorMessageBox::errorMessageBox(err_msg, MB_OK);
        logger.log(err_msg);

        PostQuitMessage(ErrorChangeLockscreen::session_detection);

        return false;
    }

    return true;
}

template <class T>
BaseChangelockscreenDaemon<T>::~BaseChangelockscreenDaemon()
{
    logger.log(L"Daemon Terminated");
    WTSUnRegisterSessionNotification(main_hwnd);
}

template <class DerivedType>
BaseChangelockscreenDaemon<DerivedType>::BaseChangelockscreenDaemon(const ParsedData pd) : commandline_data(pd){
    ChangeLockscreenDataBuilder builder;

    if (pd.lockscreen_images_location)
        builder.setRoot(pd.lockscreen_images_location.value());

    if (pd.cwd)
        logger.setRoot(pd.cwd.value());

    data = builder.build();
}

template <class T>
constexpr HWND BaseChangelockscreenDaemon<T>::windows() { return main_hwnd; }

ChangeLockscreenDaemon::ChangeLockscreenDaemon(const ParsedData pd): BaseChangelockscreenDaemon<ChangeLockscreenDaemon>(pd) {}

int ChangeLockscreenDaemon::writeNewShuffle(std::fstream &out_stream)
{
    std::vector<int> numbers(data.files.size() + 1);
    numbers.push_back(0);
    std::iota(numbers.begin() + 1, numbers.end(), 0);

    std::shuffle(numbers.begin() + 1, numbers.end(), data.random_gen);

    std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(out_stream, "\n"));

    return numbers[1];
}

bool ChangeLockscreenDaemon::copyFile(const std::filesystem::path from_path, const std::filesystem::path to_path)
{
    std::ifstream in(from_path, std::ios::binary);
    std::ofstream out(to_path, std::ios::binary);

    out << in.rdbuf();

    return out && in;
}

constexpr const wchar_t *ChangeLockscreenDaemon::className() const noexcept { return L"Sample Window Class"; }
LRESULT ChangeLockscreenDaemon::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_WTSSESSION_CHANGE:
        switch (wParam)
        {
        case WTS_SESSION_LOCK:
            logger.log(L"Computer locked");
            data.has_locked = true;

            return 0;
        case WTS_SESSION_UNLOCK:
            if (data.has_locked)
            {
                changeLockscreen();
                logger.log(L"Image changed on unlocked");
                data.has_locked = false;
            }

            return 0;
        default:
            return DefWindowProc(main_hwnd, uMsg, wParam, lParam);
        }

    case WM_ENDSESSION:
        WTSUnRegisterSessionNotification(main_hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_APP:
        SetForegroundWindow(main_hwnd);
        switch (LOWORD(lParam))
        {
        case NIN_SELECT:     // On left click
        case WM_CONTEXTMENU: // On right click
        case NIN_KEYSELECT:
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            switch (TrackPopupMenu(
                tray_menu,
                (GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0 ? TPM_LEFTALIGN : TPM_RIGHTALIGN) | TPM_BOTTOMALIGN |
                    TPM_RETURNCMD | TPM_LEFTBUTTON |
                    TPM_NOANIMATION,
                // GET_X_LPARAM(wParam), GET_Y_LPARAM(wParam),
                (int)(cursorPos.x), (int)(cursorPos.y),
                0,
                main_hwnd,
                NULL))
            {
            case TrayMenuItems::check_next_lockscreen_image:
            {
                ShellExecute(
                    main_hwnd,
                    L"open",
                    (data.root / data.current_file).wstring().c_str(),
                    NULL,
                    NULL,
                    SW_SHOW);
            }
            break;
            case TrayMenuItems::terminate:
                PostQuitMessage(0);
                break;
            case 0:
                return DefWindowProc(main_hwnd, uMsg, wParam, lParam);
            }

            break;
        default:
            return DefWindowProc(main_hwnd, uMsg, wParam, lParam);
        }

        return 0;
    default:
        return DefWindowProc(main_hwnd, uMsg, wParam, lParam);
    }
}


/*
It works by using data.files (vector<std::filesystem::path>),
which stores the paths of image files in root directory.
The index of each path identifies each image files.

The last_file stores 2 things.
1. The index of the current lockscreen file (at the first line of the file)
2. The order of index (which identifies contents of data.files)
   for lockscreen changes (at the remaining lines of the file).
*/

void ChangeLockscreenDaemon::changeLockscreen()
{
    const std::filesystem::path last_number_file_path{data.root / data.last_file};
    std::fstream last_number_file{last_number_file_path, std::ios::in};
    std::stringstream new_contents;

    int rolled_number = 0;
    while (!last_number_file)
    {
        wchar_t err_msg[256];
        std::wcscpy(err_msg, std::format(L"Failed to open \"{}\" for reading. Lockscreen is not changed. Click \"Yes\" to retry; \"No\" to terminate the daemon.", last_number_file_path.wstring()).c_str());
        logger.log(err_msg);

        if (ErrorMessageBox::errorMessageBox(err_msg) == IDYES)
        {
            if (last_number_file.is_open())
            {
                last_number_file.close();
            }

            last_number_file.open(last_number_file_path, std::ios::in);
        }
        else
        {
            PostQuitMessage(ErrorChangeLockscreen::read_last_file);
            return;
        }
    }

    int last_index = -1;
    last_number_file >> last_index;
    new_contents << last_index + 1;
    new_contents << last_number_file.rdbuf();
    last_number_file.seekg(std::ios::beg);

    for (int i = 0; i < last_index + 2; i++)
    {
        last_number_file.ignore(128, '\n');
    }

    if (last_number_file >> rolled_number)
    {
        std::stringstream backup;
        backup << last_number_file.rdbuf();
        last_number_file.close();
        last_number_file.open(last_number_file_path, std::ios::out);

        while (!(last_number_file << new_contents.rdbuf()))
        {
            wchar_t err_msg[256];
            std::wcscpy(err_msg, std::format(L"Failed to update sequence in \"{}\". Lockscreen is not changed. Click \"Yes\" to retry; \"No\" to terminate the daemon.", last_number_file_path.wstring()).c_str());
            logger.log(err_msg);

            if (ErrorMessageBox::errorMessageBox(err_msg) == IDYES)
            {
                if (last_number_file.is_open())
                {
                    last_number_file.close();
                }

                last_number_file.open(last_number_file_path, std::ios::out);
            }
            else
            {
                PostQuitMessage(ErrorChangeLockscreen::write_last_file_update);
                return;
            }
        }

        last_number_file.close();
    }
    else
    {
        last_number_file.ignore(128, '\n');
        if (last_number_file.eof())
        {
            logger.log(L"EOF. Constructing new random sequence.");
            last_number_file.close();
            last_number_file.open(last_number_file_path, std::ios::out);
            rolled_number = writeNewShuffle(last_number_file);

            while (!last_number_file)
            {
                wchar_t err_msg[256];
                std::wcscpy(err_msg, std::format(L"Failed to write new random sequence to \"{}\". Lockscreen is not changed. Click \"Yes\" to retry; \"No\" to terminate the daemon.", last_number_file_path.wstring()).c_str());
                logger.log(err_msg);

                if (ErrorMessageBox::errorMessageBox(err_msg) == IDYES)
                {
                    if (last_number_file.is_open())
                    {
                        last_number_file.close();
                    }
                    last_number_file.open(last_number_file_path, std::ios::out);
                    rolled_number = writeNewShuffle(last_number_file);
                }
                else
                {
                    PostQuitMessage(ErrorChangeLockscreen::copy_images);
                    return;
                }
            }

            last_number_file.close();
        }
        else
        {
            wchar_t err_msg[256];
            std::wcscpy(err_msg, std::format(L"Failed to read next index from \"{}\". Lockscreen is not changed and the daemon will be terminated.", last_number_file_path.wstring()).c_str());

            ErrorMessageBox::errorMessageBox(err_msg, MB_OK);
            logger.log(err_msg);

            PostQuitMessage(ErrorChangeLockscreen::read_last_file_next_index);
            return;
        }
    }

    while (!copyFile(data.files[rolled_number], data.root / data.current_file))
    {
        wchar_t err_msg[256];
        std::wcscpy(err_msg, std::format(L"Failed to copy next image in the sequence to \"{}\". Lockscreen is not changed. Click \"Yes\" to retry; \"No\" to terminate the daemon.", data.current_file.wstring()).c_str());
        logger.log(err_msg);

        if (ErrorMessageBox::errorMessageBox(err_msg) == IDNO)
        {
            PostQuitMessage(ErrorChangeLockscreen::copy_images);
            return;
        }
    }
    ModifyMenu(
        tray_menu,
        TrayMenuItems::check_next_lockscreen_image,
        MF_ENABLED | MF_STRING,
        TrayMenuItems::check_next_lockscreen_image,
        data.files[rolled_number].filename().wstring().c_str());

    logger.log(L"Lockscreen updated to index {}: \"{}\"", rolled_number, data.files[rolled_number].wstring());
}