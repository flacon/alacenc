#include <iostream>
#include "encoder.h"
#include "vendor/docopt/docopt.h"

static constexpr auto VERSION_STR = PROJECT_NAME " " PROJECT_VERSION;

static constexpr auto USAGE_TEXT =
        PROJECT_NAME " - " PROJECT_DESCRIPTION R"(

Usage:
  alacenc [options] <INPUT_FILE> <OUTPUT_FILE>

Arguments:
  INPUT_FILE    Input file name, when INPUT_FILE is -, read standard input
  OUTPUT_FILE   Output ALAC file name, when OUTPUT_FILE is -, write to standard output

Options:
  -q, --quiet   Produce no output to stderr
  -h, --help     Print this help text and exit
  -V, --version  Print the version number

)";

int main(int argc, const char **argv)
{
    docopt::Options args = docopt::docopt(USAGE_TEXT, { argv + 1, argv + argc }, true, VERSION_STR);

    Encoder::Options options;
    options.inFile       = args.at("<INPUT_FILE>").asString();
    options.outFile      = args.at("<OUTPUT_FILE>").asString();
    options.showProgress = !args.at("--quiet").asBool();

    try {
        Encoder enc(options);
        enc.run();
    }
    catch (const std::runtime_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
}
