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

// clang-format on

/*
moov    		movie atom                              SIZE: 268786  POS:     32
+  • mvhd		movie header atom                       SIZE:    108  POS:     40      DATA 100 bytes
+  ⊿ trak		track atoms                             SIZE:   9476  POS:    148
+    • tkhd      	track header atom                       SIZE:     92  POS:    156      DATA 84 bytes
+    ⊿ mdia      	media atom                              SIZE:   9376  POS:    248
+      • mdhd    	media header atom                       SIZE:     32  POS:    256      DATA 24 bytes
+      • hdlr    	handler reference                       SIZE:     36  POS:    288      DATA 28 bytes
+      ⊿ minf    	media information                       SIZE:   9300  POS:    324
+        • smhd  	Sound Media Information Header          SIZE:     16  POS:    332      DATA 8 bytes
+        ⊿ dinf  	Data Information Atoms                  SIZE:     36  POS:    348
+          • dref	Data Reference Atoms                    SIZE:     28  POS:    356      DATA 20 bytes
         ⊿ stbl  	Sample Table Atoms                      SIZE:   9240  POS:    384
+          • stsd	Sample Description Atoms                SIZE:     88  POS:    392      DATA 80 bytes
           • stts	Time-to-Sample Atoms                    SIZE:     32  POS:    480      DATA 24 bytes
           • stsc	Sample-to-Chunk Atoms                   SIZE:     40  POS:    512      DATA 32 bytes
           • stsz	Sample Size Atoms                       SIZE:   7548  POS:    552      DATA 7540 bytes
           • stco	Chunk Offset Atoms                      SIZE:   1524  POS:   8100      DATA 1516 bytes
   ⊿ udta		user data atom                          SIZE: 259194  POS:   9624
     ⊿ meta      	Timed Metadata Media                    SIZE: 259186  POS:   9632
*/
#endif // ATOMS_H
