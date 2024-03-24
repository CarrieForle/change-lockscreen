#ifndef CHANGE_LOCKSCREEN_DATA_HPP
#define CHANGE_LOCKSCREEN_DATA_HPP

#include <utility>
#include "ErrorChangeLockscreen.hpp"
#include <filesystem>
#include <vector>
#include <random>
#include <windows.h>
#include <fstream>

struct ChangeLockscreenData
{
    ChangeLockscreenData() = default;

    ChangeLockscreenData(std::filesystem::path last_file,
                         std::wstring current_file,
                         std::filesystem::path root) : has_locked(false),
                                                              random_gen(std::random_device{}()),
                                                              last_file(last_file),
                                                              ext(L"png"),
                                                              current_file(current_file + ext),
                                                              root(root)

    {
        if (!std::filesystem::exists(root / last_file))
        {
            std::ofstream tmp{root / last_file};
            tmp << -1;
            tmp.close();
        }

        for (const auto &entry : std::filesystem::directory_iterator(root))
        {
            const std::filesystem::path file_path = entry.path();
            const std::wstring file_path_wstring = file_path.wstring();

            if (file_path_wstring != (current_file + ext) && file_path_wstring.ends_with(ext))
            {
                files.push_back(file_path);
            }
        }
    }

    std::vector<std::filesystem::path> files;
    std::mt19937 random_gen;
    bool has_locked;
    std::filesystem::path root;
    std::filesystem::path last_file;
    std::filesystem::path current_file;
    std::wstring ext;
};

#endif