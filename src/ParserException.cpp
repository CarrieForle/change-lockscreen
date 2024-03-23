#include "ParserException.hpp"
#include <cstring>

ParserException::ParserException(const char *message)
{
    std::strcpy(this->message, message);
}

const char *ParserException::what()
{
    return message;
}