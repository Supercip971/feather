#ifndef FLANG_CSTAR_TOKEN_H
#define FLANG_CSTAR_TOKEN_H

#include <stddef.h>
#include <stdio.h>
#include <string>

enum token_type
{
    TOKEN_NULL = 0,

    PLUS = 1,
    MINUS = 2,
    STAR = 3,
    SLASH = 4,
    SEMICOLON = 5,
    INT_VALUE = 6,
    PARENTHESIS_OPEN = 7,
    PARENTHESIS_CLOSE = 8,
    KEYWORD = 9,
    EQUAL = 10,
    END_OF_STREAM = 11,
    COMMA = 12,
    CURLY_START = 13,
    CURLY_END = 14,
};

class token
{

    std::string _raw = "_null_";

    token_type _id = TOKEN_NULL;
    size_t _position = 0;

public:
    token(){};
    token(std::string raw_value, token_type type, size_t position) : _raw(raw_value),
                                                                     _id(type),
                                                                     _position(position){

                                                                     };
    token(char raw_value, token_type type, size_t position) : _id(type),
                                                              _position(position)
    {
        _raw = "";
        _raw += raw_value;
    };
    std::string &raw() { return _raw; };
    token_type type() const { return _id; };
    size_t pos() const { return _position; };

    void debug_print()
    {
        printf("token: %s on %zu type: %i \n", _raw.c_str(), _position, _id);
    }
};


#endif //FLANG_CSTAR_TOKEN_H
