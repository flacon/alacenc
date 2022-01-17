#ifndef TAGS_H
#define TAGS_H

#include "types.h"
#include <string>
#include <iosfwd>
#include <map>

class Tags
{
public:
    static constexpr const char *GENRES[] = {
        "",
        "Blues",
        "Classic Rock",
        "Country",
        "Dance",
        "Disco",
        "Funk",
        "Grunge",
        "Hip-Hop",
        "Jazz",
        "Metal",
        "New Age",
        "Oldies",
        "Other",
        "Pop",
        "R&B",
        "Rap",
        "Reggae",
        "Rock",
        "Techno",
        "Industrial",
        "Alternative",
        "Ska",
        "Death Metal",
        "Pranks",
        "Soundtrack",
        "Euro-Techno",
        "Ambient",
        "Trip-Hop",
        "Vocal",
        "Jazz-Funk",
        "Fusion",
        "Trance",
        "Classical",
        "Instrumental",
        "Acid",
        "House",
        "Game",
        "Sound Clip",
        "Gospel",
        "Noise",
        "Alternative Rock",
        "Bass",
        "Soul",
        "Punk",
        "Space",
        "Meditative",
        "Instrumental Pop",
        "Instrumental Rock",
        "Ethnic",
        "Gothic",
        "Darkwave",
        "Techno-Industrial",
        "Electronic",
        "Pop-Folk",
        "Eurodance",
        "Dream",
        "Southern Rock",
        "Comedy",
        "Cult",
        "Gangsta",
        "Top 40",
        "Christian Rap",
        "Pop/Funk",
        "Jungle",
        "Native American",
        "Cabaret",
        "New Wave",
        "Psychedelic",
        "Rave",
        "Showtunes",
        "Trailer",
        "Lo-Fi",
        "Tribal",
        "Acid Punk",
        "Acid Jazz",
        "Polka",
        "Retro",
        "Musical",
        "Rock & Roll",
        "Hard Rock",
        "Folk",
        "Folk Rock",
        "National Folk",
        "Swing",
        "Fast Fusion",
        "Bebop",
        "Latin",
        "Revival",
        "Celtic",
        "Bluegrass",
        "Avant-garde",
        "Gothic Rock",
        "Progressive Rock",
        "Psychedelic Rock",
        "Symphonic Rock",
        "Slow Rock",
        "Big Band",
        "Chorus",
        "Easy Listening",
        "Acoustic",
        "Humour",
        "Speech",
        "Chanson",
        "Opera",
        "Chamber Music",
        "Sonata",
        "Symphony",
        "Booty Bass",
        "Primus",
        "Porn Groove",
        "Satire",
        "Slow Jam",
        "Club",
        "Tango",
        "Samba",
        "Folklore",
        "Ballad",
        "Power Ballad",
        "Rhythmic Soul",
        "Freestyle",
        "Duet",
        "Punk Rock",
        "Drum Solo",
        "A Cappella",
        "Euro-House",
        "Dancehall",
        "Goa",
        "Drum & Bass",
        "Club-House",
        "Hardcore Techno",
        "Terror",
        "Indie",
        "Britpop",
        "Worldbeat",
        "Polsk Punk",
        "Beat",
        "Christian Gangsta Rap",
        "Heavy Metal",
        "Black Metal",
        "Crossover",
        "Contemporary Christian",
        "Christian Rock",
        "Merengue",
        "Salsa",
        "Thrash Metal",
        "Anime",
        "Jpop",
        "Synthpop",
        "Abstract",
        "Art Rock",
        "Baroque",
        "Bhangra",
        "Big Beat",
        "Breakbeat",
        "Chillout",
        "Downtempo",
        "Dub",
        "EBM",
        "Eclectic",
        "Electro",
        "Electroclash",
        "Emo",
        "Experimental",
        "Garage",
        "Global",
        "IDM",
        "Illbient",
        "Industro-Goth",
        "Jam Band",
        "Krautrock",
        "Leftfield",
        "Lounge",
        "Math Rock",
        "New Romantic",
        "Nu-Breakz",
        "Post-Punk",
        "Post-Rock",
        "Psytrance",
        "Shoegaze",
        "Space Rock",
        "Trop Rock",
        "World Music",
        "Neoclassical",
        "Audiobook",
        "Audio Theatre",
        "Neue Deutsche Welle",
        "Podcast",
        "Indie Rock",
        "G-Funk",
        "Dubstep",
        "Garage Rock",
        "Psybient"
    };

public:
    Tags() = default;
    void parse(const std::vector<std::string> &args);

    // static void printHelp(std::ostream &out);

    std::string stringTag(const char *tag) const;
    void        setStringTag(const char *tag, const std::string &value);

    bool boolTag(const char *tag) const;
    void setBoolTag(const char *tag, bool value);

    // clang-format off
    std::string artist() const      { return stringTag("\251ART"); }
    std::string album() const       { return stringTag("\251alb"); }
    std::string albumArtist() const { return stringTag("aART"); }
    std::string title() const       { return stringTag("\251nam"); }
    std::string comment() const     { return stringTag("\251cmt"); }
    std::string date() const        { return stringTag("\251day"); }
    std::string songWriter() const  { return stringTag("\251wrt"); }
    std::string group() const       { return stringTag("\251grp"); }
    std::string lyrics() const      { return stringTag("\251lyr"); }
    bool        compilation() const { return boolTag("cpil"); }

    void setArtist(const std::string &value)        { setStringTag("\251ART", value); }
    void setAlbum(const std::string &value)         { setStringTag("\251alb", value); }
    void setAlbumArtist(const std::string &value)   { setStringTag("aART", value); }
    void setTitle(const std::string &value)         { setStringTag("\251nam", value); }
    void setComment(const std::string &value)       { setStringTag("\251cmt", value); }
    void setDate(const std::string &value)          { setStringTag("\251day", value); }
    void setSongWriter(const std::string &value)    { setStringTag("\251wrt", value); }
    void setGroup(const std::string &value)         { setStringTag("\251grp", value); }
    void setLyrics(const std::string &value)        { setStringTag("\251lyr", value); }
    void setCompilation(bool value)                 { setBoolTag("cpil", value); }
    // clang-format on

    int  trackNum() const { return mTrackNum; }
    int  trackCount() const { return mTrackCount; }
    void setTrackNum(int track, int count);

    int  discNum() const { return mDiscNum; }
    int  discCount() const { return mDiscCount; }
    void setDiscNum(int disc, int count);

    std::string genre() const { return GENRES[mGenre]; }
    uint16_t    genreNum() const { return mGenre; }
    void        setGenre(const std::string &value);

    // Bytes asBytes() const;
    const std::map<std::string, std::string> &stringTags() const { return mStringTags; }
    const std::map<std::string, bool> &       boolTags() const { return mBoolTags; }

private:
    int mTrackNum   = 0;
    int mTrackCount = 0;

    int mDiscNum   = 0;
    int mDiscCount = 0;

    bool mCompilation = false;
    int  mGenre       = 0;

    std::map<std::string, std::string> mStringTags;
    std::map<std::string, bool>        mBoolTags;
};

#endif // TAGS_H
