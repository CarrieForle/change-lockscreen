#include "ErrorMessageBox.hpp"
#include <winuser.h>

namespace ErrorMessageBox
{
    int errorMessageBox(const wchar_t *error_message, const unsigned int flags)
    {
        return MessageBox(
            NULL,
            error_message,
            universal,
            flags | MB_ICONERROR | MB_TASKMODAL);
    }
}