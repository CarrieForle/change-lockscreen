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
    ChangeLockscreenData(ChangeLockscreenData &) = default;
    virtual ~ChangeLockscreenData() = default;

    ChangeLockscreenData(const std::filesystem::path &last_file,
                         const std::wstring &current_file,
                         const std::filesystem::path &root) : has_locked(false),
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

    friend void swap(ChangeLockscreenData &first, ChangeLockscreenData &second)
    {
        using std::swap;

        swap(first.files, second.files);
        swap(first.random_gen, second.random_gen);
        swap(first.has_locked, second.has_locked);
        swap(first.root, second.root);
        swap(first.last_file, second.last_file);
        swap(first.ext, second.ext);
        swap(first.current_file, second.current_file);
    }

    ChangeLockscreenData(ChangeLockscreenData&& other): ChangeLockscreenData() {
        swap(*this, other);
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