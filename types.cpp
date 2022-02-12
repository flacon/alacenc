#include "types.h"
#include <iostream>
#include <cstring>

/************************************************
 * ALACEncoder::Bytes
 ************************************************/
Bytes &operator<<(Bytes &out, char value)
{
    out.push_back(value);
    return out;
}

Bytes &operator<<(Bytes &out, uint16_t value)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    out.push_back(char(value >> 8));
    out.push_back(char(value >> 0));
#endif
#if BYTE_ORDER == BIG_ENDIAN
    out.push_back(char(value >> 0));
    out.push_back(char(value >> 8));
#endif
    return out;
}

Bytes &operator<<(Bytes &out, uint32_t value)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    out.push_back(char(value >> 24));
    out.push_back(char(value >> 16));
    out.push_back(char(value >> 8));
    out.push_back(char(value >> 0));
#endif
#if BYTE_ORDER == BIG_ENDIAN
    out.push_back(char(value >> 0));
    out.push_back(char(value >> 8));
    out.push_back(char(value >> 16));
    out.push_back(char(value >> 24));
#endif
    return out;
}

Bytes &operator<<(Bytes &out, const char val[4])
{
    out.push_back(val[0]);
    out.push_back(val[1]);
    out.push_back(val[2]);
    out.push_back(val[3]);
    return out;
}

Bytes &operator<<(Bytes &out, const Bytes &value)
{
    out.reserve(out.size() + value.size());
    for (const char &c : value) {
        out.push_back(c);
    }
    return out;
}

Bytes &operator<<(Bytes &out, const std::string &value)
{
    out.reserve(out.size() + value.size());
    for (const char &c : value) {
        out.push_back(c);
    }
    return out;
}

OutFile::OutFile() :
    mStream(std::cout)
{
}

OutFile::OutFile(const std::string &fileName) :
    mFile(fileName),
    mStream(mFile)
{
}

void OutFile::flush()
{
    mStream.flush();
}

OutFile &OutFile::operator<<(char value)
{
    mStream << value;
    mPos += 1;
    return *this;
}

#if BYTE_ORDER == LITTLE_ENDIAN
OutFile &OutFile::operator<<(uint16_t value)
{
    mStream.put(char(value >> 8));
    mStream.put(char(value >> 0));
    mPos += 2;
    return *this;
}

OutFile &OutFile::operator<<(uint32_t value)
{
    mStream.put(char(value >> 24));
    mStream.put(char(value >> 16));
    mStream.put(char(value >> 8));
    mStream.put(char(value >> 0));
    mPos += 4;
    return *this;
}

#endif

#if BYTE_ORDER == BIG_ENDIAN
OutFile &OutFile::operator<<(uint16_t value)
{
    mStream.put((value >> 0));
    mStream.put(char(value >> 8));
    mPos += 2;
    return *this;
}

OutFile &OutFile::operator<<(uint32_t value)
{
    mStream.put(char(value >> 0));
    mStream.put(char(value >> 8));
    mStream.put(char(value >> 16));
    mStream.put(char(value >> 24));
    mPos += 4;
    return *this;
}

#endif

OutFile &OutFile::operator<<(const std::array<char, 4> &value)
{
    mStream.write(value.data(), value.size());
    mPos += value.size();
    return *this;
}

OutFile &OutFile::operator<<(const Bytes &value)
{
    mStream.write(value.data(), value.size());
    mPos += value.size();
    return *this;
}

OutFile &OutFile::operator<<(const char value[5])
{
    mStream.put(value[0]);
    mStream.put(value[1]);
    mStream.put(value[2]);
    mStream.put(value[3]);
    mPos += 4;
    return *this;
}

void OutFile::write(unsigned char *data, std::streamsize size)
{
    mStream.write((char *)(data), size);
    mPos += size;
}

bool iequals(const std::string &a, const std::string &b)
{
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

/************************************************
 *  See https://en.wikipedia.org/wiki/List_of_file_signatures
 ************************************************/
FileType determineFileType(const std::string &fileName) noexcept(false)
{
    auto file = std::ifstream(fileName);
    if (file.fail()) {
        throw Error(fileName + ": " + strerror(errno));
    }

    char buf[12] = { 0 };
    file.read(buf, 12);

    if (strncmp(buf, "\xFF\xD8\xFF\xDB", 4) == 0) {
        return FileType::JPEG;
    }

    if (strncmp(buf, "\xFF\xD8\xFF\xEE", 4) == 0) {
        return FileType::JPEG;
    }

    if (strncmp(buf, "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46\x49\x46\x00\x01", 12) == 0) {
        return FileType::JPEG;
    }

    if (strncmp(buf, "\xFF\xD8\xFF\xE1", 4) == 0 && strncmp(buf + 6, "\x45\x78\x69\x66\x00\x00", 6)) {
        return FileType::JPEG;
    }

    if (strncmp(buf, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 4) == 0) {
        return FileType::PNG;
    }

    if (strncmp(buf, "\x42\x4D", 2) == 0) {
        return FileType::BMP;
    }

    if (strncmp(buf, "\x47\x49\x46\x38\x37\x61", 6) == 0) {
        return FileType::GIF;
    }

    if (strncmp(buf, "\x47\x49\x46\x38\x39\x61", 6) == 0) {
        return FileType::GIF;
    }

    throw Error(fileName + ": Unsupported file type");
}
