/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)MIT
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2022
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * MIT License
 *
 * Copyright (c) 2022 Alexander Sokoloff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <array>
#include <cstdint>

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
    // virtual ~OutFile();

    uint32_t tellp() const { return mPos; }
    bool     good() const { return mStream.good(); }
    void     flush();

    OutFile &operator<<(char value);
    OutFile &operator<<(uint16_t value);
    OutFile &operator<<(uint32_t value);

    OutFile &operator<<(const std::array<char, 4> &value);
    OutFile &operator<<(const Bytes &value);
    OutFile &operator<<(const char value[5]);

    void write(const unsigned char *data, std::streamsize size);

private:
    std::ofstream mFile;
    std::ostream &mStream;
    uint32_t      mPos = 0;
};

bool iequals(const std::string &a, const std::string &b);

enum class FileType {
    Unknown = 0,
    JPEG,
    PNG,
    BMP,
    GIF,
};

FileType determineFileType(const std::string &fileName) noexcept(false);

#endif // TYPES_H
