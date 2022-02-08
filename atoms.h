#ifndef ATOMS_H
#define ATOMS_H

#include <vector>
#include <array>
#include <ostream>
#include "types.h"
#include "wavheader.h"

class Encoder;

struct Atom
{
    class TypeId : public std::array<char, 4>
    {
    public:
        TypeId(const char type[5]);
        bool isEmpty() const;
    };

    TypeId            typeId = "    ";
    std::vector<Atom> subAtoms;
    Bytes             data;

    size_t size() const;
};

OutFile &operator<<(OutFile &os, const Atom &atom);

struct FtypAtom : public Atom
{
    FtypAtom();
};

struct FreeAtom : public Atom
{
    FreeAtom(uint32_t size) :
        mSize(size) {};

    uint32_t mSize;
};

OutFile &operator<<(OutFile &os, const FreeAtom &atom);

// clang-format off

struct MoovAtom : public Atom { MoovAtom(const Encoder &encoder); };
struct MvhdAtom : public Atom { MvhdAtom(const Encoder &encoder); };
struct TrakAtom : public Atom { TrakAtom(const Encoder &encoder); };
struct TkhdAtom : public Atom { TkhdAtom(const Encoder &encoder); };
struct MdiaAtom : public Atom { MdiaAtom(const Encoder &encoder); };
struct MdhdAtom : public Atom { MdhdAtom(const Encoder &encoder); };
struct HdlrAtom : public Atom { HdlrAtom(const Encoder &encoder); };
struct MinfAtom : public Atom { MinfAtom(const Encoder &encoder); };
struct SmhdAtom : public Atom { SmhdAtom(const Encoder &encoder); };
struct DinfAtom : public Atom { DinfAtom(const Encoder &encoder); };
struct DrefAtom : public Atom { DrefAtom(const Encoder &encoder); };
struct StblAtom : public Atom { StblAtom(const Encoder &encoder); };
struct StsdAtom : public Atom { StsdAtom(const Encoder &encoder); };
struct StscAtom : public Atom { StscAtom(const Encoder &encoder); };
struct StcoAtom : public Atom { StcoAtom(const Encoder &encoder); };
struct StszAtom : public Atom { StszAtom(const Encoder &encoder); };
struct SttsAtom : public Atom { SttsAtom(const Encoder &encoder); };
struct UdtaAtom : public Atom { UdtaAtom(const Encoder &encoder); };
struct MetaAtom : public Atom { MetaAtom(const Encoder &encoder); };
struct CovrAtom : public Atom { CovrAtom(const Encoder &encoder); };
struct TrknAtom : public Atom { TrknAtom(const Encoder &encoder); };
struct DiskAtom : public Atom { DiskAtom(const Encoder &encoder); };

// clang-format on

/*
• ftyp  		file type atom                          SIZE:     28  POS:      8
• free  		free space                              SIZE:      8  POS:     36
• mdat  		DATA .............                      SIZE: 25433528  POS:     44
⊿ moov  		movie atom                              SIZE:  29181  POS: 25433572
  • mvhd		movie header atom                       SIZE:    108  POS: 25433580
  ⊿ trak		track atoms                             SIZE:  10249  POS: 25433688
    • tkhd      	track header atom                       SIZE:     92  POS: 25433696
    ⊿ edts      	Edit Atom                               SIZE:     36  POS: 25433788
      • elst    	Edit List Atom                          SIZE:     28  POS: 25433796
    ⊿ mdia      	media atom                              SIZE:  10113  POS: 25433824
      • mdhd    	media header atom                       SIZE:     32  POS: 25433832
      • hdlr    	handler reference                       SIZE:     45  POS: 25433864
      ⊿ minf    	media information                       SIZE:  10028  POS: 25433909
        • smhd  	Sound Media Information Header          SIZE:     16  POS: 25433917
        ⊿ dinf  	Data Information Atoms                  SIZE:     36  POS: 25433933
          • dref	Data Reference Atoms                    SIZE:     28  POS: 25433941
        ⊿ stbl  	Sample Table Atoms                      SIZE:   9968  POS: 25433969
          • stsd	Sample Description Atoms                SIZE:     88  POS: 25433977
          • stts	Time-to-Sample Atoms                    SIZE:     32  POS: 25434065
          • stsc	Sample-to-Chunk Atoms                   SIZE:    268  POS: 25434097
          • stsz	Sample Size Atoms                       SIZE:   9456  POS: 25434365
          • stco	Chunk Offset Atoms                      SIZE:    116  POS: 25443821
  ⊿ udta		user data atom                          SIZE:  18816  POS: 25443937
    ⊿ meta      	Metadata                                SIZE:  18808  POS: 25443945
      • hdlr    	handler reference                       SIZE:     33  POS: 25443957
      ⊿ ilst    	Ilist                                   SIZE:  16953  POS: 25443990
        • ©too  	                                        SIZE:     37  POS: 25443998
        • covr  	                                        SIZE:  16670  POS: 25444035
        • ©nam  	                                        SIZE:     40  POS: 25460705
        • ©ART  	                                        SIZE:     36  POS: 25460745
        • aART  	                                        SIZE:     46  POS: 25460781
        • ©wrt  	                                        SIZE:     44  POS: 25460827
        • ©alb  	                                        SIZE:     36  POS: 25460871
        • ©grp  	                                        SIZE:     36  POS: 25460907
      • free    	free space                              SIZE:   1810  POS: 25460943
*/
#endif // ATOMS_H
