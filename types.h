#ifndef TYPES_H
#define TYPES_H

#include <string>

class Error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

#endif // TYPES_H
