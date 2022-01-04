#include "encoder.h"
#include <iostream>
#include <fstream>
#include "types.h"
#include "vendor/alac/codec/ALACAudioTypes.h"
#include "vendor/alac/codec/ALACEncoder.h"

struct noop
{
    void operator()(...) const { }
};

Encoder::Encoder(const Options &options) noexcept(false) :
    mOptions(options)
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

    if (mOptions.outFile == "-") {
        mOutFile.reset(&std::cout, noop());
    }
    else {
        mOutFile.reset(new std::ofstream(mOptions.outFile));
        if (mOutFile->fail()) {
            throw Error(mOptions.outFile + ": " + strerror(errno));
        }
    }
}

AudioFormatDescription calcInputAudioFormat(const WavHeader &inputHeader)
{
    AudioFormatDescription res;
    res.mFormatID         = kALACFormatLinearPCM;
    res.mSampleRate       = inputHeader.sampleRate();
    res.mBitsPerChannel   = inputHeader.bitsPerSample();
    res.mFormatFlags      = kALACFormatFlagIsSignedInteger | kALACFormatFlagIsPacked; // always little endian
    res.mBytesPerFrame    = (inputHeader.bitsPerSample() / 8) * inputHeader.numChannels();
    res.mFramesPerPacket  = 1;
    res.mBytesPerPacket   = res.mBytesPerFrame * res.mFramesPerPacket;
    res.mChannelsPerFrame = inputHeader.numChannels();
    res.mReserved         = 0;
    return res;
}

AudioFormatDescription calcOutputAudioFormat(const WavHeader &inputHeader)
{
    enum FormatFlag {
        Flag_16BitSource = 1,
        Flag_20BitSource = 2,
        Flag_24BitSource = 3,
        Flag_32BitSource = 4
    };

    AudioFormatDescription res;
    res.mFormatID = kALACFormatAppleLossless;
    // clang-format off
    switch(inputHeader.bitsPerSample())
    {
        case 16: res.mFormatFlags = Flag_16BitSource; break;
        case 20: res.mFormatFlags = Flag_20BitSource; break;
        case 24: res.mFormatFlags = Flag_24BitSource; break;
        case 32: res.mFormatFlags = Flag_32BitSource; break;
        default: throw Error("Unsupported bitsPerSample" + std::to_string(inputHeader.bitsPerSample()));
    }
    // clang-format on

    res.mFramesPerPacket  = kALACDefaultFramesPerPacket;
    res.mChannelsPerFrame = inputHeader.numChannels();
    res.mBytesPerPacket   = 0; // because we are VBR
    res.mBytesPerFrame    = 0; // because there are no discernable bits assigned to a particular sample
    res.mBitsPerChannel   = 0; // because there are no discernable bits assigned to a particular sample
    res.mReserved         = 0;

    return res;
}

void Encoder::run()
{
    WavHeader inputHeader = WavHeader(mInFile.get());

    AudioFormatDescription inputFormat  = calcInputAudioFormat(inputHeader);
    AudioFormatDescription outputFormat = calcOutputAudioFormat(inputHeader);

    ALACEncoder alacEncoder;
    alacEncoder.SetFrameSize(outputFormat.mFramesPerPacket);
    alacEncoder.InitializeEncoder(outputFormat);

    // get the magic cookie
    // theMagicCookieSize = theEncoder->GetMagicCookieSize(theOutputFormat.mChannelsPerFrame);
    // theMagicCookie = (uint8_t *)calloc(theMagicCookieSize, 1);
    // theEncoder->GetMagicCookie(theMagicCookie, &theMagicCookieSize);

    const int32_t inSize  = inputFormat.mChannelsPerFrame * (inputFormat.mBitsPerChannel / 8) * outputFormat.mFramesPerPacket;
    const int32_t outSize = inSize + kALACMaxEscapeHeaderBytes;

    char inBuf[inSize];
    char outBuf[outSize];
    while (mInFile->read(inBuf, inSize)) {
        int32_t size = inSize;
        alacEncoder.Encode(inputFormat, outputFormat, (unsigned char *)inBuf, (unsigned char *)outBuf, &size);
        std::cerr << "*" << size << std::endl;
        mOutFile->write(outBuf, size);
    }

    // int32_t theInputPacketBytes = theInputFormat.mChannelsPerFrame * (theInputFormat.mBitsPerChannel >> 3) * theOutputFormat.mFramesPerPacket;

    //    std::cerr
    //            << "bitsPerSample: " << mInFileHeader.bitsPerSample() << std::endl;
    //    std::cerr << "sampleRate:   " << mInFileHeader.sampleRate() << std::endl;
    //    std::cerr << "dataStartPos: " << mInFileHeader.dataStartPos() << std::endl;
    //    std::cerr << "dataSize:     " << mInFileHeader.dataSize() << std::endl;
    //    std::cerr << "pos:     " << mInFile->tellg() << std::endl;
}
