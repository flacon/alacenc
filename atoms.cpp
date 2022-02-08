#include "atoms.h"
#include <array>
#include "types.h"
#include <iostream>
#include "vendor/alac/codec/ALACAudioTypes.h"
#include "encoder.h"
#include <assert.h>
#include <cmath>

// https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html

OutFile &operator<<(OutFile &os, const Atom &atom)
{
    assert(!atom.typeId.isEmpty() && "Type TAG is empty");

    os << uint32_t(atom.size());
    os << atom.typeId;
    os << atom.data;
    for (auto a : atom.subAtoms) {
        os << a;
    }
    return os;
}

Bytes &operator<<(Bytes &out, const Atom &atom)
{
    assert(!atom.typeId.isEmpty() && "Type TAG is empty");

    out << uint32_t(atom.size());
    out << atom.typeId[0] << atom.typeId[1] << atom.typeId[2] << atom.typeId[3];
    out << atom.data;
    for (auto a : atom.subAtoms) {
        out << a;
    }
    return out;
}

/************************************************
 * SubAtoms
 ************************************************/
inline std::vector<Atom> &operator<<(std::vector<Atom> &arr, const Atom &atom)
{
    arr.push_back(atom);
    return arr;
}

/************************************************
 * Atom
 ************************************************/
size_t Atom::size() const
{
    size_t res = 8; // Size + Type
    res += data.size();
    for (auto a : subAtoms) {
        res += a.size();
    }
    return res;
}

Atom::TypeId::TypeId(const char type[5]) :
    std::array<char, 4>({ type[0], type[1], type[2], type[3] })
{
}

bool Atom::TypeId::isEmpty() const
{
    for (const char c : *this) {
        if (c != ' ') {
            return false;
        }
    }
    return true;
}

/************************************************
 * Atom_ftyp
 ************************************************/
FtypAtom::FtypAtom()
{
    typeId = "ftyp";

    data << "M4A ";             // Major_Brand
    data << "\x00\x00\x02\x00"; // Minor_Version

    // Compatible_Brands
    data << "M4A ";
    data << "isom";
    data << "iso2";
}

/************************************************
 * MoovAtom
 ************************************************/
MoovAtom::MoovAtom(const Encoder &encoder)
{
    typeId = "moov";
    subAtoms.push_back(MvhdAtom(encoder));
    subAtoms.push_back(TrakAtom(encoder));
    subAtoms.push_back(UdtaAtom(encoder));
}

/************************************************
 * You use the movie header atom to specify the characteristics of an entire QuickTime movie.
 * The data contained in this atom defines characteristics of the entire QuickTime movie,
 * such as time scale and duration. It has an atom type value of 'mvhd'.
 *
 * See https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html
 ************************************************/
MvhdAtom::MvhdAtom(const Encoder &encoder)
{
    typeId = "mvhd";

    // Version  A 1-byte specification of the version of this movie header atom.
    data << '\0';

    // Flags    Three bytes of space for future movie header flags.
    data << '\0' << '\0' << '\0';

    // Creation time    A 32-bit integer that specifies the calendar date and time (in seconds since midnight, January 1, 1904)
    // when the movie atom was created. It is strongly recommended that this value should be specified using coordinated universal time (UTC).

    data << uint32_t(0);

    // Modification time
    // A 32-bit integer that specifies the calendar date and time (in seconds since midnight, January 1, 1904) when the movie atom was changed. BooleanIt is strongly recommended that this value should be specified using coordinated universal time (UTC).
    data << uint32_t(0);

    // Time scale
    // A time value that indicates the time scale for this movie—that is, the number of time units that pass per second in its time coordinate system. A time coordinate system that measures time in sixtieths of a second, for example, has a time scale of 60.
    data << uint32_t(1000); // header.sampleRate());

    // Duration
    // A time value that indicates the duration of the movie in time scale units. Note that this property is derived from the movie’s tracks.
    // The value of this field corresponds to the duration of the longest track in the movie.
    data << uint32_t(std::ceil(encoder.inputWavHeader().dataSize() * 1000.0 / encoder.inputWavHeader().byteRate()));

    // Preferred rate
    // A 32-bit fixed-point number that specifies the rate at which to play this movie. A value of 1.0 indicates normal rate.
    data << uint32_t(1 << 16);

    // Preferred volume
    // A 16-bit fixed-point number that specifies how loud to play this movie’s sound. A value of 1.0 indicates full volume.
    data << uint16_t(1 << 8);

    // Reserved
    // Ten bytes reserved for use by Apple. Set to 0.
    data << '\0' << '\0' << '\0' << '\0' << '\0' << '\0' << '\0' << '\0' << '\0' << '\0';

    // Matrix structure
    // The matrix structure associated with this movie. A matrix shows how to map points from one coordinate space into another. See Matrices for a discussion of how display matrices are used in QuickTime.
    data << uint32_t(0x00010000) << uint32_t(0x00000000) << uint32_t(0x00000000);
    data << uint32_t(0x00000000) << uint32_t(0x00010000) << uint32_t(0x00000000);
    data << uint32_t(0x00000000) << uint32_t(0x00000000) << uint32_t(0x40000000);

    // Preview time
    // The time value in the movie at which the preview begins.
    data << uint32_t(0);

    // Preview duration
    // The duration of the movie preview in movie time scale units.
    data << uint32_t(0);

    // Poster time
    // The time value of the time of the movie poster.
    data << uint32_t(0);

    // Selection time
    // The time value for the start time of the current selection.
    data << uint32_t(0);

    // Selection duration
    // The duration of the current selection in movie time scale units.
    data << uint32_t(0);

    // Current time
    // The time value for current time position within the movie.
    data << uint32_t(0);

    // Next track ID
    // A 32-bit integer that indicates a value to use for the track ID number of the next track added to this movie. Note that 0 is not a valid track ID value.
    data << uint32_t(2);
}

/************************************************
 * TrakAtom
 ************************************************/
TrakAtom::TrakAtom(const Encoder &encoder)
{
    typeId = "trak";
    subAtoms.push_back(TkhdAtom(encoder));
    subAtoms.push_back(MdiaAtom(encoder));
}

/************************************************
 * TkhdAtom
 ************************************************/
TkhdAtom::TkhdAtom(const Encoder &encoder)
{
    typeId = "tkhd";

    // Version
    // A 1-byte specification of the version of this track header.
    data << char(0);

    // Flags
    // Three bytes that are reserved for the track header flags. These flags indicate how the track is used in the movie. The following flags are valid (all flags are enabled when set to 1).
    enum Flag : uint16_t {
        Enabled   = 0x0001, // Indicates that the track is enabled. Flag value is 0x0001.
        InMovie   = 0x0002, //                    Indicates that the track is used in the movie. Flag value is 0x0002.
        InPreview = 0x0004, //                    Indicates that the track is used in the movie’s preview. Flag value is 0x0004.
        InPoster  = 0x0008, //                     Indicates that the track is used in the movie’s poster. Flag value is 0x0008.
    };
    data << char(0) << uint16_t(Flag::Enabled | Flag::InMovie);

    // Creation time
    // A 32-bit integer that indicates the calendar date and time (expressed in seconds since midnight, January 1, 1904) when the track header was created. It is strongly recommended that this value should be specified using coordinated universal time (UTC).
    data << uint32_t(0);

    // Modification time
    // A 32-bit integer that indicates the calendar date and time (expressed in seconds since midnight, January 1, 1904) when the track header was changed. It is strongly recommended that this value should be specified using coordinated universal time (UTC).
    data << uint32_t(0);

    // Track ID
    // A 32-bit integer that uniquely identifies the track. The value 0 cannot be used.
    data << uint32_t(1);

    // Reserved
    // A 32-bit integer that is reserved for use by Apple. Set this field to 0.
    data << uint32_t(0);

    // Duration
    // A time value that indicates the duration of this track (in the movie’s time coordinate system). Note that this property is derived from the track’s edits. The value of this field is equal to the sum of the durations of all of the track’s edits. If there is no edit list, then the duration is the sum of the sample durations, converted into the movie timescale.
    data << uint32_t(encoder.inputWavHeader().duration());

    // Reserved
    // An 8-byte value that is reserved for use by Apple. Set this field to 0.
    data << uint32_t(0) << uint32_t(0);

    // Layer
    // A 16-bit integer that indicates this track’s spatial priority in its movie.
    // The QuickTime Movie Toolbox uses this value to determine how tracks overlay one another.
    // Tracks with lower layer values are displayed in front of tracks with higher layer values.
    data << uint16_t(0);

    // Alternate group
    // A 16-bit integer that identifies a collection of movie tracks that contain alternate data for one another. This same identifier appears in each 'tkhd' atom of the other tracks in the group. QuickTime chooses one track from the group to be used when the movie is played. The choice may be based on such considerations as playback quality, language, or the capabilities of the computer.
    data << uint16_t(1);

    // Volume
    // A 16-bit fixed-point value that indicates how loudly this track’s sound is to be played. A value of 1.0 indicates normal volume.
    data << uint16_t(1 << 8);

    // Reserved
    // A 16-bit integer that is reserved for use by Apple. Set this field to 0.
    data << uint16_t(0);

    // Matrix structure
    // The matrix structure associated with this track. See Figure 2-3 for an illustration of a matrix structure.
    data << uint32_t(0x00010000) << uint32_t(0x00000000) << uint32_t(0x00000000);
    data << uint32_t(0x00000000) << uint32_t(0x00010000) << uint32_t(0x00000000);
    data << uint32_t(0x00000000) << uint32_t(0x00000000) << uint32_t(0x40000000);

    // Track width
    // A 32-bit fixed-point number that specifies the width of this track in pixels.
    data << uint32_t(0);

    // Track height
    // A 32-bit fixed-point number that indicates the height of this track in pixels.
    data << uint32_t(0);
}

/************************************************
 * MdiaAtom
 ************************************************/
MdiaAtom::MdiaAtom(const Encoder &encoder)
{
    typeId = "mdia";
    subAtoms << MdhdAtom(encoder);
    subAtoms << HdlrAtom(encoder);
    subAtoms << MinfAtom(encoder);
}

/************************************************
 * MdhdAtom
 ************************************************/
MdhdAtom::MdhdAtom(const Encoder &encoder)
{
    typeId = "mdhd";

    // Version
    // One byte that specifies the version of this header atom.
    data << '\0';

    // Flags
    // Three bytes of space for media header flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Creation time
    // A 32-bit integer that specifies (in seconds since midnight, January 1, 1904) when the media atom was created. It is strongly recommended that this value should be specified using coordinated universal time (UTC).
    data << uint32_t(0);

    // Modification time
    // A 32-bit integer that specifies (in seconds since midnight, January 1, 1904) when the media atom was changed. It is strongly recommended that this value should be specified using coordinated universal time (UTC).
    data << uint32_t(0);

    // Time scale
    // A time value that indicates the time scale for this media—that is, the number of time units that pass per second in its time coordinate system.
    data << uint32_t(encoder.inputWavHeader().sampleRate());

    // Duration
    // The duration of this media in units of its time scale.
    data << uint32_t(encoder.inputWavHeader().duration() / encoder.inputWavHeader().sampleRate() / 1000.0);

    // Language
    // A 16-bit integer that specifies the language code for this media. See Language Code Values for valid language codes.
    // Also see Extended Language Tag Atom for the preferred code to use here if an extended language tag is also included in the media atom.
    data << uint16_t(0);

    // Quality
    // A 16-bit integer that specifies the media’s playback quality—that is, its suitability for playback in a given environment.
    data << uint16_t(0);
}

/************************************************
 * HdlrAtom
 * The handler reference atom specifies the media handler component that is to be used to interpret the media’s data.
 ************************************************/
HdlrAtom::HdlrAtom(const Encoder &)
{
    typeId = "hdlr";

    // Version
    // A 1-byte specification of the version of this handler information.
    data << '\0';

    // Flags
    // A 3-byte space for handler information flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Component type
    // A four-character code that identifies the type of the handler. Only two values are valid for this field: 'mhlr' for media handlers and 'dhlr' for data handlers.
    data << "\0\0\0\0";

    // Component subtype
    // A four-character code that identifies the type of the media handler or data handler.
    // For media handlers, this field defines the type of data—for example, 'vide' for video data, 'soun' for sound data or ‘subt’ for subtitles.
    // See Media Data Atom Types for information about defined media data types.
    data << "soun";

    // Component manufacturer
    // Reserved. Set to 0.
    data << uint32_t(0);

    // Component flags
    // Reserved. Set to 0.
    data << uint32_t(0);

    // Component flags mask
    // Reserved. Set to 0.
    data << uint32_t(0);

    // Component name
    // A (counted) string that specifies the name of the component—that is,
    // the media handler used when this media was created.
    // This field may contain a zero-length (empty) string.
    // data << "\0\0\0\0";
    data << std::string("SoundHandler") << '\0';
    // data << "dHandler";
}

/************************************************
 * MinfAtom
 *
 * Video media information atoms are the highest-level atoms in video media.
 * These atoms contain a number of other atoms that define specific characteristics
 * of the video media data. Figure 2-24 shows the layout of a video media information atom.
 ************************************************/
MinfAtom::MinfAtom(const Encoder &encoder)
{
    typeId = "minf";
    subAtoms << SmhdAtom(encoder);
    subAtoms << DinfAtom(encoder);
    subAtoms << StblAtom(encoder);
}

/************************************************
 * SmhdAtom
 *
 * The sound media information header atom stores the
 * sound media’s control information, such as balance.
 ************************************************/
SmhdAtom::SmhdAtom(const Encoder &)
{
    typeId = "smhd";

    // Version
    // A 1-byte specification of the version of this sound media information header atom.
    data << '\0';

    // Flags
    // A 3-byte space for sound media information flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Balance
    // A 16-bit integer that specifies the sound balance of this sound media. Sound balance is the setting that controls the mix of sound between the two speakers of a computer. This field is normally set to 0. See Balance for more information about balance values.
    data << uint16_t(0);

    // Reserved
    // Reserved for use by Apple. Set this field to 0.
    data << uint16_t(0);
}

/************************************************
 * DinfAtom
 *
 * Data Information Atom
 ************************************************/
DinfAtom::DinfAtom(const Encoder &encoder)
{
    typeId = "dinf";
    subAtoms << DrefAtom(encoder);
}

/************************************************
 * DrefAtom
 *
 * Data reference atoms contain tabular data that
 * instructs the data handler component how to access the media’s data.
 ************************************************/
DrefAtom::DrefAtom(const Encoder &)
{
    struct Url : public Atom
    {
        Url()
        {
            typeId = "url ";

            // Version
            // A 1-byte specification of the version of the data reference.
            data << '\0';

            // Flags
            // A 3-byte space for data reference flags. There is one defined flag.
            //  Self reference - This flag indicates that the media’s data is in the
            //                   same file as the movie atom. On the Macintosh, and other
            //                   file systems with multi-fork files, set this flag to 1 even
            //                   if the data resides in a different fork from the movie atom.
            //                   This flag’s value is 0x0001.
            data << '\0' << '\0' << '\1';
        }
    };

    typeId = "dref";

    // Version
    // A 1-byte specification of the version of this data reference atom.
    data << '\0';

    // Flags
    // A 3-byte space for data reference flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Number of entries
    // A 32-bit integer containing the count of data references that follow.
    data << uint32_t(1);

    // Data references
    // An array of data references.
    Url url;
    data << uint32_t(url.size());
    data << url.typeId.data();
    data << url.data;
}

/************************************************
 * StblAtom
 *
 * Sample Table Atom
 * The sample table atom contains information for converting
 * from media time to sample number to sample location.
 * This atom also indicates how to interpret the sample
 * (for example, whether to decompress the video data and, if so, how).
 ************************************************/
StblAtom::StblAtom(const Encoder &encoder)
{
    typeId = "stbl";
    subAtoms << StsdAtom(encoder);
    subAtoms << StscAtom(encoder);
    subAtoms << StcoAtom(encoder);
    subAtoms << StszAtom(encoder);
    subAtoms << SttsAtom(encoder);
}

/************************************************
 * StsdAtom
 *
 * The sample description atom stores information that allows you
 * to decode samples in the media.
 * The data stored in the sample description varies, depending on the media type.
 ************************************************
 * See vendor/alac/ALACMagicCookieDescription.txt
 *
 * ---------------- SoundDescriptionBox (FullBox) ----------------------------
 *
 * SampleEntrySize              // = sizeof(SoundDescriptionBox)(16) + sizeof (AudioSampleEntry)(AudioSampleEntry.SampleEntrySize)
 * SampleEntryType              // = 'stsd'
 * VersionFlags                 // = 0
 * EntryCount                   // = 1
 *
 * ---------------- Audio Sample Entry (REQUIRED) -----------------------------
 *
 * SampleEntrySize              // sizeof(AudioSampleEntry)(36) + sizeof(full ISO box header)(12) + sizeof(Apple Lossless Magic Cookie)
 * SampleEntryType              // = 'alac', specifies that the AudioSampleEntry describes an Apple Lossless bitstream
 * mReserved[6]                 // = 0
 * dref index                   // = 1
 * reserved[2]                  // = 0
 * channel count                // = number of channels as a uint_16 value
 * sample size                  // = source pcm bitdepth (example = 16bit source pcm)
 * predefined                   // = 0
 * reserved                     // = 0
 * sample rate                  // sample rate as a uint_32 value
 *
 * Appended to AudioSampleEntry:
 *
 * ALAC Specific Info Size      // uint_32 value, = 36 (12 + sizeof(ALACSpecificConfig))
 * ALAC Specific Info ID        // uint_32 value, = 'alac', format ID which matches the Audio Sample Entry SampleEntryType field
 * Version Flags                // uint_32 value, = 0
 *
 * Apple Lossless Magic Cookie  // 'magic cookie' vended from ALAC encoder (24 or 48 Bytes)
 ************************************************/
StsdAtom::StsdAtom(const Encoder &encoder)
{
    typeId = "stsd";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Number of entries
    // A 32-bit integer containing the number of sample descriptions that follow.
    data << uint32_t(1);

    // ------------------------------------------
    // Sample description table
    // An array of sample descriptions. For details, see General Structure of a Sample Description.

    // Sample description size
    data << uint32_t(72);

    // Data format
    // A 32-bit integer indicating the format of the stored data.
    data << "alac";

    // Reserved
    // Six bytes that must be set to 0.
    data << '\0' << '\0' << '\0' << '\0' << '\0' << '\0';

    // Data reference index
    // A 16 - bit integer that contains the index of the data reference to
    // use to retrieve data associated with samples that use this sample description.
    data << uint16_t(1);

    // ------------------------------------------
    // Sound Sample Description

    // Version
    // A 16-bit integer that holds the sample description version (currently 0 or 1).
    data << uint16_t(0);

    // Revision level
    // A 16-bit integer that must be set to 0.
    data << uint16_t(0);

    // Vendor
    // A 32-bit integer that must be set to 0.
    data << uint32_t(0);

    // Number of channels
    // A 16-bit integer that indicates the number of sound channels used by the sound sample. Set to 1 for monaural sounds, 2 for stereo sounds. Higher numbers of channels are not supported.
    data << uint16_t(encoder.inputWavHeader().numChannels());

    // Sample size (bits)
    // A 16-bit integer that specifies the number of bits in each uncompressed sound sample. Allowable values are 8 or 16. Formats using more than 16 bits per sample set this field to 16 and use sound description version 1.
    data << uint16_t(encoder.inputWavHeader().bitsPerSample());

    // Compression ID
    // A 16-bit integer that must be set to 0 for version 0 sound descriptions. This may be set to –2 for some version 1 sound descriptions; see Redefined Sample Tables.
    data << uint16_t(0);

    // Packet size
    // A 16-bit integer that must be set to 0.
    data << uint16_t(0);

    // Sample rate
    // A 32-bit unsigned fixed-point number (16.16) that indicates the rate at which the sound samples were obtained. The integer portion of this number should match the media’s time scale. Many older version 0 files have values of 22254.5454 or 11127.2727, but most files have integer values, such as 44100. Sample rates greater than 2^16 are not supported.
    data << uint32_t(encoder.inputWavHeader().sampleRate() << 16);

    // ------------------------------------------
    // Magic Cookie
    std::vector<char> magicCookie = encoder.getMagicCookie();

    // Size
    data << uint32_t(magicCookie.size() + 12);

    // ID
    data << "alac";

    // Version?
    data << uint32_t(0);

    for (const char &c : magicCookie) {
        data.push_back(c);
    }
}

OutFile &operator<<(OutFile &os, const FreeAtom &atom)
{
    os << uint32_t(atom.mSize);
    os << "free";
    for (uint i = 0; i < atom.mSize - 8; ++i) {
        os << '\0';
    }
    return os;
}

/// Sample-to-Chunk Atoms
/// The sample-to-chunk atom contains a table that maps samples to chunks in the media data stream.
///
StscAtom::StscAtom(const Encoder &encoder)
{
    typeId = "stsc";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Number of entries
    // A 32-bit integer containing the number of sample descriptions that follow.
    data << uint32_t(1);

    // Sample-to-chunk table
    // * 32bit - First chunk
    // * 32bit - Samples per chunk
    // * 32bit - Sample description ID

    // I have not found the logic by which samples are combined into chunks,
    // so I use one chunk for all samples.
    // If you have more information, tell me.
    data << uint32_t(1);
    data << uint32_t(encoder.sampleSizeTable().size());
    data << uint32_t(1);
}

/// Chunk Offset Atoms
/// Chunk offset atoms identify the location of each chunk of data in the media’s data stream.
StcoAtom::StcoAtom(const Encoder &encoder)
{
    typeId = "stco";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Number of entries
    data << uint32_t(1);

    // Chunk offset table
    // A chunk offset table consisting of an array of offset values.
    // There is one table entry for each chunk in the media.
    // Offsets are file offsets, not the offset into any atom.
    data << uint32_t(encoder.audioDataStartPos());
}

/// Sample Size Atom
/// The sample size atom contains the sample count and a table giving the size of each sample.
StszAtom::StszAtom(const Encoder &encoder)
{
    typeId = "stsz";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // Sample size
    data << uint32_t(0);

    // Number of entries
    data << uint32_t(encoder.sampleSizeTable().size());

    // Sample size table
    // A table containing the sample size, in bytes, for every sample in the media’s data stream.
    for (uint32_t sz : encoder.sampleSizeTable()) {
        data << uint32_t(sz);
    }
}

/// Time-to-Sample Atoms
/// The atom contains a compact version of a table that allows indexing from time to sample number.
/// Each entry in the table gives the number of consecutive samples with the same time delta, and
/// the delta of those samples.
SttsAtom::SttsAtom(const Encoder &encoder)
{
    typeId = "stts";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // uint32_t total = encoder.inputWavHeader().dataSize() / encoder.inFormat().mChannelsPerFrame / (encoder.inFormat().mBitsPerChannel / 8);
    uint32_t full = encoder.sampleSize() / encoder.inFormat().mChannelsPerFrame / (encoder.inFormat().mBitsPerChannel / 8);
    uint32_t last = (encoder.inputWavHeader().dataSize() / encoder.inFormat().mChannelsPerFrame / (encoder.inFormat().mBitsPerChannel / 8)) - full * (encoder.sampleSizeTable().size() - 1);

    if (last < full) {
        // Number of entries
        data << uint32_t(2);

        // Time-to-sample table
        // Sample count + Sample duration
        data << uint32_t(encoder.sampleSizeTable().size() - 1);
        data << uint32_t(full);

        data << uint32_t(1);
        data << uint32_t(last);
    }
    else {
        // Number of entries
        data << uint32_t(1);

        // Time-to-sample table
        // Sample count + Sample duration
        data << uint32_t(encoder.sampleSizeTable().size());
        data << uint32_t(full);
    }
}

UdtaAtom::UdtaAtom(const Encoder &encoder)
{
    typeId = "udta";
    subAtoms.push_back(MetaAtom(encoder));
}

enum class AtomDataType {
    Implicit  = 0,  // for use with tags for which no type needs to be indicated because only one type is allowed
    UTF8      = 1,  // without any count or null terminator
    UTF16     = 2,  // also known as UTF-16BE
    SJIS      = 3,  // deprecated unless it is needed for special Japanese characters
    HTML      = 6,  // the HTML file header specifies which HTML version
    XML       = 7,  // the XML header must identify the DTD or schemas
    UUID      = 8,  // also known as GUID; stored as 16 bytes in binary (valid as an ID)
    ISRC      = 9,  // stored as UTF-8 text (valid as an ID)
    MI3P      = 10, // stored as UTF-8 text (valid as an ID)
    GIF       = 12, // (deprecated) a GIF image
    JPEG      = 13, // a JPEG image
    PNG       = 14, // a PNG image
    URL       = 15, // absolute, in UTF-8 characters
    Duration  = 16, // in milliseconds, 32-bit integer
    DateTime  = 17, // in UTC, counting seconds since midnight, January 1, 1904; 32 or 64-bits
    Genred    = 18, // a list of enumerated values
    Int       = 21, // a signed big-endian integer with length one of { 1,2,3,4,8 } bytes
    RIAAPA    = 24, // RIAA parental advisory; { -1=no, 1=yes, 0=unspecified }, 8-bit integer
    UPC       = 25, // Universal Product Code, in text UTF-8 format (valid as an ID)
    BMP       = 27, // Windows bitmap image
    Undefined = 255 // undefined
};

void addTag(Atom &out, const std::string &key, const std::string &value)
{
    if (value.empty()) {
        return;
    }

    out.data << uint32_t(value.length() + 24); // Size
    out.data << key;                           // Tag key
    out.data << uint32_t(value.length() + 16); // Size
    out.data << "data";                        //
    out.data << uint32_t(AtomDataType::UTF8);  // Data type
    out.data << uint32_t(0);                   // Language ?
    out.data << value;                         // Tag value
};

void addTag(Atom &out, const std::string &key, bool value)
{
    if (!value) {
        return;
    }
    out.data << uint32_t(1 + 24);            // Size
    out.data << key;                         // Tag key
    out.data << uint32_t(1 + 16);            // Size
    out.data << "data";                      //
    out.data << uint32_t(AtomDataType::Int); // Data type
    out.data << uint32_t(0);                 // Language ?
    out.data << (value ? '\1' : '\0');       // Value
}

void addTag(Atom &out, const std::string &key, uint16_t value)
{
    out.data << uint32_t(2 + 24);                 // Size
    out.data << key;                              // Tag key
    out.data << uint32_t(2 + 16);                 // Size
    out.data << "data";                           //
    out.data << uint32_t(AtomDataType::Implicit); // Data type
    out.data << uint32_t(0);                      // Language ?
    out.data << uint16_t(value);                  // Value
}

struct DataAtom : public Atom
{
    DataAtom()
    {
        typeId = "data";
    }
};

TrknAtom::TrknAtom(const Encoder &encoder)
{
    typeId = "trkn";

    DataAtom dataAtom;
    dataAtom.data << uint32_t(AtomDataType::Implicit); // Data type
    dataAtom.data << uint32_t(0);                      // Language ?
    dataAtom.data << uint16_t(0);
    dataAtom.data << uint16_t(encoder.tags().trackNum());
    dataAtom.data << uint16_t(encoder.tags().trackCount());
    dataAtom.data << uint16_t(0);
    subAtoms << dataAtom;
}

DiskAtom::DiskAtom(const Encoder &encoder)
{
    typeId = "disk";

    DataAtom dataAtom;
    dataAtom.data << uint32_t(AtomDataType::Implicit); // Data type
    dataAtom.data << uint32_t(0);                      // Language ?
    dataAtom.data << uint16_t(0);
    dataAtom.data << uint16_t(encoder.tags().discNum());
    dataAtom.data << uint16_t(encoder.tags().discCount());
    dataAtom.data << uint16_t(0);
    subAtoms << dataAtom;
}

MetaAtom::MetaAtom(const Encoder &encoder)
{
    Atom ilst;
    ilst.typeId = "ilst";
    for (auto tag : encoder.tags().stringTags()) {
        addTag(ilst, tag.first, tag.second);
    }

    for (auto tag : encoder.tags().boolTags()) {
        addTag(ilst, tag.first, tag.second);
    }

    if (encoder.tags().genreNum()) {
        addTag(ilst, "gnre", encoder.tags().genreNum());
    }

    if (encoder.tags().trackNum()) {
        ilst.data << TrknAtom(encoder);
    }

    if (encoder.tags().discNum()) {
        ilst.data << DiskAtom(encoder);
    }

    if (!encoder.tags().coverFile().empty()) {
        CovrAtom covr(encoder);
        ilst.data << covr;
    }

    // ................................
    typeId = "meta";

    // Version
    // A 1-byte specification of the version of this sample description atom.
    data << '\0';

    // Flags
    // A 3-byte space for sample description flags. Set this field to 0.
    data << '\0' << '\0' << '\0';

    // I do not know what the exact structure of this atom is.
    // The data saved by taglib and ffmpeg does not match the description on Apple site
    // https://developer.apple.com/library/archive/documentation/QuickTime/QTFF/Metadata/Metadata.html
    Atom hdlr;
    hdlr.typeId = "hdlr";
    hdlr.data << uint32_t(0) << uint32_t(0);
    hdlr.data << "mdir"
              << "appl";
    hdlr.data << uint32_t(0) << uint32_t(0);
    hdlr.data << '\0';

    data << hdlr;
    data << ilst;

    // subAtoms << FreeAtom(4 * 1024);
}

CovrAtom::CovrAtom(const Encoder &encoder)
{
    Atom dataAtom;
    dataAtom.typeId = "data";

    // clang-format off
    switch (encoder.tags().coverType()) {
        case FileType::JPEG: dataAtom.data << uint32_t(AtomDataType::JPEG); break;
        case FileType::PNG:  dataAtom.data << uint32_t(AtomDataType::PNG);  break;
        case FileType::BMP:  dataAtom.data << uint32_t(AtomDataType::BMP);  break;
        case FileType::GIF:  dataAtom.data << uint32_t(AtomDataType::GIF);  break;
        default:             throw Error(encoder.tags().coverFile() + ": Unsupported file type");
    }
    // clang-fornmat on
    dataAtom.data << uint32_t(0); // I don't know what is

    // Append file data ..................
    auto file = std::ifstream(encoder.tags().coverFile(), std::ios::in | std::ios::binary);
    if (file.fail()) {
        throw Error(encoder.tags().coverFile() + ": " + strerror(errno));
    }

    file.seekg( 0, std::ios::end );
    uint64_t size = file.tellg();
    file.seekg( 0, std::ios::beg );

    dataAtom.data.resize(size + 8);
    file.read(dataAtom.data.data() + 8, size);

    typeId = "covr";
    subAtoms << dataAtom;
}



