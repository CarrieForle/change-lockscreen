#ifndef CHANGE_LOCKSCREEN_ERROR_CODE_HPP
#define CHANGE_LOCKSCREEN_ERROR_CODE_HPP

enum ErrorChangeLockscreen
{
    read_last_file = 1,
    read_last_file_next_index,
    write_last_file_update,
    write_last_file_new,
    copy_images,
    session_detection
};

#endif