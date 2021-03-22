#ifndef B6BCD645_0842_4B2E_B8FD_1E8527588556
#define B6BCD645_0842_4B2E_B8FD_1E8527588556
#include <fstream>
#include <memory>
#include "defs.h"
#include <vector>

class scanner{
    const char* _file_path;
    std::vector<char> _buffer;
    size_t _cursor;
public:
    scanner();
    size_t cur() const {return _cursor;};
    int open(const char* path);

    int advance();
    int current();
    int eat_current();
    int skip_space();
    token next_token();
    long next_int();
    bool eof() const {return _cursor > (_buffer.size()-1);};
};

#endif /* B6BCD645_0842_4B2E_B8FD_1E8527588556 */
