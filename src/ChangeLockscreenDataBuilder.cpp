#include "ErrorMessageBox.hpp"
#include "ChangeLockscreenDataBuilder.hpp"

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildLastFile(const std::filesystem::path &last_file)
{
    this->last_file = last_file;
    flags |= 1;
    return *this;
}

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildCurrentFile(const std::wstring &current_file)
{
    this->current_file = current_file;
    flags |= 1 << 1;
    return *this;
}

ChangeLockscreenDataBuilder &ChangeLockscreenDataBuilder::buildRoot(const std::filesystem::path &root)
{
    this->root = root;
    flags |= 1 << 2;
    return *this;
}

ChangeLockscreenData ChangeLockscreenDataBuilder::build()
{
    if (~flags & 1)
    {
        this->last_file = L"last_file.txt";
    }

    if (~flags & 1 << 1)
    {
        this->current_file = L"current.";
    }

    if (~flags & 1 << 2)
    {
        // wchar_t *home_path;
        // GetEnviron~mentVariable(L"HOMEPATH", home_path, 256);
        char *home_path = std::getenv("HOMEPATH");

        if (home_path == NULL)
        {
            ErrorMessageBox::errorMessageBox(L"Failed to retrieve \"HOMEPATH\" environment variable. The daemon will be terminated.", MB_OK);
            this->root = L"";
            PostQuitMessage(ErrorChangeLockscreen::retrieve_home_path);
        }

        this->root = std::filesystem::path(home_path) / R"(Pictures/slideshow_lockscreen)";
    }

    return ChangeLockscreenData(last_file, current_file, root);
}