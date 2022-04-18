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

#include "encoder.h"
#include <iostream>
#include <fstream>
#include "types.h"
#include "atoms.h"
#include <list>
#include <cstring>
#include <algorithm>

struct noop
{
    void operator()(...) const { }
};

Encoder::Encoder(const Options &options) noexcept(false) :
    mOptions(options),
    mInFormat({}),
    mOutFormat({})
{
    if (mOptions.inFile == "-") {
        mInFile.reset(&std::cin, noop());
    }
    else {
        mInFile.reset(new std::ifstream(mOptions.inFile.c_str()));
        if (mInFile->fail()) {
            throw Error(mOptions.inFile + ": " + strerror(errno));
        }
    }
}

void Encoder::initInFormat()
{
    mInFormat.mFormatID         = kALACFormatLinearPCM;
    mInFormat.mSampleRate       = mWavHeader.sampleRate();
    mInFormat.mBitsPerChannel   = mWavHeader.bitsPerSample();
    mInFormat.mFormatFlags      = kALACFormatFlagIsSignedInteger | kALACFormatFlagIsPacked; // always little endian
    mInFormat.mBytesPerFrame    = (mWavHeader.bitsPerSample() / 8) * mWavHeader.numChannels();
    mInFormat.mFramesPerPacket  = 1;
    mInFormat.mBytesPerPacket   = mInFormat.mBytesPerFrame * mInFormat.mFramesPerPacket;
    mInFormat.mChannelsPerFrame = mWavHeader.numChannels();
    mInFormat.mReserved         = 0;
}

void Encoder::initOutFormat()
{
    enum FormatFlag {
        Flag_16BitSource = 1,
        Flag_20BitSource = 2,
        Flag_24BitSource = 3,
        Flag_32BitSource = 4
    };

    mOutFormat.mFormatID = kALACFormatAppleLossless;
    // clang-format off
    switch(mWavHeader.bitsPerSample())
    {
        case 16: mOutFormat.mFormatFlags = Flag_16BitSource; break;
        case 20: mOutFormat.mFormatFlags = Flag_20BitSource; break;
        case 24: mOutFormat.mFormatFlags = Flag_24BitSource; break;
        case 32: mOutFormat.mFormatFlags = Flag_32BitSource; break;
        default: throw Error("Unsupported bitsPerSample" + std::to_string(mWavHeader.bitsPerSample()));
    }
    // clang-format on

    mOutFormat.mSampleRate       = mWavHeader.sampleRate();
    mOutFormat.mFramesPerPacket  = kALACDefaultFramesPerPacket;
    mOutFormat.mChannelsPerFrame = mWavHeader.numChannels();
    mOutFormat.mBytesPerPacket   = 0; // because we are VBR
    mOutFormat.mBytesPerFrame    = 0; // because there are no discernable bits assigned to a particular sample
    mOutFormat.mBitsPerChannel   = 0; // because there are no discernable bits assigned to a particular sample
    mOutFormat.mReserved         = 0;
}

void Encoder::run()
{
    OutFile out = mOptions.outFile == "-" ? OutFile() : OutFile(mOptions.outFile);
    mWavHeader  = WavHeader(mInFile.get());
    initInFormat();
    initOutFormat();

    mEncoder.SetFrameSize(mOutFormat.mFramesPerPacket);
    mEncoder.InitializeEncoder(mOutFormat);
    mEncoder.SetFastMode(mOptions.fastMode);

    out << FtypAtom();
    out << FreeAtom(8);
    writeAudioData(mInFile.get(), out);
    out << MoovAtom(*this);
    out.flush();
}

std::vector<char> Encoder::getMagicCookie() const
{
    uint32_t size = mEncoder.GetMagicCookieSize(mWavHeader.numChannels());

    std::vector<char> res(size);
    mEncoder.GetMagicCookie(res.data(), &size);
    res.resize(size);
    return res;
}

uint32_t Encoder::sampleSize() const
{
    return mInFormat.mChannelsPerFrame * (mInFormat.mBitsPerChannel / 8) * mOutFormat.mFramesPerPacket;
}

void Encoder::setTags(const Tags &value)
{
    mTags = value;
}

void Encoder::writeAudioData(std::istream *in, OutFile &out)
{
    const int32_t inBufSize = sampleSize();
    mSampleSizeTable.reserve(mWavHeader.dataSize() / inBufSize);
    std::vector<char> inBuf(inBufSize);

    std::list<std::vector<unsigned char>> data;

    uint32_t outDataSize = 0;
    uint64_t total       = mWavHeader.dataSize();
    uint64_t remained    = total;
    int      percent     = 0;

    while (remained > 0) {
        if (!in->good()) {
            throw Error(mOptions.inFile + ": " + strerror(errno));
        }

        if (mOptions.showProgress) {
            int p = (total - remained) * 100.0 / total;
            if (p > percent) {
                fprintf(stderr, "  %d%%\r", p);
                percent = p;
            }
        }

        int32_t readed = std::min(uint64_t(inBufSize), remained);
        in->read(inBuf.data(), readed);
        remained -= readed;

        int32_t size = readed;

        std::vector<unsigned char> &outBuf = data.emplace_back(mEncoder.maxOutputBytes());
        mEncoder.Encode(mInFormat, mOutFormat, (unsigned char *)inBuf.data(), outBuf.data(), &size);
        outBuf.resize(size);

        outDataSize += size;
        mSampleSizeTable.push_back(size);
    }

    out << uint32_t(outDataSize + 8);
    out << "mdat";
    mAudioDataStartPos = out.tellp();

    for (auto &chunk : data) {
        out.write(chunk.data(), chunk.size());
    }
}
