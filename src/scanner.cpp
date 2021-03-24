#include "scanner.h"
#include <cctype>
#include <string.h>
scanner::scanner()
{
    _cursor = 0;
    _line_char = 0;
    _line = 0;
}
int scanner::open(const char *path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    _buffer = std::vector<char>(size);
    if (file.read(_buffer.data(), size))
    {
        return 1;
    }
    else
    {
        printf("[error] scanner: can't read file %s");
        exit(-1);
        return 0;
    }
}

int scanner::advance()
{
    if (_cursor + 1 > _buffer.size())
    {
        return -1; // eof
    }
    char result = _buffer[_cursor];

    if (result == '\n')
    {
        _line++;
        _line_char = 0;
    }
    else
    {
        _line_char++;
    }
    _cursor++;
    return result;
}
int scanner::current()
{

    if (_cursor + 1 > _buffer.size())
    {
        return -1; // eof
    }
    return _buffer[_cursor];
}
int scanner::eat_current()
{

    if (_cursor + 1 > _buffer.size())
    {
        return -1; // eof
    }

    char result = _buffer[_cursor];
    _buffer.erase(_buffer.begin() + _cursor);
    return result;
}
int scanner::skip_space()
{
    while (isspace(current()))
    {
        advance();
    }
    return 0;
}

long scanner::next_int()
{
    std::string v = "";
    while (isdigit(current()))
    {
        v += advance();
    }
    return std::stoi(v);
}
std::string scanner::get_full_line()
{
    size_t current_line = 0;
    std::string result = "";
    for (int i = 0; i < _buffer.size(); i++)
    {

        if (_buffer[i] == '\n')
        {
            current_line++;
        }
        if (current_line > _line)
        {
            return result;
        }
        else if (current_line == _line && _buffer[i] != '\n')
        {
            result += _buffer[i];
        }
    }
    return result;
}
token scanner::next_token()
{
    skip_space();
    char v = current();
    switch (v)
    {
    case '+':
        advance();
        return token(token_type::PLUS);

    case '-':
        advance();
        return token(token_type::MINUS);

    case '*':
        advance();
        return token(token_type::STAR);

    case '/':
        advance();
        return token(token_type::SLASH);

    case -1:
        advance();
        return token(token_type::END_OF_STREAM);

    default:
        if (isdigit(v))
        {
            return token(token_type::INT_VALUE, next_int());
        }
        else
        {
            printf("unrecognised token: %c (%i) \n", v, v);
            exit(-2);
        }
        break;
    }
}