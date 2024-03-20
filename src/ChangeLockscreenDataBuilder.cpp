#include "ChangeLockscreenDataBuilder.hpp"

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildLastFile(const std::filesystem::path &last_file)
{
    this->last_file = last_file;
    flags |= 1;
    return *this;
}

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildCurrentFile(const std::wstring &current_file)
{
    flags |= 1 << 1;
    this->current_file = current_file;
    return *this;
}

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildRoot(const std::filesystem::path &root)
{
    flags |= 1 << 2;
    this->root = root;
    return *this;
}

ChangeLockscreenData ChangeLockscreenDataBuilder::build()
{
    if (flags & 1)
    {
        this->last_file = L"last_file.txt";
    }
    if (flags & 1 << 1)
    {
        this->current_file = L"current_file.txt";
    }
    if (flags & 1 << 2)
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
                MB_OK);
            PostQuitMessage(ErrorChangeLockscreen::retrieve_home_path);
        }

        this->root = std::filesystem::path(home_path) / R"(Pictures/slideshow_lockscreen)";
    }
    return ChangeLockscreenData(last_file, current_file, root);
}