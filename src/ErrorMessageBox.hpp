#ifndef ERROR_MESSAGE_BOX_HPP
#define ERROR_MESSAGE_BOX_HPP

#include <windows.h>

namespace ErrorMessageBox
{
    static constexpr const wchar_t* universal = L"CF Lockscreen Error";
    int errorMessageBox(const wchar_t *, const unsigned int flags = MB_YESNO);
}

#endif