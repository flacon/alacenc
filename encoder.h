#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <istream>
#include "wavheader.h"

class Encoder
{
public:
    struct Options
    {
        std::string inFile;
        std::string outFile;
    };

    // static void run(const Options &options) noexcept(false);

    explicit Encoder(const Options &options) noexcept(false);
    // virtual ~Encoder();

    Options options() const { return mOptions; }

    void run();

private:
    const Options                 mOptions;
    std::shared_ptr<std::istream> mInFile;
    std::shared_ptr<std::ostream> mOutFile;

    // WavHeader mInFileHeader;
};

#endif // ENCODER_H
