#include <iostream>
#include "types.h"
#include "encoder.h"

#if 0
#include <iostream>
#include <map>
#include "vendor/docopt/docopt.h"
#include "encoder.h"
#include "types.h"

using namespace std;

static constexpr auto VERSION_STR = PROJECT_NAME " " PROJECT_VERSION " - " PROJECT_DESCRIPTION;

static constexpr auto USAGE = R"(
Usage:
  alac [options] <INPUT_FILE> <OUTPUT_FILE>
  alac [options] -d <INPUT_FILE> <OUTPUT_FILE>

Arguments:
  INPUT_FILE    Input file name, when INPUT_FILE is -, read standard input.
  OUTPUT_FILE   Output ALAC file name, when OUTPUT_FILE is -, write to standard output.

Options:
  -d --decode   Decode (the default behavior is to encode)
  -h --help     Show this screen and exit.
  --version     Show version.

)";

Encoder::Options getEncoderOptions(const docopt::Options &args);

int main(int argc, const char **argv)
{
    std::map<std::string, docopt::value> args =
            docopt::docopt(USAGE,
                           { argv + 1, argv + argc },
                           true,         // show help if requested
                           VERSION_STR); // version string

    for (auto const &arg : args) {
        std::cout << arg.first << " = " << arg.second << std::endl;
        //        std::cout << "    " << int(arg.second.kind()) << std::endl;
    }

    try {

        if (args.at("--decode").asBool()) {
            std::cerr << "RUN DECODE" << std::endl;
            // Todo: DECODE
        }
        else {
            std::cerr << "RUN 1" << std::endl;
            Encoder::run(getEncoderOptions(args));
            std::cerr << "RUN 2" << std::endl;
            exit(0);
        }
    }
    catch (const Error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        exit(1);
    }

    return 0;
}

Encoder::Options getEncoderOptions(const docopt::Options &args)
{

    Encoder::Options res;
    try {
        res.inFile = args.at("<INPUT_FILE>").asString();
    }
    catch (const std::out_of_range &) {
        throw Error("INPUT_FILE not set");
    }
    catch (const std::runtime_error &) {
        throw Error("INPUT_FILE not set");
    }

    try {
        res.outFile = args.at("<OUTPUT_FILE>").asString();
    }
    catch (const std::out_of_range &) {
        throw Error("OUTPUT_FILE not set");
    }
    catch (const std::runtime_error &) {
        throw Error("OUTPUT_FILE not set");
    }

    return res;
}
#endif

//#include "wavheader.h"
//#include <termios.h>
//#include <unistd.h>

// termios original;

// void enableRawMode()
//{
//     tcgetattr(STDIN_FILENO, &original);
//     termios raw = original;
//     cfmakeraw(&raw);
//     raw.c_cc[VMIN]  = 0;
//     raw.c_cc[VTIME] = 1;
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
// }

int main(int argc, const char **argv)
{
    Encoder::Options options;
    options.inFile  = argv[1];
    options.outFile = argv[2];

    try {
        Encoder enc(options);
        enc.run();
    }
    catch (const std::runtime_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
}
