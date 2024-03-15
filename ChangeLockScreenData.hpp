#ifndef CHANGE_LOCKSCREEN_DATA_HPP
#define CHANGE_LOCKSCREEN_DATA_HPP

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <random>
#include <windows.h>
#include <fstream>

struct ChangeLockscreenData
{
    std::vector<std::filesystem::path> files;
    std::mt19937 random_gen;
    bool has_locked;
    HWND main_hwnd;
    HMENU tray_menu;
    UINT alignment;
    std::filesystem::path root;
    std::filesystem::path last_file;
    std::wstring ext;
    std::filesystem::path current_file;

    ChangeLockscreenData() = default;

    ChangeLockscreenData(HWND& hwnd) : main_hwnd(hwnd), has_locked(false), tray_menu(CreatePopupMenu()),
                             random_gen(std::random_device{}()),
                             alignment(GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0 ? TPM_LEFTALIGN : TPM_RIGHTALIGN),
                             last_file(L"last_file.txt"),
                             ext(L"png"),
                             current_file(L"current." + ext)

    {

        char *home_path = std::getenv("HOMEPATH");
        if (home_path == NULL)
        {
            MessageBox(
                NULL,
                L"Failed to retrieve \"HOMEPATH\" environment variable. The program will exit.",
                L"ERROR",
                MB_OK
            );
        }
        else {
            std::wcout << L"\"HOMEPATH\" = " << home_path << L"\n";
        }
        root = std::filesystem::path(home_path) / R"(Picture/slideshow_lockscreen)";

        if (!std::filesystem::exists(root / last_file))
        {
            std::ofstream tmp{root / last_file};
            tmp << -1;
            tmp.close();
        }

        for (const auto &entry : std::filesystem::directory_iterator(root))
        {
            const std::filesystem::path file_path = entry.path();
            if (file_path.wstring().ends_with(ext))
            {
                files.push_back(file_path);
            }
        }
    }
    ChangeLockscreenData(ChangeLockscreenData &) = delete;
};

#endif