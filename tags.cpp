#include "tags.h"
#include <iostream>
#include <vector>
#include "types.h"

#if 0
struct Tag
{
    enum Type {
        Str = 0,
        Int = 1,
    };

    const char *mp4Name;
    Type        type = Type::Str;
};

static const std::map<const char *, Tag> TAGS {
    // clang-format off
    { "TITLE",              { "\251nam",    Tag::Type::Str } },
    { "ARTIST",             { "\251ART",    Tag::Type::Str } },
    { "ALBUM",              { "\251alb",    Tag::Type::Str } },
    { "COMMENT",            { "\251cmt",    Tag::Type::Str } },
    { "GENRE",              { "\251gen",    Tag::Type::Str } },
    { "DATE",               { "\251day",    Tag::Type::Str } },
    { "COMPOSER",           { "\251wrt",    Tag::Type::Str } },
    { "GROUPING",           { "\251grp",    Tag::Type::Str } },
    { "ALBUMARTIST",        { "aART",       Tag::Type::Str } },
    { "TRACKNUMBER",        { "trkn",       Tag::Type::Str } },
    { "DISCNUMBER",         { "disk",       Tag::Type::Str } },
    { "COMPILATION",        { "cpil",       Tag::Type::Str } },
    { "BPM",                { "tmpo",       Tag::Type::Str } },
    { "COPYRIGHT",          { "cprt",       Tag::Type::Str } },
    { "LYRICS",             { "\251lyr",    Tag::Type::Str } },
    { "ENCODEDBY",          { "\251too",    Tag::Type::Str } },
    { "ALBUMSORT",          { "soal",       Tag::Type::Str } },
    { "ALBUMARTISTSORT",    { "soaa",       Tag::Type::Str } },
    { "ARTISTSORT",         { "soar",       Tag::Type::Str } },
    { "TITLESORT",          { "sonm",       Tag::Type::Str } },
    { "COMPOSERSORT",       { "soco",       Tag::Type::Str } },
    { "SHOWSORT",           { "sosn",       Tag::Type::Str } },
    { "SHOWWORKMOVEMENT",   { "shwm",       Tag::Type::Str } },
    { "GAPLESSPLAYBACK",    { "pgap",       Tag::Type::Str } },
    { "PODCAST",            { "pcst",       Tag::Type::Str } },
    { "PODCASTCATEGORY",    { "catg",       Tag::Type::Str } },
    { "PODCASTDESC",        { "desc",  Tag::Type::Str } },
    { "PODCASTID",          { "egid",  Tag::Type::Str } },
    { "PODCASTURL",         { "purl",  Tag::Type::Str } },
    { "TVEPISODE",          { "tves",  Tag::Type::Str } },
    { "TVEPISODEID",        { "tven",  Tag::Type::Str } },
    { "TVNETWORK",          { "tvnn",  Tag::Type::Str } },
    { "TVSEASON",           { "tvsn",  Tag::Type::Str } },
    { "TVSHOW",             { "tvsh",  Tag::Type::Str } },
    { "WORK",               { "\251wrk",  Tag::Type::Str } },
    { "MOVEMENTNAME",       { "\251mvn",  Tag::Type::Str } },
    { "MOVEMENTNUMBER",     { "\251mvi",  Tag::Type::Str } },
    { "MOVEMENTCOUNT",      { "\251mvc",  Tag::Type::Str } },
    { "MUSICBRAINZ_TRACKID",  { "----:com.apple.iTunes:MusicBrainz Track Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_ARTISTID", { "----:com.apple.iTunes:MusicBrainz Artist Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_ALBUMID", { "----:com.apple.iTunes:MusicBrainz Album Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_ALBUMARTISTID", { "----:com.apple.iTunes:MusicBrainz Album Artist Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_RELEASEGROUPID", { "----:com.apple.iTunes:MusicBrainz Release Group Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_RELEASETRACKID", { "----:com.apple.iTunes:MusicBrainz Release Track Id",  Tag::Type::Str } },
    { "MUSICBRAINZ_WORKID", { "----:com.apple.iTunes:MusicBrainz Work Id",  Tag::Type::Str } },
    { "RELEASECOUNTRY", { "----:com.apple.iTunes:MusicBrainz Album Release Country",  Tag::Type::Str } },
    { "RELEASESTATUS", { "----:com.apple.iTunes:MusicBrainz Album Status",  Tag::Type::Str } },
    { "RELEASETYPE", { "----:com.apple.iTunes:MusicBrainz Album Type",  Tag::Type::Str } },
    { "ARTISTS", { "----:com.apple.iTunes:ARTISTS",  Tag::Type::Str } },
    { "ORIGINALDATE", { "----:com.apple.iTunes:originaldate",  Tag::Type::Str } },
    { "ASIN", { "----:com.apple.iTunes:ASIN",  Tag::Type::Str } },
    { "LABEL", { "----:com.apple.iTunes:LABEL",  Tag::Type::Str } },
    { "LYRICIST", { "----:com.apple.iTunes:LYRICIST",  Tag::Type::Str } },
    { "CONDUCTOR", { "----:com.apple.iTunes:CONDUCTOR",  Tag::Type::Str } },
    { "REMIXER", { "----:com.apple.iTunes:REMIXER",  Tag::Type::Str } },
    { "ENGINEER", { "----:com.apple.iTunes:ENGINEER",  Tag::Type::Str } },
    { "PRODUCER", { "----:com.apple.iTunes:PRODUCER",  Tag::Type::Str } },
    { "DJMIXER", { "----:com.apple.iTunes:DJMIXER",  Tag::Type::Str } },
    { "MIXER", { "----:com.apple.iTunes:MIXER",  Tag::Type::Str } },
    { "SUBTITLE", { "----:com.apple.iTunes:SUBTITLE",  Tag::Type::Str } },
    { "DISCSUBTITLE", { "----:com.apple.iTunes:DISCSUBTITLE",  Tag::Type::Str } },
    { "MOOD", { "----:com.apple.iTunes:MOOD",  Tag::Type::Str } },
    { "ISRC", { "----:com.apple.iTunes:ISRC",  Tag::Type::Str } },
    { "CATALOGNUMBER", { "----:com.apple.iTunes:CATALOGNUMBER",  Tag::Type::Str } },
    { "BARCODE", { "----:com.apple.iTunes:BARCODE",  Tag::Type::Str } },
    { "SCRIPT", { "----:com.apple.iTunes:SCRIPT",  Tag::Type::Str } },
    { "LANGUAGE", { "----:com.apple.iTunes:LANGUAGE",  Tag::Type::Str } },
    { "LICENSE", { "----:com.apple.iTunes:LICENSE",  Tag::Type::Str } },
    { "MEDIA", { "----:com.apple.iTunes:MEDIA",  Tag::Type::Str } },
    // clang-format on

};
/*


struct TagInfo
{
    const char *mp4Name;
    const char *userName;
};

static constexpr TagInfo TAG_NAMES[] = {
    { "\251nam", "TITLE" },
    { "\251ART", "ARTIST" },
    { "\251alb", "ALBUM" },
    { "\251cmt", "COMMENT" },
    { "\251gen", "GENRE" },
    { "\251day", "DATE" },
    { "\251wrt", "COMPOSER" },
    { "\251grp", "GROUPING" },
    { "aART", "ALBUMARTIST" },
    { "trkn", "TRACKNUMBER" },
    { "disk", "DISCNUMBER" },
    { "cpil", "COMPILATION" },
    { "tmpo", "BPM" },
    { "cprt", "COPYRIGHT" },
    { "\251lyr", "LYRICS" },
    { "\251too", "ENCODEDBY" },
    { "soal", "ALBUMSORT" },
    { "soaa", "ALBUMARTISTSORT" },
    { "soar", "ARTISTSORT" },
    { "sonm", "TITLESORT" },
    { "soco", "COMPOSERSORT" },
    { "sosn", "SHOWSORT" },
    { "shwm", "SHOWWORKMOVEMENT" },
    { "pgap", "GAPLESSPLAYBACK" },
    { "pcst", "PODCAST" },
    { "catg", "PODCASTCATEGORY" },
    { "desc", "PODCASTDESC" },
    { "egid", "PODCASTID" },
    { "purl", "PODCASTURL" },
    { "tves", "TVEPISODE" },
    { "tven", "TVEPISODEID" },
    { "tvnn", "TVNETWORK" },
    { "tvsn", "TVSEASON" },
    { "tvsh", "TVSHOW" },
    { "\251wrk", "WORK" },
    { "\251mvn", "MOVEMENTNAME" },
    { "\251mvi", "MOVEMENTNUMBER" },
    { "\251mvc", "MOVEMENTCOUNT" },
    { "----:com.apple.iTunes:MusicBrainz Track Id", "MUSICBRAINZ_TRACKID" },
    { "----:com.apple.iTunes:MusicBrainz Artist Id", "MUSICBRAINZ_ARTISTID" },
    { "----:com.apple.iTunes:MusicBrainz Album Id", "MUSICBRAINZ_ALBUMID" },
    { "----:com.apple.iTunes:MusicBrainz Album Artist Id", "MUSICBRAINZ_ALBUMARTISTID" },
    { "----:com.apple.iTunes:MusicBrainz Release Group Id", "MUSICBRAINZ_RELEASEGROUPID" },
    { "----:com.apple.iTunes:MusicBrainz Release Track Id", "MUSICBRAINZ_RELEASETRACKID" },
    { "----:com.apple.iTunes:MusicBrainz Work Id", "MUSICBRAINZ_WORKID" },
    { "----:com.apple.iTunes:MusicBrainz Album Release Country", "RELEASECOUNTRY" },
    { "----:com.apple.iTunes:MusicBrainz Album Status", "RELEASESTATUS" },
    { "----:com.apple.iTunes:MusicBrainz Album Type", "RELEASETYPE" },
    { "----:com.apple.iTunes:ARTISTS", "ARTISTS" },
    { "----:com.apple.iTunes:originaldate", "ORIGINALDATE" },
    { "----:com.apple.iTunes:ASIN", "ASIN" },
    { "----:com.apple.iTunes:LABEL", "LABEL" },
    { "----:com.apple.iTunes:LYRICIST", "LYRICIST" },
    { "----:com.apple.iTunes:CONDUCTOR", "CONDUCTOR" },
    { "----:com.apple.iTunes:REMIXER", "REMIXER" },
    { "----:com.apple.iTunes:ENGINEER", "ENGINEER" },
    { "----:com.apple.iTunes:PRODUCER", "PRODUCER" },
    { "----:com.apple.iTunes:DJMIXER", "DJMIXER" },
    { "----:com.apple.iTunes:MIXER", "MIXER" },
    { "----:com.apple.iTunes:SUBTITLE", "SUBTITLE" },
    { "----:com.apple.iTunes:DISCSUBTITLE", "DISCSUBTITLE" },
    { "----:com.apple.iTunes:MOOD", "MOOD" },
    { "----:com.apple.iTunes:ISRC", "ISRC" },
    { "----:com.apple.iTunes:CATALOGNUMBER", "CATALOGNUMBER" },
    { "----:com.apple.iTunes:BARCODE", "BARCODE" },
    { "----:com.apple.iTunes:SCRIPT", "SCRIPT" },
    { "----:com.apple.iTunes:LANGUAGE", "LANGUAGE" },
    { "----:com.apple.iTunes:LICENSE", "LICENSE" },
    { "----:com.apple.iTunes:MEDIA", "MEDIA" },
};
*/
Tags::Tags()
{
}

// static std::map<std::string, std::string> argsToMap(const std::vector<std::string> &args)
//{
//     using namespace std;
//     map<string, string> res;

//    for (const string &s : args) {
//        auto n = s.find('=');
//        if (n == string::npos) {
//            throw Error(s + ": tag field contains no '=' character");
//        }

//        res[s.substr(0, n)] = s.substr(n + 1, s.size());
//    }
//    return res;
//}

void Tags::parse(const std::vector<std::string> &args)
{
    using namespace std;

    for (const string &s : args) {
        auto n = s.find('=');
        if (n == string::npos) {
            throw Error(s + ": tag field contains no '=' character");
        }

        mStringTags TAGS.at(s.substr(0, n))
                res[s.substr(0, n)] = s.substr(n + 1, s.size());
    }

    //    using namespace std;
    //    map<string, string> req = argsToMap(args);

    //    for (const TagInfo &info : TAG_NAMES) {
    //        auto it = req.find()
    //    }
}

void Tags::printHelp(std::ostream &out)
{
    out << "You can use the following tag names:" << std::endl;

    for (const TagInfo &info : TAG_NAMES) {

        out << "{  \"" << info.userName << "\", \"" << info.mp4Name << "\" }," << std::endl;
        // out << "  " << info.userName << std::endl;
    }
}

#endif

std::string Tags::stringTag(const char *tag) const
{
    auto it = mStringTags.find(tag);
    if (it != mStringTags.end()) {
        return it->second;
    }
    return "";
}

void Tags::setStringTag(const char *tag, const std::string &value)
{
    mStringTags[tag] = value;
}

bool Tags::boolTag(const char *tag) const
{
    auto it = mBoolTags.find(tag);
    if (it != mBoolTags.end()) {
        return it->second;
    }
    return false;
}

void Tags::setBoolTag(const char *tag, bool value)
{
    mBoolTags[tag] = value;
}

static int genreIndex(const std::string &str)
{
    int res = 0;
    for (auto s : Tags::GENRES) {
        if (iequals(s, str)) {
            return res;
        }
        res++;
    }
    throw Error(str + ": Unknown genre");
}

void Tags::setGenre(const std::string &value)
{
    mGenre = genreIndex(value);
}

void Tags::setTrackNum(int track, int count)
{
    mTrackNum   = track;
    mTrackCount = count;
}

void Tags::setDiscNum(int disc, int count)
{
    mDiscNum   = disc;
    mDiscCount = count;
}
