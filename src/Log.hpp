#ifndef LOG_HPP
#define LOG_HPP

#include <optional>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <fstream>
#include <sstream>

class Log
{
public:
    Log(const std::filesystem::path &p = L"log.txt")
        : log_path(p)
    {
    }

    virtual ~Log()
    {
        std::wofstream out{log_path};
        out << log_stream.rdbuf();
    }

    // TO-DO: use std::print() after available
    template <class... Args>
    void log(std::wformat_string<std::type_identity_t<Args>...> fmt, Args &&...args)
    {
        using namespace std::chrono;
        time_t current_time = system_clock::to_time_t(system_clock::now());

        // TO-DO: Make it std::chrono only when available.
        log_stream << std::put_time(std::localtime(&current_time), time_format);

        log_stream << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }

    constexpr void log(std::wstring_view str)
    {
        using namespace std::chrono;
        time_t current_time = system_clock::to_time_t(system_clock::now());

        // TO-DO: Make it std::chrono only when available.
        log_stream << std::put_time(std::localtime(&current_time), time_format);

        log_stream << str << '\n';
    }

    /*
    std::wstring_view::data() is not neccessary null-terminated,
    */
    constexpr void writeTime(const std::wstring &format)
    {
        using namespace std::chrono;
        time_t current_time = system_clock::to_time_t(system_clock::now());

        // TO-DO: Make it std::chrono only when available.
        log_stream << std::put_time(std::localtime(&current_time), format.c_str());
    }

    template <class T>
    constexpr void write(T &&content)
    {
        log_stream << std::forward<T>(content);
    }

    template <class T>
    constexpr Log &operator<<(T &&content)
    {
        log_stream << std::forward<T>(content);
        return *this;
    }

    constexpr wchar_t *getCharPointer() const
    {
        wchar_t *ptr;
        std::wstreambuf *buf = log_stream.rdbuf();
        buf->sgetn(ptr, buf->in_avail());
        return ptr;
    }

    constexpr std::wstreambuf *rdbuf() const noexcept
    {
        return log_stream.rdbuf();
    }

    constexpr const std::wostream &getStream() const noexcept { return log_stream; }

private:
    static constexpr const wchar_t *time_format = L"[%T] ";
    const std::filesystem::path log_path;
    std::wstringstream log_stream;
};

#endif