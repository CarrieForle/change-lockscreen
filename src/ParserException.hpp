#ifndef PARSER_EXCEPTION_HPP
#define PARSER_EXCEPTION_HPP

#include <exception>

class ParserException : public std::exception
{
public:
    ParserException(const char *);
    virtual const char *what();
private:
    char message[256];
};

#endif