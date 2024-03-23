#ifndef CHANGE_LOCKSCREEN_DATA_HPP
#define CHANGE_LOCKSCREEN_DATA_HPP

#include "ErrorMessageBox.hpp"
#include "ErrorChangeLockscreen.hpp"
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
    std::filesystem::path root;
    std::filesystem::path last_file;
    std::wstring ext;
    std::filesystem::path current_file;

    ChangeLockscreenData() : has_locked(false),
                             random_gen(std::random_device{}()),
                             last_file(L"last_file.txt"),
                             ext(L"png"),
                             current_file(L"current." + ext)

    {

        // wchar_t *home_path;
        // GetEnvironmentVariable(L"HOMEPATH", home_path, 256);

        char *home_path = std::getenv("HOMEPATH");
          if (home_path == NULL)
        {
            MessageBox(
                NULL,
                L"Failed to retrieve \"HOMEPATH\" environment variable. The program will exit.",
                ErrorMessageBox::universal,
                MB_OK
            );
            PostQuitMessage(ErrorChangeLockscreen::retrieve_home_path);
        }

        root = std::filesystem::path(home_path) / R"(Pictures/slideshow_lockscreen)";

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