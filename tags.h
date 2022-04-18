/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)MIT
 *
 * Flacon - audio File Encoder
 * https://github.com/flacon/flacon
 *
 * Copyright: 2022
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * MIT License
 *
 * Copyright (c) 2022 Alexander Sokoloff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef TAGS_H
#define TAGS_H

#include "types.h"
#include <string>
#include <iosfwd>
#include <map>

class Tags
{
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
    std::string genre() const       { return stringTag("\251gen"); }
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
    void setGenre(const std::string &value)         { setStringTag("\251gen", value); }
    void setCompilation(bool value)                 { setBoolTag("cpil", value); }
    // clang-format on

    int  trackNum() const { return mTrackNum; }
    int  trackCount() const { return mTrackCount; }
    void setTrackNum(int track, int count);

    int  discNum() const { return mDiscNum; }
    int  discCount() const { return mDiscCount; }
    void setDiscNum(int disc, int count);

    std::string coverFile() const { return mCoverFile; }
    FileType    coverType() const { return mCoverType; }
    void        setCoverFile(const std::string &value, FileType type);

    // Bytes asBytes() const;
    const std::map<std::string, std::string> &stringTags() const { return mStringTags; }
    const std::map<std::string, bool>        &boolTags() const { return mBoolTags; }

private:
    int mTrackNum   = 0;
    int mTrackCount = 0;

    int mDiscNum   = 0;
    int mDiscCount = 0;

    bool mCompilation = false;
    int  mGenre       = 0;

    std::string                        mCoverFile;
    FileType                           mCoverType = FileType::Unknown;
    std::map<std::string, std::string> mStringTags;
    std::map<std::string, bool>        mBoolTags;
};

#endif // TAGS_H
