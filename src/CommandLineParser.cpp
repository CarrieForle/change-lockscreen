#include "CommandLineParser.hpp"
#include "ParserException.hpp"
#include <format>
#include <optional>
#include <filesystem>
#include <sstream>
#include <map>

namespace CommandLineParser
{
    // TO-DO: Detect or trim trailing spaces.

    ParsedData Parse(wchar_t *raw_command)
    {
        ParsedData parsed_data;

        const std::map<std::wstring, std::optional<std::filesystem::path> *> arguments{
            {L"cwd", &parsed_data.cwd},
            {L"root", &parsed_data.lockscreen_images_location},
        };

        const std::map<std::wstring, std::optional<bool> *> boolean_arguments{
            {L"startup", &parsed_data.is_change_lockscreen_on_startup}};

        std::wstringstream ss{raw_command};
        std::wstring token;
        void *key = nullptr;

        while (ss >> token)
        {
            bool found_argument = false;
            size_t delimiter_pos = std::wstring::npos;

            for (auto &argument : arguments)
            {
                if (token == std::get<0>(argument))
                {
                    std::wstring value;

                    if (!(ss >> value))
                    {
                        throw ParserException("Failed to parse value in a key-value pair option");
                    }
                    *std::get<1>(argument) = value;

                    found_argument = true;
                    break;
                }
                else if ((delimiter_pos = token.find('=')) != std::wstring::npos)
                {
                    *std::get<1>(argument) = token.substr(delimiter_pos);
                    found_argument = true;
                    break;
                }
            }

            if (found_argument)
                continue;

            for (auto &option : boolean_arguments)
            {
                if (token == std::format(L"--{}", std::get<0>(option)))
                {
                    *std::get<1>(option) = true;
                    found_argument = true;
                    break;
                }
            }

            if (!found_argument)
                throw ParserException("Unknown option");
        }

        return parsed_data;
    }
}