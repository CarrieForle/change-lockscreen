#include "ErrorMessageBox.hpp"
#include <windows.h>

namespace ErrorMessageBox
{
    void errorMessageBox(const wchar_t *error_message)
    {
        MessageBox(
            NULL,
            error_message,
            universal,
            MB_OK);
    }
}