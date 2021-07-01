#include "scanner.h"
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <string>
#include <unordered_map>

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
        printf("[error] scanner: can't read file %s", path);
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

std::string scanner::next_identifier()
{
    std::string result;
    char cur_char;
    while (true)
    {

        cur_char = current();
        if (isalpha(cur_char) || isdigit(cur_char) || cur_char == '_')
        {
            result += cur_char;
            advance();
        }
        else
        {
            return result;
        }
    }
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
        return token("+", token_type::PLUS, current());

    case '-':
        advance();
        return token("-", token_type::MINUS, current());
    case '=':
        advance();
        return token("=", token_type::EQUAL, current());

    case '*':
        advance();
        return token("*", token_type::STAR, current());

    case ',':
        advance();
        return token(",", token_type::COMMA, current());

    case '/':
        advance();
        return token("/", token_type::SLASH, current());

    case ';':
        advance();
        return token(";", token_type::SEMICOLON, current());
    case '(':
        advance();
        return token("(", token_type::PARENTHESIS_OPEN, current());
    case ')':
        advance();
        return token(")", token_type::PARENTHESIS_CLOSE, current());
    case '{':
        advance();
        return token("{", token_type::CURLY_START, current());
    case '}':
        advance();
        return token("}", token_type::CURLY_END, current());
    case -1:
        advance();
        return token("end of stream", token_type::END_OF_STREAM, current());

    default:
        if (isdigit(v))
        {
            return token(std::to_string(next_int()) + "(int)", token_type::INT_VALUE, current());
        }
        else if (isalpha(v) || '_' == v)
        {
            // return token(token_type::KEYWORD, v);
            return token(next_identifier(), token_type::KEYWORD, current());
        }
        else
        {
            printf("error: unrecognised token: %c (%i) \n", v, v);
            print_error_file_information();
            exit(-2);
        }
        break;
    }
}

void scanner::print_error_file_information()
{
    printf("on line: %zu ", line() + 1);
    printf("offset: %zu \n", line_character());
    std::string line_str = "";
    line_str += std::to_string(line() + 1);
    line_str += " | ";

    printf("%s%s \n", line_str.c_str(), get_full_line().c_str());
    for (int i = 0; i < line_character() + line_str.length(); i++)
    {
        printf(" ");
    }
    printf("\033[1;31m↑\033[0m\n");
}
