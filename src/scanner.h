#ifndef B6BCD645_0842_4B2E_B8FD_1E8527588556
#define B6BCD645_0842_4B2E_B8FD_1E8527588556
#include "defs.h"
#include <fstream>
#include <memory>
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


    std::string get_full_line();

    int advance();
    int current();
    int eat_current();
    int skip_space();
    token next_token();
    long next_int();
    bool eof() const { return _cursor > (_buffer.size() - 1); };
};

#endif /* B6BCD645_0842_4B2E_B8FD_1E8527588556 */
