#ifndef _STRING_H
#define _STRING_H

#include <cassert>
#include <string>
#include <iostream>

namespace my
{

constexpr size_t MaxStringSize = 18;

class String
{
private:
    size_t _size;
    char* _data;

    void resize(const size_t);
    inline void clear();
public:

    String();
    String(const char* const);
    String(const String&);
    String(String&&) noexcept;
    ~String();

    String& operator=(const String&);
    String& operator=(String&&) noexcept;
    char operator[](const size_t) const;
    char& operator[](const size_t);
    String& operator+=(const String&);
    String& operator+=(const char&);
    bool operator==(const String&) const;
    //String& operator=(const std::string&);

    size_t length();
    bool isEmpty() const;
    void append(const char&);
    unsigned int to_uint();

    friend std::ostream& operator<<(std::ostream& os, const String& other)
    {
        for (size_t i = 0; i < other._size; i++)
            os << other._data[i];
        
        return os;
    }
};

}

#endif