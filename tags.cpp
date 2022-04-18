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
