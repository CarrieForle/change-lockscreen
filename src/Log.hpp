#ifndef LOG_HPP
#define LOG_HPP

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <fstream>

template <class Char>
class Log
{
public:
    constexpr Log(const std::basic_ostream<Char> &stream)
        : log_stream(stream.rdbuf()), is_file_stream(false) {}
    constexpr Log(const std::filesystem::path &p)
        : log_stream(std::basic_ofstream<Char>(p)), is_file_stream(true) {}
    virtual ~Log() {
        if (is_file_stream) {
            std::basic_ofstream<Char> &stream = static_cast<std::basic_ofstream<Char>&>(log_stream);
            if (stream.is_open())
                stream.close();
        }
    }

    // TO-DO: use std::print() after available
    template<class ...Args>
    void log(std::basic_format_string<Char, std::type_identity_t<Args>...> fmt, Args&&... args) {
        std::basic_string_view<Char> formatted_string = std::format(fmt, std::forward<Args>(args)...);
        log_stream << formatted_string;
    }

    void log(std::basic_string_view<Char> str) {
        log_stream << str;
    }

    constexpr Char* GetCharPointer() const {
        Char *ptr;
        std::basic_streambuf<Char>* buf = log_stream.rdbuf();
        buf->sgetn(ptr, buf->in_avail());
        return ptr;
    }

    constexpr std::basic_streambuf<Char>* rdbuf() const noexcept {
        return log_stream.rdbuf();
    }

    constexpr const std::basic_ostream<Char>& GetStream() const noexcept {return log_stream;}

private:
    std::basic_ostream<Char> log_stream;
    bool is_file_stream;
};

#endif