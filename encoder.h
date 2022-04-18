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

#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <istream>
#include <memory>
#include "types.h"
#include "wavheader.h"
#include "vendor/alac/codec/ALACAudioTypes.h"
#include "vendor/alac/codec/ALACEncoder.h"
#include "tags.h"

class Encoder
{
public:
    struct Options
    {
        std::string inFile;
        std::string outFile;

        bool showProgress = true;
        bool fastMode     = false;
    };

    explicit Encoder(const Options &options) noexcept(false);

    void run();

    Options options() const { return mOptions; }

    const std::vector<uint32_t> &sampleSizeTable() const { return mSampleSizeTable; }

    std::vector<char> getMagicCookie() const;
    WavHeader         inputWavHeader() const { return mWavHeader; }

    uint32_t audioDataStartPos() const { return mAudioDataStartPos; }

    uint32_t sampleSize() const;

    AudioFormatDescription inFormat() const { return mInFormat; }

    const Tags &tags() const { return mTags; }
    void        setTags(const Tags &value);

private:
    const Options                 mOptions;
    std::shared_ptr<std::istream> mInFile;
    WavHeader                     mWavHeader;
    AudioFormatDescription        mInFormat;
    AudioFormatDescription        mOutFormat;
    ALACEncoder                   mEncoder;
    std::vector<uint32_t>         mSampleSizeTable;
    uint32_t                      mAudioDataStartPos = 0;
    Tags                          mTags;

    void initInFormat();
    void initOutFormat();
    void writeAudioData(std::istream *in, OutFile &out);
};

#endif // ENCODER_H
