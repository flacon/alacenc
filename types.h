#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <array>

class Error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

// clang-format off
#if BYTE_ORDER == LITTLE_ENDIAN
inline uint8_t  toBigEndian(uint8_t value)  { return value; }
inline uint16_t toBigEndian(uint16_t value) { return __builtin_bswap16(value); }
inline uint32_t toBigEndian(uint32_t value) { return __builtin_bswap32(value); }
inline uint64_t toBigEndian(uint64_t value) { return __builtin_bswap64(value); }
#endif

#if BYTE_ORDER == BIG_ENDIAN
inline uint8_t toBigEndian(uint8_t value)   { return value; }
inline uint16_t toBigEndian(uint16_t value) { return value; }
inline uint32_t toBigEndian(uint32_t value) { return value; }
inline uint64_t toBigEndian(uint64_t value) { return value; }
#endif
// clang-format on

using Bytes = std::vector<char>;
Bytes &operator<<(Bytes &out, char value);
Bytes &operator<<(Bytes &out, uint16_t value);
Bytes &operator<<(Bytes &out, uint32_t value);
Bytes &operator<<(Bytes &out, const char val[4]);
Bytes &operator<<(Bytes &out, const Bytes &value);
Bytes &operator<<(Bytes &out, const std::string &value);

class OutFile
{
public:
    OutFile();
    explicit OutFile(const std::string &fileName);

    uint32_t tellp() const { return mPos; }
    bool     good() const { return mStream.good(); }
    bool     fail() const { return mStream.fail(); }
    void     flush();

    OutFile &operator<<(char value);
    OutFile &operator<<(uint16_t value);
    OutFile &operator<<(uint32_t value);

    OutFile &operator<<(const std::array<char, 4> &value);
    OutFile &operator<<(const Bytes &value);
    OutFile &operator<<(const char value[5]);

    void write(unsigned char *data, std::streamsize size);

private:
    std::ofstream mFile;
    std::ostream &mStream;
    uint32_t      mPos = 0;
};

#endif // TYPES_H
