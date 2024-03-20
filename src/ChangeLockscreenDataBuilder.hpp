#ifndef CHANGE_LOCKSCREEN_DATA_BUILDER_HPP
#define CHANGE_LOCKSCREEN_DATA_BUILDER_HPP

#include "ChangeLockscreenData.hpp"
#include <cstdint>
#include <filesystem>
#include <string>

class ChangeLockscreenDataBuilder
{
private:
    std::filesystem::path last_file;
    std::wstring current_file;
    std::filesystem::path root;
    /*
    0b001 = last_file
    0b010 = current_file
    0b100 = root
    */
    std::int8_t flags = 0;

public:
    ChangeLockscreenDataBuilder &buildLastFile(const std::filesystem::path &);
    ChangeLockscreenDataBuilder &buildCurrentFile(const std::wstring &);
    ChangeLockscreenDataBuilder &buildRoot(const std::filesystem::path &);
    ChangeLockscreenData build();
};

#endif