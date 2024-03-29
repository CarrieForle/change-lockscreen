#ifndef ERROR_CHANGE_LOCKSCREEN_HPP
#define ERROR_CHANGE_LOCKSCREEN_HPP

enum ErrorChangeLockscreen
{
    read_last_file = 1,
    read_last_file_next_index,
    write_last_file_update,
    write_last_file_new,
    copy_images,
    session_detection,
    build_daemon,
    retrieve_home_path
};

#endif