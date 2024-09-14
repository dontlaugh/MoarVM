/* Representation used by VM-level strings.
 *
 * Strings come in one of 3 forms today, with 1 expected future form:
 *   - 32-bit buffer of graphemes (Unicode codepoints or synthetic codepoints)
 *   - 8-bit buffer of codepoints that all fall in the ASCII range
 *   - Buffer of strands
 *   - (LATER) 8-bit buffer of codepoints with negatives as synthetics (we
 *     draw out a distinction with the ASCII range buffer because we can do
 *     some I/O simplifications when we know all is in the ASCII range).
 *
 * A buffer of strands represents a string made up of other non-strand
 * strings. That is, there's no recursive strands. This simplifies the
 * process of iteration enormously. A strand may refer to just part of
 * another string by specifying offsets. Furthermore, it may specify a
 * repetition count.
 */

/* Kinds of grapheme we may hold in a string. */
typedef int32_t MVMGrapheme32;
typedef int8_t  MVMGraphemeASCII;
typedef int8_t  MVMGrapheme8;       /* Future use */

/* What kind of data is a string storing? */
#define MVM_STRING_GRAPHEME_32      0
#define MVM_STRING_GRAPHEME_ASCII   1
#define MVM_STRING_GRAPHEME_8       2
#define MVM_STRING_STRAND           3
#define MVM_STRING_IN_SITU_8        4
#define MVM_STRING_IN_SITU_32       5

/* String index data type, for when we talk about indexes. */
typedef uint32_t MVMStringIndex;

/* Data type for a Unicode codepoint. */
typedef int32_t MVMCodepoint;

/* Maximum number of strands we will have. */
#define MVM_STRING_MAX_STRANDS  64

/* The body of a string. */
struct MVMStringBody {
    union {
        MVMGrapheme32    *blob_32;
        MVMGraphemeASCII *blob_ascii;
        MVMGrapheme8     *blob_8;
        MVMStringStrand  *strands;
        MVMGrapheme8     in_situ_8[8];
        MVMGrapheme32    in_situ_32[2];
        uint64_t        any;
        void             *any_ptr;
    } storage;
    uint16_t storage_type;
    uint16_t num_strands;
    uint32_t num_graphs;
    MVMHashv  cached_hash_code;
};

/* A strand of a string. */
struct MVMStringStrand {
    /* Another string that must be some kind of grapheme string. */
    MVMString *blob_string;

    /* Start and end indexes we refer to in the blob string. */
    MVMStringIndex start;
    MVMStringIndex end;

    /* Number of repetitions. */
    uint32_t repetitions;
};

/* The MVMString, with header and body. */
struct MVMString {
    MVMObject common;
    MVMStringBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMString_initialize(struct MVMThreadContext *tc);
