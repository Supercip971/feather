//
// Created by root on 5/11/21.
//

#ifndef FLANG_SCANNER_H
#define FLANG_SCANNER_H
#include "cstar_token.h"
#include <vector>
class scanner
{
    const char *_file_path;
    std::vector<char> _buffer;
    size_t _cursor;
    size_t _line;
    size_t _line_char;

public:
    scanner();
    size_t cur() const { return _cursor; };
    int open(const char *path);
    size_t line() const { return _line; };
    size_t line_character() const { return _line_char; };

    void print_error_file_information();

    std::string get_full_line();
    int advance();
    int current();
    int eat_current();
    int skip_space();

    token next_token();
    long next_int();
    std::string next_identifier();

    bool eof() const { return _cursor > (_buffer.size() - 1); };
};

#endif //FLANG_SCANNER_H
