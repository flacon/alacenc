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
