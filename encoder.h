#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <istream>
#include <memory>
#include "types.h"
#include "wavheader.h"
#include "vendor/alac/codec/ALACAudioTypes.h"
#include "vendor/alac/codec/ALACEncoder.h"

class Encoder
{
public:
    struct Options
    {
        std::string inFile;
        std::string outFile;

        bool showProgress = true;
    };

    // static void run(const Options &options) noexcept(false);

    explicit Encoder(const Options &options) noexcept(false);
    // virtual ~Encoder();

    Options options() const { return mOptions; }

    void run();

    const std::vector<uint32_t> &sampleSizeTable() const { return mSampleSizeTable; }

    std::vector<char> getMagicCookie() const;
    WavHeader         inputWavHeader() const { return mWavHeader; }

    uint32_t audioDataStartPos() const { return mAudioDataStartPos; }

    uint32_t sampleSize() const;

    AudioFormatDescription inFormat() const { return mInFormat; }

private:
    const Options                 mOptions;
    std::shared_ptr<std::istream> mInFile;
    // std::shared_ptr<std::ostream> mOutFile;
    WavHeader              mWavHeader;
    AudioFormatDescription mInFormat;
    AudioFormatDescription mOutFormat;
    ALACEncoder            mEncoder;
    std::vector<uint32_t>  mSampleSizeTable;
    uint32_t               mAudioDataStartPos = 0;

    void initInFormat();
    void initOutFormat();
    void writeAudioData(std::istream *in, OutFile &out);
};

#endif // ENCODER_H
