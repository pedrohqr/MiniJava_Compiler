#include "String.h"

/* Return the length of a string */
inline 
size_t 
strlen(const char* const s)
{
    size_t i = 0;
    while(s[i] != '\0')
        i++;
    return i;
}

/* Copy a source string to a destination string. */
inline 
void 
strcpy(char* dest, const char* source)
{
    size_t i = 0;
    while(source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
}

/* Default constructor. */
my::String::String(): _size{}
{
    _data = new char;
    _data[0] = '\0';
}

/* Literal constructor. */
my::String::String(const char* const str)
{
    try
    {        
        if ((_size = strlen(str)) <= MaxStringSize)
        {
            _data = new char[_size + 1];
            strcpy(_data, str);
        }
        else
            throw MaxStringSize;
    }
    catch(size_t m)
    {
        _size = 0;
        std::cerr << "Size of the string given is bigger than max allow(" << m << ")." << '\n';
    }    
}

/* Copy constructor. */
my::String::String(const String& other)
{
    if ((_size = other._size) > 0)
    {
        _data = new char[_size + 1];
        strcpy(_data, other._data);
    }
}

/* Move constructor. */
my::String::String(my::String&& other) noexcept
{
    _data = other._data;
    _size = other._size;

    other._data = nullptr;
    other._size = 0;
}

/* Default destructor. */
my::String::~String()
{
    clear();
}

/* Return a reference for index i, to read a value in given index. */
char 
my::String::operator[](const size_t i) const
{
    try
    {
        if(i > _size)
            throw;

        return _data[i];
    }
    catch(...)
    {
        std::cerr << "Invalid index access to read(" << i << ").\n";
        return 0;
    }
}

/* Return a reference for index i, to write a value in given index. */
char& 
my::String::operator[](const size_t i)
{
    try
    {
        if(i > _size)
            throw;

        return *(_data+i);
    }
    catch(...)
    {
        std::cerr << "Invalid index access to write(" << i << ").\n";
        return *_data;
    }
}

/* Return the length of string. */
size_t 
my::String::length()
{
    return _size;
}

/* Copy assignment operator. */
my::String& 
my::String::operator=(const my::String& other)
{
    // If are not in the same address
    if (this != &other)
    {
        // If are not the same size
        if (_size != other._size)
        {
            delete[] _data;
            _data = (_size = other._size) > 0 ? new char[_size + 1] : nullptr;
        }

        if (_data)
            strcpy(_data, other._data);
    }

    return *this;
}

/* Move assignment operator. */
my::String& 
my::String::operator=(my::String&& other) noexcept
{    
    my::String::operator=(other);

    other._size = 0;
    delete[] other._data;
    
    return *this;
}

/* Concatenation of strings. */
my::String& 
my::String::operator+=(const my::String& other)
{
    try
    {        
        if ((_size + other._size) <= MaxStringSize)
        {
            size_t oldSize = _size;
            resize(_size + other._size);
            for (size_t i = oldSize; i < oldSize + other._size; i++)
                _data[i] = other._data[i-oldSize];
            return *this;
        }
        else
            throw;
    }
    catch(...)
    {
        std::cerr << "Cannot concat string because the resultant size is greater than max size of a string.\n";
        return *this;
    }
}

/* Concatenation of the string and a char. */
my::String& 
my::String::operator+=(const char& c)
{
    append(c);
    return *this;
}

/* Resize the data array with a new size. */
void 
my::String::resize(const size_t newSize)
{
    try
    {
        if (newSize > MaxStringSize)
            throw std::string("New size of resize function must be less equal of a max string size.");
        if (_size < newSize)
        {
            char* newData = new char[newSize+1];
            newData[newSize] = '\0';
            
            strcpy(newData, _data);

            if (_data != nullptr)
                clear();
            _data = newData;
            _size = newSize;
        }
        else if (_size > newSize)
            throw;
    }
    catch(std::string &E)
    {
        std::cerr << E << '\n';
        throw;
    }
    catch(...)
    {
        std::cerr << "New size value for resize must be greater than older.\n";
        throw;
    }
}

/* Clear all content of _data */
inline void my::String::clear()
{
    _size == 0 ? delete _data : delete[] _data;
}

/* Equals operator. */
bool 
my::String::operator==(const my::String& other) const
{
    // If both size are not equal, then false
    if (_size != other._size)
        return false;

    // Compare char by char
    for (size_t i = 0; i < _size; i++)
        if (_data[i] != other._data[i])
            return false;
    
    return true;
}

/* Return true if the string is empty. */
bool 
my::String::isEmpty() const
{
    return _size == 0 ? true : false;
}

/* Expand the string and append char c. */
void 
my::String::append(const char& c)
{
    try
    {
        if (_size <= MaxStringSize)
        {
            resize(_size+1);
            _data[_size-1] = c;
        }
        else
            throw;
    }
    catch(...)
    {
        std::cerr << "Cannot append char. String is in the max size.\n";
        throw;
    }   
}

/* Assignment operator. */
/*my::String&
my::String::operator=(const std::string& other)
{
    if (_size != other.length())
    {
        delete[] _data;
        _data = new char[other.length()+1];
        _size = other.length();
    }
    
    for (size_t i = 0; i < other.length(); i++)
        _data[i] = other[i];

    return *this;
}*/

/* Cast the string to unsigned int. */
unsigned int
my::String::to_uint()
{
    try
    {
        for (size_t i = 0; i < _size; i++)
            if (_data[i] < 48 && _data[i] > 57) // If not a numbers
                throw;


        return std::stoul(_data);
    }
    catch(...)
    {
        std::cerr << "Cannot convert to unsigned integer. This data contains chars that is not a digit.\n";
        return 0;
    }
}