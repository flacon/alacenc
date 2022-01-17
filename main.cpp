#include <iostream>
#include "encoder.h"
#include "vendor/docopt/docopt.h"
#include "tags.h"

static constexpr auto VERSION_STR = PROJECT_NAME " " PROJECT_VERSION;

static constexpr auto USAGE_TEXT =
        PROJECT_NAME " - " PROJECT_DESCRIPTION R"(

Usage:
  alacenc [options] [--] <INPUT_FILE> <OUTPUT_FILE>

Arguments:
  INPUT_FILE        Input file name,
                    when INPUT_FILE is -, read standard input
  OUTPUT_FILE       Output ALAC file name,
                    when OUTPUT_FILE is -, write to standard output

Options:
  -q --quiet               Produce no output to stderr
  -h --help                Print this help text and exit
  -V --version             Print the version number
  -f --fast                Fast mode. Encode a channel pair without
                           the search loop for maximum possible speed
  --artist <value>         Set artist name
  --album <value>          Set album/performer name
  --albumArtist <value>    Set album artist name
  --title <value>          Set title/track name
  --comment <value>        Set comment
  --genre <value>          Set genre
  --year <value>           Set year
  --songWriter <value>     Set song writer name
  --group <value>          Set group name
  --lyrics <value>         Set lyrics
  --compilation            Set track as part of a compilation
  --track <number/total>   Set track number
  --disc <number/total>    Set disc number
)";

std::tuple<int, int> splitNums(const std::string &s)
{
    if (s.empty()) {
        return std::make_tuple(0, 0);
    }

    auto n = s.find('/');
    if (n == std::string::npos) {
        throw Error(s + ": tag field contains no '/' character");
    }

    try {
        return std::make_tuple(stoi(s.substr(0, n)), stoi(s.substr(n + 1, s.size())));
    }
    catch (const std::invalid_argument &err) {
        throw Error(s + ": tag must have the format NUMBER/NUMBER");
    }
}

static Tags parseTags(const docopt::Options &args)
{
    Tags res;

    // clang-format off
    if (args.at("--artist").kind()      != docopt::Kind::Empty) { res.setArtist(args.at("--artist").asString());            }
    if (args.at("--album").kind()       != docopt::Kind::Empty) { res.setAlbum(args.at("--album").asString());              }
    if (args.at("--albumArtist").kind() != docopt::Kind::Empty) { res.setAlbumArtist(args.at("--albumArtist").asString());  }
    if (args.at("--title").kind()       != docopt::Kind::Empty) { res.setTitle(args.at("--title").asString());              }
    if (args.at("--comment").kind()     != docopt::Kind::Empty) { res.setComment(args.at("--comment").asString());          }
    if (args.at("--genre").kind()       != docopt::Kind::Empty) { res.setGenre(args.at("--genre").asString());              }
    if (args.at("--year").kind()        != docopt::Kind::Empty) { res.setDate(args.at("--year").asString());                }
    if (args.at("--songWriter").kind()  != docopt::Kind::Empty) { res.setSongWriter(args.at("--songWriter").asString());    }
    if (args.at("--group").kind()       != docopt::Kind::Empty) { res.setGroup(args.at("--group").asString());              }
    if (args.at("--lyrics").kind()      != docopt::Kind::Empty) { res.setLyrics(args.at("--lyrics").asString());            }
    if (args.at("--compilation").kind() != docopt::Kind::Empty) { res.setCompilation(args.at("--compilation").asBool());    }
    // clang-format on

    if (args.at("--track").kind() != docopt::Kind::Empty) {
        int n, t;
        std::tie(n, t) = splitNums(args.at("--track").asString());
        res.setTrackNum(n, t);
    }

    if (args.at("--disc").kind() != docopt::Kind::Empty) {
        int n, t;
        std::tie(n, t) = splitNums(args.at("--disc").asString());
        res.setDiscNum(n, t);
    }

    return res;
}

int main(int argc, const char **argv)
{
    docopt::Options args = docopt::docopt(USAGE_TEXT, { argv + 1, argv + argc }, true, VERSION_STR);
#if 0
    for (auto a : args) {
        std::cerr << a.first << " : ";
        // clang-format off
        switch (a.second.kind()) {
            case docopt::Kind::Empty:      std::cerr << "Empty";      break;
            case docopt::Kind::Bool:       std::cerr << "Bool";       break;
            case docopt::Kind::Long:       std::cerr << "Long";       break;
            case docopt::Kind::String:     std::cerr << "String";     break;
            case docopt::Kind::StringList: std::cerr << "StringList"; break;
        }
        // clang-format on
        std::cerr << std::endl;

        if (a.second.isStringList()) {
            for (auto s : a.second.asStringList()) {
                std::cerr << "  * " << s << std::endl;
            }
        }
    }
#endif

    Encoder::Options options;
    options.inFile       = args.at("<INPUT_FILE>").asString();
    options.outFile      = args.at("<OUTPUT_FILE>").asString();
    options.showProgress = !args.at("--quiet").asBool();
    options.fastMode     = args.at("--fast").asBool();

    try {
        Encoder enc(options);
        enc.setTags(parseTags(args));
        enc.run();
    }
    catch (const std::runtime_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        return 1;
    }
}
