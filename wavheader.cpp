/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2012-2013
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "wavheader.h"
#include "types.h"
#include <iostream>
#include <array>

static const char *WAV_RIFF = "RIFF";
static const char *WAV_WAVE = "WAVE";
static const char *WAV_FMT  = "fmt ";
static const char *WAV_DATA = "data";

static const char                   *WAVE64_RIFF      = "riff";
static const char                   *WAVE64_WAVE      = "wave";
static const std::array<uint8_t, 16> WAVE64_GUID_RIFF = { 0x72, 0x69, 0x66, 0x66, 0x2E, 0x91, 0xCF, 0x11, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00 };
static const std::array<uint8_t, 16> WAVE64_GUID_WAVE = { 0x77, 0x61, 0x76, 0x65, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A };
static const std::array<uint8_t, 16> WAVE64_GUID_FMT  = { 0x66, 0x6D, 0x74, 0x20, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A };
static const std::array<uint8_t, 16> WAVE64_GUID_DATA = { 0x64, 0x61, 0x74, 0x61, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A };

// 16 bytes of GUID + 8 bytes of Int64
static constexpr int WAVE64_CHUNK_HEADER_SIZE = 16 + 8;

// static const int READ_DELAY = 1000;

#if 'ABCD' == 0x41424344
// little-endian target architecture

static inline uint16_t fromLittleEndian(uint16_t value)
{
    return value;
}

static inline uint32_t fromLittleEndian(uint32_t value)
{
    return value;
}

static inline uint64_t fromLittleEndian(uint64_t value)
{
    return value;
}

static inline uint16_t toLittleEndian(uint16_t value)
{
    return value;
}

static inline uint32_t toLittleEndian(uint32_t value)
{
    return value;
}

static inline uint64_t toLittleEndian(uint64_t value)
{
    return value;
}

#else
// big-endian target architecture

static inline uint16_t fromLittleEndian(uint16_t value)
{
    return uint16_t(0 | ((value & 0x00ff) << 8) | ((value & 0xff00) >> 8));
}

static inline uint32_t fromLittleEndian(uint32_t value)
{
    return 0
            | ((value & 0x000000ff) << 24)
            | ((value & 0x0000ff00) << 8)
            | ((value & 0x00ff0000) >> 8)
            | ((value & 0xff000000) >> 24);
}

static inline uint64_t fromLittleEndian(uint64_t value)
{
    return 0
            | ((value & uint64_t(0x00000000000000ff)) << 56)
            | ((value & uint64_t(0x000000000000ff00)) << 40)
            | ((value & uint64_t(0x0000000000ff0000)) << 24)
            | ((value & uint64_t(0x00000000ff000000)) << 8)
            | ((value & uint64_t(0x000000ff00000000)) >> 8)
            | ((value & uint64_t(0x0000ff0000000000)) >> 24)
            | ((value & uint64_t(0x00ff000000000000)) >> 40)
            | ((value & uint64_t(0xff00000000000000)) >> 56);
}

#endif

/************************************************
 *
 ************************************************/
static inline void mustRead(std::istream *stream, char *data, size_t size)
{
    if (!stream->read(data, size)) {
        throw WavHeaderError("Unexpected end of file on " + std::to_string(stream->tellg()));
    }
}

/************************************************
 *
 ************************************************/
ByteArray &operator<<(ByteArray &out, const char val[4])
{
    out.push_back(val[0]);
    out.push_back(val[1]);
    out.push_back(val[2]);
    out.push_back(val[3]);
    return out;
}

/************************************************
 *
 ************************************************/
ByteArray &operator<<(ByteArray &out, ByteArray val)
{
    for (const uint8_t &b : val) {
        out.push_back(b);
    }

    return out;
}

/************************************************
 *
 ************************************************/
ByteArray &operator<<(ByteArray &out, uint64_t val)
{
    union {
        uint64_t n;
        char     bytes[8];
    };

    n = toLittleEndian(val);
    out.push_back(bytes[0]);
    out.push_back(bytes[1]);
    out.push_back(bytes[2]);
    out.push_back(bytes[3]);
    out.push_back(bytes[4]);
    out.push_back(bytes[5]);
    out.push_back(bytes[6]);
    out.push_back(bytes[7]);

    return out;
}

/************************************************
 *
 ************************************************/
ByteArray &operator<<(ByteArray &out, uint32_t val)
{
    union {
        uint32_t n;
        char     bytes[4];
    };

    n = toLittleEndian(val);
    out.push_back(bytes[0]);
    out.push_back(bytes[1]);
    out.push_back(bytes[2]);
    out.push_back(bytes[3]);

    return out;
}

/************************************************
 *
 ************************************************/
ByteArray &operator<<(ByteArray &out, uint16_t val)
{
    union {
        uint32_t n;
        char     bytes[2];
    };

    n = toLittleEndian(val);
    out.push_back(bytes[0]);
    out.push_back(bytes[1]);

    return out;
}

/************************************************
 *
 ************************************************/
static ByteArray &operator<<(ByteArray &out, const std::array<uint8_t, 16> &val)
{
    for (const uint8_t &b : val) {
        out.push_back(b);
    }

    return out;
}

/************************************************
 *
 ************************************************/
// struct SplitterError
//{
//     int     trackNum;
//     QString msg;

//    SplitterError(int num, QString msg) :
//        trackNum(num),
//        msg(msg)
//    {
//    }
//};

/************************************************
 *
 ************************************************/
static ByteArray readBytes(std::istream *stream, size_t size)
{
    ByteArray res(size);
    mustRead(stream, (char *)(res.data()), size);
    return res;
}

/************************************************
 *
 ************************************************/
static uint64_t readUInt64(std::istream *stream)
{
    uint64_t n;
    if (!stream->read((char *)&n, 8)) {

        throw WavHeaderError("Unexpected end of file");
    }
    return fromLittleEndian(n);
}

/************************************************
 *
 ************************************************/
static uint32_t readUInt32(std::istream *stream)
{
    uint32_t n;
    if (!stream->read((char *)&n, 4))
        throw WavHeaderError("Unexpected end of file");
    return fromLittleEndian(n);
}

/************************************************
 *
 ************************************************/
static uint16_t readUInt16(std::istream *stream)
{
    uint16_t n;
    if (!stream->read((char *)&n, 2))
        throw WavHeaderError("Unexpected end of file");
    return fromLittleEndian(n);
}

/************************************************
 *
 ************************************************/
class FourCC : public std::array<char, 4>
{
public:
    FourCC() :
        std::array<char, 4>({ '\0' }) { }

    inline void load(std::istream *stream) { return mustRead(stream, this->data(), this->size()); }
    inline bool operator==(const char *str) const { return strncmp(data(), str, size()) == 0; }
    inline bool operator!=(const char *str) const { return !this->operator==(str); }
};

ByteArray &operator<<(ByteArray &out, const FourCC &val)
{
    out.insert(out.end(), val.begin(), val.end());
    return out;
}

/************************************************
 *
 ************************************************/
class Guid : public std::array<char, 16>
{
public:
    static constexpr int SIZE = 16;
    Guid() :
        std::array<char, SIZE>({ '\0' }) { }

    inline void load(std::istream *stream) { return mustRead(stream, this->data(), this->size()); }
    inline bool operator==(const char *str) const { return strncmp(data(), str, size()) == 0; }
    inline bool operator!=(const char *str) const { return !this->operator==(str); }
    inline bool startsWidth(const char str[4]) const { return strncmp(data(), str, 4) == 0; }
};

ByteArray &operator<<(ByteArray &out, const Guid &val)
{
    out.insert(out.end(), val.begin(), val.end());
    return out;
}

/************************************************
 * See WAV specoification
 *   http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 *   https://en.wikipedia.org/wiki/WAV
 ************************************************/
WavHeader::WavHeader(std::istream *stream) noexcept(false)
{
    char tag[] = "\0\0\0\0";

    mustRead(stream, tag, 4);

    if (strcmp(tag, WAV_RIFF) == 0) {
        m64Bit = false;
        readWavHeader(stream);

        return;
    }

    if (strcmp(tag, WAVE64_RIFF) == 0) {
        readBytes(stream, 12); // Wave64 format uses 128-bit GUIDs, we readed 4 bytes, there are still 12 bytes
        m64Bit = true;
        readWave64Header(stream);
        return;
    }

    throw WavHeaderError("WAVE header is missing RIFF tag while processing file");
}

/************************************************
 * 52 49 46 46      RIFF
 * 24 B9 4D 02      file size - 8
 * 57 41 56 45      WAVE
 *
 * // Chanks
 *   66 6D 74 20    SubchunkID      "fmt "
 *   10 00 00 00    SubchunkSize        16
 *         01 00      AudioFormat      PCM
 *         02 00	  NumChannels        2
 *   44 AC 00 00      SampleRate     44100
 *   10 B1 02 00      ByteRate      176400
 *         04 00      BlockAlign         4
 *         10 00      BitsPerSample     16
 * // Data
 *   64 61 74 61 	SubchunkID 		"data"
 *   00 B9 4D 02 	SubchunkSize
 ************************************************/
void WavHeader::readWavHeader(std::istream *stream)
{
    this->mFileSize = readUInt32(stream) + 8;

    FourCC waveTag;
    waveTag.load(stream);

    if (waveTag != WAV_WAVE) {
        throw WavHeaderError("WAVE header is missing WAVE tag while processing file");
    }

    FourCC   chunkId;
    uint64_t pos = 12;
    while (pos < this->mFileSize) {

        chunkId.load(stream);
        uint32_t chunkSize = readUInt32(stream);
        pos += 8;

        if (chunkId == WAV_DATA) {
            this->mDataSize     = chunkSize;
            this->mDataStartPos = pos;
            return;
        }

        if (chunkSize < 1) {
            throw WavHeaderError("[WAV] incorrect chunk size " + std::to_string(chunkSize) + " at " + std::to_string(pos - 4));
        }

        if (chunkId == WAV_FMT) {
            loadFmtChunk(stream, chunkSize);
            pos += chunkSize;
        }
        else {
            mOtherCunks << chunkId;
            mOtherCunks << chunkSize;
            mOtherCunks << readBytes(stream, chunkSize);
            pos += chunkSize;
        }
    }

    throw WavHeaderError("data chunk not found");
}

/************************************************
 * All chunks are byte-aligned on 8-byte boundaries, but their
 * chunk size fields do not include any padding if it is necessary.
 ************************************************/
void WavHeader::readWave64Header(std::istream *stream)
{
    this->mFileSize = readUInt64(stream);

    Guid waveTag;
    waveTag.load(stream);

    if (!waveTag.startsWidth(WAVE64_WAVE)) {
        throw WavHeaderError("WAVE64 header is missing WAVE tag while processing file");
    }

    Guid     chunkId;
    uint64_t pos = 16 + 8 + 16;
    while (pos < this->mFileSize) {

        // All chunks are byte-aligned on 8-byte boundaries
        if (pos % 8) {
            char d[8];
            mustRead(stream, d, 8 - (pos % 8));
        }

        chunkId.load(stream);
        uint64_t chunkSize = readUInt64(stream);
        pos += WAVE64_CHUNK_HEADER_SIZE;

        if (chunkId.startsWidth(WAV_DATA)) {
            this->mDataSize     = chunkSize - WAVE64_CHUNK_HEADER_SIZE;
            this->mDataStartPos = pos;
            return;
        }

        if (chunkSize < 1) {
            throw WavHeaderError("[WAVE] incorrect chunk size " + std::to_string(chunkSize) + " at " + std::to_string(pos - 4));
        }

        if (chunkId.startsWidth(WAV_FMT)) {
            loadFmtChunk(stream, chunkSize - 16 - 8);
            pos += chunkSize - WAVE64_CHUNK_HEADER_SIZE;
        }
        else {
            mOtherCunks << chunkId;
            mOtherCunks << chunkSize;
            mOtherCunks << readBytes(stream, chunkSize - WAVE64_CHUNK_HEADER_SIZE);
            pos += chunkSize - WAVE64_CHUNK_HEADER_SIZE;
        }
    }

    throw WavHeaderError("data chunk not found");
}

/************************************************
 *
 ************************************************/
bool WavHeader::isCdQuality() const
{
    static const int CD_NUM_CHANNELS    = 2;
    static const int CD_BITS_PER_SAMPLE = 16;
    static const int CD_SAMPLE_RATE     = 44100;
    static const int CD_BYTE_RATE       = 176400;

    return mNumChannels == CD_NUM_CHANNELS && mBitsPerSample == CD_BITS_PER_SAMPLE && mSampleRate == CD_SAMPLE_RATE && mByteRate == CD_BYTE_RATE;
}

/************************************************
 *
 ************************************************/
uint64_t WavHeader::duration() const
{
    return (mDataSize * 1000ull) / mByteRate;
}

/************************************************
 *
 ************************************************/
uint32_t WavHeader::bytesPerSecond(WavHeader::Quality quality)
{
    switch (quality) {
        case Quality_Stereo_CD:
            return 2 * 16 * 44100 / 8;
        case Quality_Stereo_24_96:
            return 2 * 24 * 96000 / 8;
        case Quality_Stereo_24_192:
            return 2 * 24 * 192000 / 8;
    }
    return 0;
}

/************************************************
 *
 ************************************************/
uint32_t WavHeader::bytesPerSecond()
{
    return mNumChannels * mBitsPerSample * mSampleRate / 8;
}

/************************************************
 *
 ************************************************/
void checkFormat(uint16_t format)
{

    switch (format) {
        case WavHeader::Format_Unknown:
        case WavHeader::Format_PCM:
        case WavHeader::Format_ADPCM:
        case WavHeader::Format_IEEE_FLOAT:
        case WavHeader::Format_ALAW:
        case WavHeader::Format_MULAW:
        case WavHeader::Format_OKI_ADPCM:
        case WavHeader::Format_IMA_ADPCM:
        case WavHeader::Format_DIGISTD:
        case WavHeader::Format_DIGIFIX:
        case WavHeader::Format_DOLBY_AC2:
        case WavHeader::Format_GSM610:
        case WavHeader::Format_ROCKWELL_ADPCM:
        case WavHeader::Format_ROCKWELL_DIGITALK:
        case WavHeader::Format_G721_ADPCM:
        case WavHeader::Format_G728_CELP:
        case WavHeader::Format_MPEG:
        case WavHeader::Format_MPEGLAYER3:
        case WavHeader::Format_G726_ADPCM:
        case WavHeader::Format_G722_ADPCM:
        case WavHeader::Format_Extensible:
            return;
    }

    throw WavHeaderError("Unknown format (" + std::to_string(format) + " in WAVE header");
}

/************************************************
 *
 ************************************************/
void WavHeader::loadFmtChunk(std::istream *stream, const uint32_t chunkSize)
{
    if (chunkSize != FmtChunkMin && chunkSize != FmtChunkMid && chunkSize != FmtChunkExt)
        throw WavHeaderError("fmt chunk in WAVE header hase incorrect length");

    mFmtSize = FmtChunkSize(chunkSize);

    uint16_t format = readUInt16(stream);

    this->mFormat = static_cast<Format>(format);
    checkFormat(format);
    this->mNumChannels   = readUInt16(stream);
    this->mSampleRate    = readUInt32(stream);
    this->mByteRate      = readUInt32(stream);
    this->mBlockAlign    = readUInt16(stream);
    this->mBitsPerSample = readUInt16(stream);

    if (chunkSize == FmtChunkMin)
        return;

    mExtSize = readUInt16(stream); // Size of the extension:
    if (chunkSize == FmtChunkMid)
        return;

    if (mExtSize != FmtChunkExt - FmtChunkMid)
        throw WavHeaderError("Size of the extension in WAVE header hase incorrect length");

    mValidBitsPerSample = readUInt16(stream);    // at most 8*M
    mChannelMask        = readUInt32(stream);    // Speaker position mask
    mSubFormat          = readBytes(stream, 16); // GUID (first two bytes are the data format code)
}

/************************************************
 *
 ************************************************/
// QByteArray WavHeader::toByteArray() const
//{
//     if (m64Bit) {
//         return wave64ToByteArray();
//     }
//     else {
//         return wavToByteArray(true);
//     }
// }

/************************************************
 *
 ************************************************/
// QByteArray WavHeader::toLegacyWav() const
//{
//     return wavToByteArray(false);
// }

/************************************************
 * 52 49 46 46      RIFF
 * 24 B9 4D 02      file size - 8
 * 57 41 56 45      WAVE
 *
 * // Chanks
 *   66 6D 74 20    SubchunkID      "fmt "
 *   10 00 00 00    SubchunkSize        16
 *         01 00      AudioFormat      PCM
 *         02 00	  NumChannels        2
 *   44 AC 00 00      SampleRate     44100
 *   10 B1 02 00      ByteRate      176400
 *         04 00      BlockAlign         4
 *         10 00      BitsPerSample     16
 * // Data
 *   64 61 74 61 	SubchunkID 		"data"
 *   00 B9 4D 02 	SubchunkSize
 ************************************************/
ByteArray WavHeader::wavToByteArray(bool keepOtherChunks) const
{
    ByteArray res;
    res.reserve(mDataStartPos - 1);
    res << WAV_RIFF;
    res << uint32_t(0);
    res << WAV_WAVE;

    res << WAV_FMT;
    res << uint32_t(mFmtSize);
    res << uint16_t(mFormat);
    res << mNumChannels;
    res << mSampleRate;
    res << mByteRate;
    res << mBlockAlign;
    res << mBitsPerSample;

    if (mFmtSize > FmtChunkMin) {
        res << mExtSize;
    }

    if (mExtSize > 0) {
        res << mValidBitsPerSample;
        res << mChannelMask;
        res.insert(res.end(), mSubFormat.begin(), mSubFormat.end());
    }

    if (keepOtherChunks) {
        res.insert(res.end(), mOtherCunks.begin(), mOtherCunks.end());
    }

    res << WAV_DATA;
    res << uint32_t(mDataSize);

    // Write file size .........
    uint64_t fileSize = mDataSize + res.size() - 8;
    if (fileSize > 0xFFFFFFFF) {
        throw WavHeaderError("Stream is too big to fit in a legacy WAVE file");
    }

    uint32_t le = toLittleEndian(uint32_t(fileSize));
    res[4]      = (le >> 0) & 0xFF;
    res[5]      = (le >> 8) & 0xFF;
    res[6]      = (le >> 16) & 0xFF;
    res[7]      = (le >> 24) & 0xFF;

    return res;
}

/************************************************
 * The chunk size fields directly following the chunk-GUID and preceeding
 * the chunk body, include the size of the chunk-GUID and the chunk length
 * field itself.
 * Therefore, it corresponds to the chunk data size plus 24 (16 bytes for
 * the GUID, 8 bytes for the size field).
 ************************************************/
ByteArray WavHeader::wave64ToByteArray() const
{
    ByteArray res;
    res.reserve(mDataStartPos - 1);
    res << WAVE64_GUID_RIFF;
    res << uint64_t(mFileSize);
    res << WAVE64_GUID_WAVE;

    res << WAVE64_GUID_FMT;
    // The chunk size fields include the size of the chunk-GUID and the chunk length field itself. Therefore, it corresponds to the chunk data size plus 24 (16 bytes for the GUID, 8 bytes for the size field).
    res << uint64_t(mFmtSize + 24); //
    res << uint16_t(mFormat);
    res << mNumChannels;
    res << mSampleRate;
    res << mByteRate;
    res << mBlockAlign;
    res << mBitsPerSample;

    if (mFmtSize > FmtChunkMin) {
        res << mExtSize;
    }

    if (mExtSize > 0) {
        res << mValidBitsPerSample;
        res << mChannelMask;
        res.insert(res.end(), mSubFormat.begin(), mSubFormat.end());
    }
    res.insert(res.end(), mOtherCunks.begin(), mOtherCunks.end());

    res << WAVE64_GUID_DATA;
    res << uint64_t(mDataSize + WAVE64_CHUNK_HEADER_SIZE);

    return res;
}

/************************************************
 *
 ************************************************/
void WavHeader::resizeData(uint32_t dataSize)
{
    mDataSize = dataSize;
    mFileSize = mDataStartPos + mDataSize;
}
