#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

class Exception : public std::exception
{
public:
    template <typename... Args> Exception(Args&&... args)
    {
        std::stringstream formatter;
        _append(formatter, args...);
        _message = formatter.str();
    }

    virtual const char* what() const noexcept
    {
        return _message.c_str();
    }

private:
    template <typename T, typename... Args> void _append(std::stringstream& formatter, T&& head, Args&&... tail)
    {
        formatter << head;
        _append(formatter, tail...);
    }

    void _append(std::stringstream& formatter)
    {
        formatter << std::endl;
    }

    std::string _message;
};

#endif // EXCEPTION_H
