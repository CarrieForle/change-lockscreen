#ifndef LOG_HPP
#define LOG_HPP

#include <type_traits>
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <fstream>

template <class Char>
    requires std::is_same_v<Char, char> || std::is_same_v<Char, wchar_t>
class Log
{
public:
    constexpr Log(const std::basic_ostream<Char> &stream)
        : log_ostream(*stream), is_file_stream(false)
    {
        log_stream = log_ostream;
    }
    constexpr Log(const std::filesystem::path &p)
        : log_ofstream(p), is_file_stream(true)
    {
        log_stream = &log_ofstream;
    }
    virtual ~Log()
    {
        if (is_file_stream)
        {
            // std::basic_ofstream<Char> &stream = static_cast<std::basic_ofstream<Char> &>(log_stream);
            // if (stream.is_open())
            // stream.close();

            log_ofstream.close();
        }
    }

    // TO-DO: use std::print() after available

    template <class... Args>
    void log(bool is_showtime, bool is_add_newline, std::basic_format_string<Char, std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        if (is_showtime)
        {
            *log_stream << std::format(time_format, std::chrono::system_clock::now());
        }

        *log_stream << std::format(fmt, std::forward<Args>(args)...);

        if (is_add_newline)
        {
            *log_stream << '\n';
        }
    }

    template <class... Args>
    void log(bool is_add_newline, std::basic_format_string<Char, std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        log(true, is_add_newline, fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void log(std::basic_format_string<Char, std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        log(true, true, fmt, std::forward<Args>(args)...);
    }

    constexpr void log(bool is_showtime, bool is_add_newline, std::basic_string_view<Char> str)
    {
        if (is_showtime)
        {
            // *log_stream << std::format(time_format, std::chrono::system_clock::now());
        }

        *log_stream << str;

        if (is_add_newline)
        {
            *log_stream << '\n';
        }
    }

    void log(bool is_add_newline, std::basic_string_view<Char> str)
    {
        log(true, is_add_newline, str);
    }

    void log(std::basic_string_view<Char> str)
    {
        log(true, true, str);
    }

    constexpr Char *GetCharPointer() const
    {
        Char *ptr;
        std::basic_streambuf<Char> *buf = log_stream->rdbuf();
        buf->sgetn(ptr, buf->in_avail());
        return ptr;
    }

    constexpr std::basic_streambuf<Char> *rdbuf() const noexcept
    {
        return log_stream->rdbuf();
    }

    constexpr const std::basic_ostream<Char> &GetStream() const noexcept { return *log_stream; }

private:
    const char* time_format = "[{0:%H}:{0:%M}:{0:%S}] ";
    std::basic_ofstream<Char> *log_stream = nullptr;
    std::basic_ostream<Char> *log_ostream = nullptr;
    std::basic_ofstream<Char> log_ofstream;
    bool is_file_stream;
};

#endif