#include "moar.h"

static uint8_t BOM_UTF16LE[2] = { 0xFF, 0xFE };
static uint8_t BOM_UTF16BE[2] = { 0xFE, 0xFF };
#define UTF16_DECODE_BIG_ENDIAN 1
#define UTF16_DECODE_LITTLE_ENDIAN 2
#define UTF16_DECODE_AUTO_ENDIAN 4
MVM_STATIC_INLINE int has_little_endian_bom (uint8_t *buf8) {
    return buf8[0] == BOM_UTF16LE[0] && buf8[1] == BOM_UTF16LE[1];
}
MVM_STATIC_INLINE int has_big_endian_bom (uint8_t *buf8) {
    return buf8[0] == BOM_UTF16BE[0] && buf8[1] == BOM_UTF16BE[1];
}
MVM_STATIC_INLINE void init_utf16_decoder_state(MVMDecodeStream *ds, int setting) {
    if (!ds->decoder_state) {
        ds->decoder_state = MVM_malloc(sizeof(int32_t));
    }
    *((int32_t*)ds->decoder_state) = setting;
}
#define utf16_decoder_state(ds) (*((int32_t*)(ds)->decoder_state))
uint32_t MVM_string_utf16_decodestream_main(MVMThreadContext *tc, MVMDecodeStream *ds,
                                    const uint32_t *stopper_chars,
                                    MVMDecodeStreamSeparators *seps, int endianess);
uint32_t MVM_string_utf16_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds,
                                    const uint32_t *stopper_chars,
                                    MVMDecodeStreamSeparators *seps) {
    if (!ds->decoder_state) {
#       ifdef MVM_BIGENDIAN
            init_utf16_decoder_state(ds, UTF16_DECODE_BIG_ENDIAN);
#       else
            init_utf16_decoder_state(ds, UTF16_DECODE_LITTLE_ENDIAN);
#       endif
    }
    return MVM_string_utf16_decodestream_main(tc, ds, stopper_chars, seps, UTF16_DECODE_AUTO_ENDIAN);
}
uint32_t MVM_string_utf16le_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds,
                                    const uint32_t *stopper_chars,
                                    MVMDecodeStreamSeparators *seps) {
    init_utf16_decoder_state(ds, UTF16_DECODE_LITTLE_ENDIAN);
    return MVM_string_utf16_decodestream_main(tc, ds, stopper_chars, seps, UTF16_DECODE_LITTLE_ENDIAN);
}
uint32_t MVM_string_utf16be_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds,
                                    const uint32_t *stopper_chars,
                                    MVMDecodeStreamSeparators *seps) {
    init_utf16_decoder_state(ds, UTF16_DECODE_BIG_ENDIAN);
    return MVM_string_utf16_decodestream_main(tc, ds, stopper_chars, seps, UTF16_DECODE_BIG_ENDIAN);
}
/* mostly from YAML-LibYAML */
/* Decodes using a decodestream. Decodes as far as it can with the input
 * buffers, or until a stopper is reached. */
uint32_t MVM_string_utf16_decodestream_main(MVMThreadContext *tc, MVMDecodeStream *ds,
                                    const uint32_t *stopper_chars,
                                    MVMDecodeStreamSeparators *seps, int endianess) {
    uint32_t count = 0, total = 0;
    uint32_t bufsize;
    MVMGrapheme32 *buffer;
    MVMDecodeStreamBytes *cur_bytes;
    MVMDecodeStreamBytes *last_accept_bytes = ds->bytes_head;
    int32_t last_accept_pos;
    uint32_t reached_stopper;
    int low, high;
    /* Set to 1 to remove the BOM even when big endian or little endian are
     * explicitly specified. */
    int remove_bom = 0;

    /* If there's no buffers, we're done. */
    if (!ds->bytes_head)
        return 0;
    last_accept_pos = ds->bytes_head_pos;

    /* If we're asked for zero chars, also done. */
    if (stopper_chars && *stopper_chars == 0)
        return 1;

    bufsize = ds->result_size_guess;
    buffer = MVM_malloc(bufsize * sizeof(MVMGrapheme32));

    /* Decode each of the buffers. */
    cur_bytes = ds->bytes_head;
    reached_stopper = 0;
    if (utf16_decoder_state(ds) == UTF16_DECODE_LITTLE_ENDIAN) {
        low = 0;
        high = 1;
    }
    else if (utf16_decoder_state(ds) == UTF16_DECODE_BIG_ENDIAN) {
        low = 1;
        high = 0;
    }
    else {
        MVM_free(buffer);
        MVM_exception_throw_adhoc(tc, "Unknown config setting in utf16 decodestream. This should never happen.");
    }
    while (cur_bytes) {
        /* Process this buffer. */
        int32_t  pos = cur_bytes == ds->bytes_head ? ds->bytes_head_pos : 0;
        uint8_t *bytes = cur_bytes->bytes;
        if (ds->abs_byte_pos == 0 && pos + 1 < cur_bytes->length) {
            if (has_little_endian_bom(bytes + pos)) {
                /* Only change the setting if we are using standard 'utf16' decode
                 * which is meant to detect the encoding. */
                if (endianess == UTF16_DECODE_AUTO_ENDIAN) {
                    low = 0;
                    high = 1;
                    last_accept_pos = pos += 2;
                    utf16_decoder_state(ds) = UTF16_DECODE_LITTLE_ENDIAN;
                }
                /* If we see little endian BOM and we're set to utf16le, skip
                 * the BOM. */
                else if (endianess == UTF16_DECODE_LITTLE_ENDIAN && remove_bom) {
                    last_accept_pos = pos += 2;
                }
            }
            else if (has_big_endian_bom(bytes + pos)) {
                if (endianess == UTF16_DECODE_AUTO_ENDIAN) {
                    low = 1;
                    high = 0;
                    last_accept_pos = pos += 2;
                    utf16_decoder_state(ds) = UTF16_DECODE_BIG_ENDIAN;
                }
                /* If we see a big endian BOM and we're set to utf16be, skip
                 * the BOM. */
                else if (endianess == UTF16_DECODE_BIG_ENDIAN && remove_bom) {
                    last_accept_pos = pos += 2;
                }

            }
        }
        while (pos + 1 < cur_bytes->length) {
            uint32_t value = (bytes[pos+high] << 8) + bytes[pos+low];
            uint32_t value2;

            if ((value & 0xFC00) == 0xDC00) {
                MVM_free(buffer);
                MVM_exception_throw_adhoc(tc, "Malformed UTF-16; unexpected low surrogate");
            }

            if ((value & 0xFC00) == 0xD800) { /* high surrogate */
                pos += 2;
                if (pos + 1 >= cur_bytes->length) {
                    MVM_free(buffer);
                    MVM_exception_throw_adhoc(tc, "Malformed UTF-16; incomplete surrogate pair");
                }
                value2 = (bytes[pos+high] << 8) + bytes[pos+low];
                if ((value2 & 0xFC00) != 0xDC00) {
                    MVM_free(buffer);
                    MVM_exception_throw_adhoc(tc, "Malformed UTF-16; incomplete surrogate pair");
                }
                value = 0x10000 + ((value & 0x3FF) << 10) + (value2 & 0x3FF);
            }
            if (count == bufsize) {
                /* We filled the buffer. Attach this one to the buffers
                 * linked list, and continue with a new one. */
                MVM_string_decodestream_add_chars(tc, ds, buffer, bufsize);
                buffer = MVM_malloc(bufsize * sizeof(MVMGrapheme32));
                count = 0;
            }
            buffer[count++] = value;
            last_accept_bytes = cur_bytes;
            last_accept_pos = pos += 2;
            total++;
            if (MVM_string_decode_stream_maybe_sep(tc, seps, value) ||
                    (stopper_chars && *stopper_chars == total)) {
                reached_stopper = 1;
                goto done;
            }
        }
        cur_bytes = cur_bytes->next;
    }
  done:

    /* Attach what we successfully parsed as a result buffer, and trim away
     * what we chewed through. */
    if (count) {
        MVM_string_decodestream_add_chars(tc, ds, buffer, count);
    }
    else {
        MVM_free(buffer);
    }
    MVM_string_decodestream_discard_to(tc, ds, last_accept_bytes, last_accept_pos);

    return reached_stopper;
}
static MVMString * MVM_string_utf16_decode_main(MVMThreadContext *tc,
        const MVMObject *result_type, uint8_t *utf16_chars, size_t bytes, int endianess);
MVMString * MVM_string_utf16be_decode(MVMThreadContext *tc,
        const MVMObject *result_type, char *utf16_chars, size_t bytes) {
    return MVM_string_utf16_decode_main(tc, result_type, (uint8_t*)utf16_chars, bytes, UTF16_DECODE_BIG_ENDIAN);
}
MVMString * MVM_string_utf16le_decode(MVMThreadContext *tc,
            const MVMObject *result_type, char *utf16_chars, size_t bytes) {
    return MVM_string_utf16_decode_main(tc, result_type, (uint8_t*)utf16_chars, bytes, UTF16_DECODE_LITTLE_ENDIAN);
}
MVMString * MVM_string_utf16_decode(MVMThreadContext *tc,
            const MVMObject *result_type, char *utf16_chars_in, size_t bytes) {
    uint8_t *utf16_chars = (uint8_t*)utf16_chars_in;
#ifdef MVM_BIGENDIAN
    int mode = UTF16_DECODE_BIG_ENDIAN;
#else
    int mode = UTF16_DECODE_LITTLE_ENDIAN;
#endif
    /* set the byte order if there's a BOM */
    if (2 <= bytes) {
        if (has_little_endian_bom(utf16_chars)) {
            mode = UTF16_DECODE_LITTLE_ENDIAN;
            utf16_chars += 2;
            bytes -= 2;
        }
        else if (has_big_endian_bom(utf16_chars)) {
            mode = UTF16_DECODE_BIG_ENDIAN;
            utf16_chars += 2;
            bytes -= 2;
        }
    }
    return MVM_string_utf16_decode_main(tc, result_type, (uint8_t*)utf16_chars, bytes, mode);
}
/* Decodes the specified number of bytes of utf16 into an NFG string, creating
 * a result of the specified type. The type must have the MVMString REPR. */
static MVMString * MVM_string_utf16_decode_main(MVMThreadContext *tc,
        const MVMObject *result_type, uint8_t *utf16_chars, size_t bytes, int endianess) {
    MVMString *result;
    MVMGrapheme32 *buffer;
    size_t str_pos = 0;
    uint8_t *utf16 = (uint8_t *)utf16_chars;
    uint8_t *utf16_end = NULL;
    int low, high;
    MVMNormalizer norm;
    int32_t ready;
    switch (endianess) {
        case UTF16_DECODE_BIG_ENDIAN:
            low  = 1;
            high = 0;
            break;
        case UTF16_DECODE_LITTLE_ENDIAN:
            low  = 0;
            high = 1;
            break;
        default:
            MVM_exception_throw_adhoc(tc, "Unknown mode set in utf16 decode. This should never happen.");
    }

    if (bytes % 2) {
        MVM_exception_throw_adhoc(tc, "Malformed UTF-16; odd number of bytes (%"PRIu64")", (uint64_t)bytes);
    }

    utf16_end = utf16 + bytes;

    /* possibly allocating extra space; oh well */
    buffer = MVM_malloc(sizeof(MVMGrapheme32) * bytes / 2);

    /* Need to normalize to NFG as we decode. */
    MVM_unicode_normalizer_init(tc, &norm, MVM_NORMALIZE_NFG);

    for (; utf16 < utf16_end; utf16 += 2) {
        uint32_t value = (utf16[high] << 8) + utf16[low];
        uint32_t value2;
        MVMGrapheme32 g;

        if ((value & 0xFC00) == 0xDC00) {
            MVM_free(buffer);
            MVM_unicode_normalizer_cleanup(tc, &norm);
            MVM_exception_throw_adhoc(tc, "Malformed UTF-16; unexpected low surrogate");
        }

        if ((value & 0xFC00) == 0xD800) { /* high surrogate */
            utf16 += 2;
            if (utf16 == utf16_end) {
                MVM_free(buffer);
                MVM_unicode_normalizer_cleanup(tc, &norm);
                MVM_exception_throw_adhoc(tc, "Malformed UTF-16; incomplete surrogate pair");
            }
            value2 = (utf16[high] << 8) + utf16[low];
            if ((value2 & 0xFC00) != 0xDC00) {
                MVM_free(buffer);
                MVM_unicode_normalizer_cleanup(tc, &norm);
                MVM_exception_throw_adhoc(tc, "Malformed UTF-16; incomplete surrogate pair");
            }
            value = 0x10000 + ((value & 0x3FF) << 10) + (value2 & 0x3FF);
        }

        /* TODO: check for invalid values */
        ready = MVM_unicode_normalizer_process_codepoint_to_grapheme(tc, &norm, value, &g);
        if (ready) {
            buffer[str_pos++] = g;
            while (--ready > 0)
                buffer[str_pos++] = MVM_unicode_normalizer_get_grapheme(tc, &norm);
        }
    }

    /* Get any final graphemes from the normalizer, and clean it up. */
    MVM_unicode_normalizer_eof(tc, &norm);
    ready = MVM_unicode_normalizer_available(tc, &norm);
    while (ready--)
        buffer[str_pos++] = MVM_unicode_normalizer_get_grapheme(tc, &norm);
    MVM_unicode_normalizer_cleanup(tc, &norm);

    result = (MVMString *)REPR(result_type)->allocate(tc, STABLE(result_type));
    result->body.storage.blob_32 = buffer;
    result->body.storage_type = MVM_STRING_GRAPHEME_32;
    result->body.num_graphs   = str_pos;

    return result;
}
MVM_STATIC_INLINE uint16_t swap_bytes(uint16_t uint16, int enable_byte_swap) {
    return enable_byte_swap
        ? (uint16 << 8) | (uint16 >> 8)
        : uint16;
}
char * MVM_string_utf16_encode_substr_main(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines, int endianess);
/* Encodes the specified substring to utf16. The result string is NULL terminated, but
 * the specified size is the non-null part. (This being UTF-16, there are 2 null bytes
 * on the end.) */
char * MVM_string_utf16be_encode_substr(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines) {
    return MVM_string_utf16_encode_substr_main(tc, str, output_size, start, length, replacement, translate_newlines, UTF16_DECODE_BIG_ENDIAN);
}
char * MVM_string_utf16le_encode_substr(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines) {
    return MVM_string_utf16_encode_substr_main(tc, str, output_size, start, length, replacement, translate_newlines, UTF16_DECODE_LITTLE_ENDIAN);
}
char * MVM_string_utf16_encode_substr(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines) {
    return MVM_string_utf16_encode_substr_main(tc, str, output_size, start, length, replacement, translate_newlines, UTF16_DECODE_AUTO_ENDIAN);
}

char * MVM_string_utf16_encode_substr_main(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines, int endianess) {
    MVMStringIndex strgraphs = MVM_string_graphs(tc, str);
    uint32_t lengthu = (uint32_t)(length == -1 ? strgraphs - start : length);
    uint16_t *result;
    uint16_t *result_pos;
    MVMCodepointIter ci;
    uint8_t *repl_bytes = NULL;
    uint64_t repl_length = 0;
    int32_t alloc_size;
    uint64_t scratch_space = 0;
    int enable_byte_swap = 0;
#ifdef MVM_BIGENDIAN
    if (endianess == UTF16_DECODE_LITTLE_ENDIAN)
        enable_byte_swap = 1;
#else
    if (endianess == UTF16_DECODE_BIG_ENDIAN)
        enable_byte_swap = 1;
#endif
    /* must check start first since it's used in the length check */
    if (start < 0 || start > strgraphs)
        MVM_exception_throw_adhoc(tc, "start (%"PRId64") out of range (0..%"PRIu32")", start, strgraphs);
    if (start + lengthu > strgraphs)
        MVM_exception_throw_adhoc(tc, "length (%"PRId64") out of range (0..%"PRIu32")", length, strgraphs);

    if (replacement)
        repl_bytes = (uint8_t *) MVM_string_utf16_encode_substr(tc,
            replacement, &repl_length, 0, -1, NULL, translate_newlines);

    alloc_size = lengthu * 2;
    result = MVM_malloc(alloc_size + 2);
    result_pos = result;
    MVM_string_ci_init(tc, &ci, str, translate_newlines, 0);
    while (MVM_string_ci_has_more(tc, &ci)) {
        int bytes_needed;
        MVMCodepoint value = MVM_string_ci_get_codepoint(tc, &ci);

        if (value < 0x10000) {
            bytes_needed = 2;
        }
        else if (value <= 0x1FFFFF) {
            bytes_needed = 4;
        }
        else {
            bytes_needed = repl_length;
        }

        while ((alloc_size - 2 * (result_pos - result)) < bytes_needed) {
            uint16_t *new_result;

            alloc_size *= 2;
            new_result  = MVM_realloc(result, alloc_size + 2);

            result_pos = new_result + (result_pos - result);
            result     = new_result;
        }

        if (value < 0x10000) {
            result_pos[0] = swap_bytes(value, enable_byte_swap);
            result_pos++;
        }
        else if (value <= 0x1FFFFF) {
            value -= 0x10000;
            result_pos[0] = swap_bytes(0xD800 + (value >> 10), enable_byte_swap);
            result_pos[1] = swap_bytes(0xDC00 + (value & 0x3FF), enable_byte_swap);
            result_pos += 2;
        }
        else if (replacement) {
            memcpy(result_pos, repl_bytes, repl_length);
            result_pos += repl_length/2;
        }
        else {
            MVM_free(result);
            MVM_free(repl_bytes);
            MVM_exception_throw_adhoc(tc,
                "Error encoding UTF-16 string: could not encode codepoint %d",
                value);
        }
    }
    result_pos[0] = 0;
    if (!output_size)
        output_size = &scratch_space;
    *output_size = (char *)result_pos - (char *)result;
    result = MVM_realloc(result, *output_size + 2);
    MVM_free(repl_bytes);
    return (char *)result;
}

/* Encodes the whole string, double-NULL terminated. */
char * MVM_string_utf16_encode(MVMThreadContext *tc, MVMString *str, int32_t translate_newlines) {
    return MVM_string_utf16_encode_substr(tc, str, NULL, 0, -1, NULL, translate_newlines);
}
