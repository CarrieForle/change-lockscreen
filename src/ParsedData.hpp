#ifndef PARSED_DATA_HPP
#define PARSED_DATA_HPP

#include <filesystem>
#include <optional>

struct ParsedData {
    std::optional<std::filesystem::path> cwd;
    std::optional<std::filesystem::path> lockscreen_images_location;
    std::optional<bool> is_change_lockscreen_on_startup;
};

#endif