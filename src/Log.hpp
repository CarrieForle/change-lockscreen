#ifndef LOG_HPP
#define LOG_HPP

#include <iomanip>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <fstream>

class Log
{
public:
    constexpr Log(std::wostream &stream)
        : log_ostream(&stream), is_file_stream(false)
    {
        log_stream = log_ostream;
    }
    Log(const std::filesystem::path &p)
        : log_ofstream(p), is_file_stream(true)
    {
        log_stream = &log_ofstream;
    }
    virtual ~Log()
    {
        if (is_file_stream)
        {
            log_ofstream.close();
        }
    }

    // TO-DO: use std::print() after available

    template <class... Args>
    void log(bool is_showtime, bool is_add_newline, std::wformat_string<std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        if (is_showtime)
        {
            using namespace std::chrono;
            time_t current_time = system_clock::to_time_t(system_clock::now());

            // TO-DO: Make it std::chrono only when available.
            *log_stream << std::put_time(std::localtime(&current_time), time_format);
        }

        *log_stream << std::format(fmt, std::forward<Args>(args)...);

        if (is_add_newline)
        {
            *log_stream << '\n';
        }
    }

    template <class... Args>
    void log(bool is_add_newline, std::wformat_string<std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        log(true, is_add_newline, fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void log(std::wformat_string<std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        log(true, true, fmt, std::forward<Args>(args)...);
    }

    constexpr void log(bool is_showtime, bool is_add_newline, std::wstring_view str)
    {
        if (is_showtime)
        {
            using namespace std::chrono;
            time_t current_time = system_clock::to_time_t(system_clock::now());

            // TO-DO: Make it std::chrono only when available.
            *log_stream << std::put_time(std::localtime(&current_time), time_format);
        }

        *log_stream << str;

        if (is_add_newline)
        {
            *log_stream << '\n';
        }
    }

    constexpr void log(bool is_add_newline, std::wstring_view str)
    {
        log(true, is_add_newline, str);
    }

    constexpr void log(std::wstring_view str)
    {
        log(true, true, str);
    }

    constexpr wchar_t *GetCharPointer() const
    {
        wchar_t *ptr;
        std::wstreambuf *buf = log_stream->rdbuf();
        buf->sgetn(ptr, buf->in_avail());
        return ptr;
    }

    constexpr std::wstreambuf *rdbuf() const noexcept
    {
        return log_stream->rdbuf();
    }

    constexpr const std::wostream &GetStream() const noexcept { return *log_stream; }

private:
    static constexpr const wchar_t *time_format = L"[%T] ";
    std::wostream *log_stream = nullptr;
    std::wostream *log_ostream = nullptr;
    std::wofstream log_ofstream;
    bool is_file_stream;
};

#endif