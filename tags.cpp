#include "tags.h"
#include <iostream>
#include <vector>
#include "types.h"

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

void Tags::setCoverFile(const std::string &value, FileType type)
{
    mCoverFile = value;
    mCoverType = type;
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
