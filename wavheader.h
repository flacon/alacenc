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

#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <cstdint>
#include <fstream>
#include <vector>

using ByteArray      = std::vector<uint8_t>;
using WavHeaderError = std::runtime_error;

/************************************************
 * Info for format tags can be found at:
 *   http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 ************************************************/
class WavHeader
{
public:
    enum Format {
        Format_Unknown           = 0x0000,
        Format_PCM               = 0x0001,
        Format_ADPCM             = 0x0002,
        Format_IEEE_FLOAT        = 0x0003,
        Format_ALAW              = 0x0006,
        Format_MULAW             = 0x0007,
        Format_OKI_ADPCM         = 0x0010,
        Format_IMA_ADPCM         = 0x0011,
        Format_DIGISTD           = 0x0015,
        Format_DIGIFIX           = 0x0016,
        Format_DOLBY_AC2         = 0x0030,
        Format_GSM610            = 0x0031,
        Format_ROCKWELL_ADPCM    = 0x003b,
        Format_ROCKWELL_DIGITALK = 0x003c,
        Format_G721_ADPCM        = 0x0040,
        Format_G728_CELP         = 0x0041,
        Format_MPEG              = 0x0050,
        Format_MPEGLAYER3        = 0x0055,
        Format_G726_ADPCM        = 0x0064,
        Format_G722_ADPCM        = 0x0065,
        Format_Extensible        = 0xFFFE,
    };

    enum Quality {
        Quality_Stereo_CD     = 2 * 16 * 44100,
        Quality_Stereo_24_96  = 2 * 24 * 96000,
        Quality_Stereo_24_192 = 2 * 24 * 192000

    };

    typedef char SubFormat[16];

    WavHeader() = default;
    explicit WavHeader(std::istream *stream) noexcept(false);

    WavHeader(const WavHeader &other) = default;
    WavHeader &operator=(const WavHeader &other) = default;

    /// Duration of audio in milliseconds.
    uint64_t duration() const;

    // QByteArray toByteArray() const;
    // QByteArray toLegacyWav() const;

    void resizeData(uint32_t dataSize);

    static uint32_t bytesPerSecond(Quality quality);
    uint32_t        bytesPerSecond();

    uint64_t fileSize() const { return mFileSize; }
    Format   format() const { return mFormat; }
    uint16_t numChannels() const { return mNumChannels; }
    uint32_t sampleRate() const { return mSampleRate; }
    uint32_t byteRate() const { return mByteRate; }
    uint16_t blockAlign() const { return mBlockAlign; }
    uint16_t bitsPerSample() const { return mBitsPerSample; }
    uint16_t validBitsPerSample() const { return mValidBitsPerSample; }
    uint32_t channelMask() const { return mChannelMask; }
    uint64_t dataSize() const { return mDataSize; }
    uint64_t dataStartPos() const { return mDataStartPos; }
    bool     isCdQuality() const;
    bool     is64Bit() const { return m64Bit; }

protected:
    enum FmtChunkSize {
        FmtChunkMin = 16,
        FmtChunkMid = 18,
        FmtChunkExt = 40,
    };

    bool         m64Bit              = false;
    uint64_t     mFileSize           = 0;
    FmtChunkSize mFmtSize            = FmtChunkMin;
    Format       mFormat             = WavHeader::Format_Unknown;
    uint16_t     mNumChannels        = 0;
    uint32_t     mSampleRate         = 0;
    uint32_t     mByteRate           = 0;
    uint16_t     mBlockAlign         = 0;
    uint16_t     mBitsPerSample      = 0;
    uint16_t     mExtSize            = 0;     // Size of the extension:
    uint16_t     mValidBitsPerSample = 0;     // at most 8*M
    uint32_t     mChannelMask        = 0;     // Speaker position mask
    ByteArray    mSubFormat          = { 0 }; // GUID (first two bytes are the data format code)
    uint64_t     mDataSize           = 0;
    uint64_t     mDataStartPos       = 0;

    ByteArray mOtherCunks;

private:
    void loadFmtChunk(std::istream *stream, const uint32_t chunkSize);

    void readWavHeader(std::istream *stream);
    void readWave64Header(std::istream *stream);

    ByteArray wavToByteArray(bool keepOtherChunks) const;
    ByteArray wave64ToByteArray() const;
};

#endif // WAVHEADER_H
