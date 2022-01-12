#include "encoder.h"
#include <iostream>
#include <fstream>
#include "types.h"
#include "atoms.h"
#include <list>
#include <cstring>
#include <algorithm>

#define FAKE_AUDIO 0

struct noop
{
    void operator()(...) const {}
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

    //    if (mOptions.outFile == "-") {
    //        mOutFile.reset(&std::cout, noop());
    //    }
    //    else {
    //        mOutFile.reset(new std::ofstream(mOptions.outFile));
    //        if (mOutFile->fail()) {
    //            throw Error(mOptions.outFile + ": " + strerror(errno));
    //        }
    //    }

    //  mInFile->exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
    //  mOutFile->exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
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

#define FAKE 0

void Encoder::run()
{
    OutFile out = mOptions.outFile == "-" ? OutFile() : OutFile(mOptions.outFile);
    mWavHeader  = WavHeader(mInFile.get());
    initInFormat();
    initOutFormat();

    mEncoder.SetFrameSize(mOutFormat.mFramesPerPacket);
    mEncoder.InitializeEncoder(mOutFormat);

    // std::ostream &out = *mOutFile;

#if FAKE
    out << FakeAtom_ftyp(mEncoder, mWavHeader);
    out << FreeAtom(8);

    // out << FtypAtom();
    // out << MoovAtom(mEncoder, wavHeader);
    writeAudioData();
    out << FakeAtom_moov(mEncoder, mWavHeader);
    out.flush();
#else

    out << FtypAtom();
    out << FreeAtom(8);
    // out << FtypAtom();
    // out << MoovAtom(mEncoder, wavHeader);
    writeAudioData(mInFile.get(), out);
    out << MoovAtom(*this);
    // out << FakeAtom_moov(mEncoder, mWavHeader);
    out.flush();

#endif
    // return;

    //    const int32_t inSize  = mInFormat.mChannelsPerFrame * (mInFormat.mBitsPerChannel / 8) * mOutFormat.mFramesPerPacket;
    //    const int32_t outSize = inSize + kALACMaxEscapeHeaderBytes;

    //    char inBuf[inSize];
    //    char outBuf[outSize];
    //    while (mInFile->read(inBuf, inSize)) {
    //        int32_t size = inSize;
    //        alacEncoder.Encode(mInFormat, mOutFormat, (unsigned char *)inBuf, (unsigned char *)outBuf, &size);
    //        std::cerr << "*" << size << std::endl;
    //        mOutFile->write(outBuf, size);
    //    }

    // int32_t theInputPacketBytes = theInputFormat.mChannelsPerFrame * (theInputFormat.mBitsPerChannel >> 3) * theOutputFormat.mFramesPerPacket;

    //    std::cerr
    //            << "bitsPerSample: " << mInFileHeader.bitsPerSample() << std::endl;
    //    std::cerr << "sampleRate:   " << mInFileHeader.sampleRate() << std::endl;
    //    std::cerr << "dataStartPos: " << mInFileHeader.dataStartPos() << std::endl;
    //    std::cerr << "dataSize:     " << mInFileHeader.dataSize() << std::endl;
    //    std::cerr << "pos:     " << mInFile->tellg() << std::endl;
}

std::vector<char> Encoder::getMagicCookie() const
{
    uint32_t size = mEncoder.GetMagicCookieSize(mWavHeader.numChannels());

    std::vector<char> res(size);
    mEncoder.GetMagicCookie(res.data(), &size);
    res.resize(size);
    return res;
}

#if FAKE

void Encoder::writeAudioData(const std::istream &in, OutFile &out)
{

    {
        auto in = std::ifstream("fake.bin");
        *mOutFile << in.rdbuf();
        return;
    }
}
#else

uint32_t Encoder::sampleSize() const
{
    //    std::cerr << "mInFormat.mChannelsPerFrame: " << mInFormat.mChannelsPerFrame << std::endl;
    //    std::cerr << "mInFormat.mBitsPerChannel / 8: " << (mInFormat.mBitsPerChannel / 8) << std::endl;
    //    std::cerr << "mOutFormat.mFramesPerPacket: " << mOutFormat.mFramesPerPacket << std::endl;
    return mInFormat.mChannelsPerFrame * (mInFormat.mBitsPerChannel / 8) * mOutFormat.mFramesPerPacket;
}
#endif

#if FAKE_AUDIO
#include "mAudioDataStartPos.h"
void Encoder::writeAudioData(std::istream *in, OutFile &out)
{
    const int32_t inBufSize = sampleSize();
    mSampleSizeTable.reserve(mWavHeader.dataSize() / inBufSize);
    //const int32_t outBufSize = inBufSize + kALACMaxEscapeHeaderBytes;
    //char          inBuf[inBufSize];

    std::list<std::vector<unsigned char>> data;

    mAudioDataStartPos = out.tellp() + 8;

    auto f = std::ifstream("fake.bin");
    char c;
    while (f.read(&c, 1)) {
        out << c;
    }
    mSampleSizeTable = FAKE_SAMPLE_SIZE_TABLE;

    // out.write()
}

#else

void Encoder::writeAudioData(std::istream *in, OutFile &out)
{
    const int32_t inBufSize = sampleSize(); // mInFormat.mChannelsPerFrame * (mInFormat.mBitsPerChannel / 8) * mOutFormat.mFramesPerPacket;
    // std::cerr << "BUF SIZE:" << inBufSize << std::endl;
    std::cerr << "DATA SIZE:" << std::hex << mWavHeader.dataSize() << std::endl;
    mSampleSizeTable.reserve(mWavHeader.dataSize() / inBufSize);
    const int32_t outBufSize = inBufSize + kALACMaxEscapeHeaderBytes;
    char inBuf[inBufSize];

    std::list<std::vector<unsigned char>> data;

    uint32_t outDataSize = 0;
    uint64_t total = mWavHeader.dataSize();
    uint64_t remained = total;
    int percent = 0;
    while (remained > 0) {
        if (!in->good()) {
            throw Error(mOptions.inFile + ": " + strerror(errno));
        }

        if (mOptions.showProgress) {
            int p = (total - remained) * 100.0 / total;
            if (p > percent) {
                fprintf(stderr, "%d%%\r", p);
                percent = p;
            }
        }

        int32_t size = std::min(uint64_t(inBufSize), remained);

        in->read(inBuf, size);
        remained -= size;

        std::vector<unsigned char> outBuf(outBufSize);

        mEncoder.Encode(mInFormat, mOutFormat, (unsigned char *)inBuf, (unsigned char *)outBuf.data(), &size);
        outBuf.resize(size);
        data.push_back(std::move(outBuf));
        outDataSize += size;
        mSampleSizeTable.push_back(size);
    }

    out << uint32_t(outDataSize + 8);
    out << "mdat";
    mAudioDataStartPos = out.tellp();

    for (auto chunk : data) {
        out.write(chunk.data(), chunk.size());
    }

    // out.write()
}
#endif
