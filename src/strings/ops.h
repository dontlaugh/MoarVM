/* Encoding types and encoding validity check. */
#define MVM_encoding_type_MIN           1
#define MVM_encoding_type_utf8          1
#define MVM_encoding_type_ascii         2
#define MVM_encoding_type_latin1        3
#define MVM_encoding_type_utf16         4
#define MVM_encoding_type_windows1252   5
#define MVM_encoding_type_utf8_c8       6
#define MVM_encoding_type_windows1251   7
#define MVM_encoding_type_shiftjis      8
#define MVM_encoding_type_utf16le       9
#define MVM_encoding_type_utf16be      10
#define MVM_encoding_type_gb2312       11
#define MVM_encoding_type_gb18030      12
#define MVM_encoding_type_MAX          12
#define ENCODING_VALID(enc) \
    (((enc) >= MVM_encoding_type_MIN && (enc) <= MVM_encoding_type_MAX) \
    || (MVM_exception_throw_adhoc(tc, "invalid encoding type flag: %d", (enc)),1))

/* Character class constants (map to nqp::const::CCLASS_* values). */
#define MVM_CCLASS_ANY          65535
#define MVM_CCLASS_UPPERCASE    1
#define MVM_CCLASS_LOWERCASE    2
#define MVM_CCLASS_ALPHABETIC   4
#define MVM_CCLASS_NUMERIC      8
#define MVM_CCLASS_HEXADECIMAL  16
#define MVM_CCLASS_WHITESPACE   32
#define MVM_CCLASS_PRINTING     64
#define MVM_CCLASS_BLANK        256
#define MVM_CCLASS_CONTROL      512
#define MVM_CCLASS_PUNCTUATION  1024
#define MVM_CCLASS_ALPHANUMERIC 2048
#define MVM_CCLASS_NEWLINE      4096
#define MVM_CCLASS_WORD         8192

/* Checks a string is not null or non-concrete and throws if so. */
static inline void MVM_string_check_arg(struct MVMThreadContext *tc, const MVMString *s, const char *operation) {
    if (!s || !IS_CONCRETE(s))
        MVM_exception_throw_adhoc(tc, "%s requires a concrete string, but got %s",
            operation, s ? "a type object" : "null");
}

static inline uint32_t MVM_string_graphs(struct MVMThreadContext *tc, MVMString *s) {
    MVM_string_check_arg(tc, s, "chars");
    return s->body.num_graphs;
}
static inline uint32_t MVM_string_graphs_nocheck(struct MVMThreadContext *tc, MVMString *s) {
    return s->body.num_graphs;
}
static inline uint32_t MVM_string_codes(struct MVMThreadContext *tc, MVMString *s) {
    MVMGraphemeIter gi;
    int64_t codes = 0;
    MVM_string_check_arg(tc, s, "codes");
    if (MVM_string_graphs_nocheck(tc, s) == 0)
        return 0;
    MVM_string_gi_init(tc, &gi, s);

    while(MVM_string_gi_has_more(tc, &gi)) {
        MVMGrapheme32 g = MVM_string_gi_get_grapheme(tc, &gi);
        codes += g < 0 ?
            MVM_nfg_get_synthetic_info(tc, g)->num_codes
            : 1;
    }
    return codes;
}
static inline int MVM_string_buf32_can_fit_into_8bit(MVMGrapheme32 *active_blob, MVMStringIndex blob_len) {
    MVMStringIndex i;
    MVMGrapheme32 val = 0;
    MVM_VECTORIZE_LOOP
    for (i = 0; i  < blob_len; i++) {
        /* This could be written val |= ..., but GCC 7 doesn't recognize the
         * operation as ossociative unless we use a temp variable (clang has no issue). */
        MVMGrapheme32 val2 = ((active_blob[i] & 0xffffff80) + 0x80) & (0xffffff80-1);
        val |= val2;
    }
    return val ? 0 : 1;
}

uint64_t MVM_string_compute_hash_code(struct MVMThreadContext *tc, MVMString *s);
static inline uint64_t MVM_string_hash_code(struct MVMThreadContext *tc, MVMString *s) {
    return s->body.cached_hash_code ? s->body.cached_hash_code
        : MVM_string_compute_hash_code(tc, s);
}

MVMGrapheme32 MVM_string_get_grapheme_at_nocheck(struct MVMThreadContext *tc, MVMString *a, int64_t index);
int64_t MVM_string_equal(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
int64_t MVM_string_substrings_equal_nocheck(struct MVMThreadContext *tc, MVMString *a,
        int64_t starta, int64_t length, MVMString *b, int64_t startb);
int64_t MVM_string_index(struct MVMThreadContext *tc, MVMString *haystack, MVMString *needle, int64_t start);
int64_t MVM_string_index_ignore_case(struct MVMThreadContext *tc, MVMString *haystack, MVMString *needle, int64_t start);
int64_t MVM_string_index_ignore_mark(struct MVMThreadContext *tc, MVMString *Haystack, MVMString *needle, int64_t start);
int64_t MVM_string_index_ignore_case_ignore_mark(struct MVMThreadContext *tc, MVMString *haystack, MVMString *needle, int64_t start);
int64_t MVM_string_index_from_end(struct MVMThreadContext *tc, MVMString *haystack, MVMString *needle, int64_t start);
MVMString * MVM_string_concatenate(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
MVMString * MVM_string_repeat(struct MVMThreadContext *tc, MVMString *a, int64_t count);
MVMString * MVM_string_substring(struct MVMThreadContext *tc, MVMString *a, int64_t start, int64_t length);
MVMString * MVM_string_replace(struct MVMThreadContext *tc, MVMString *a, int64_t start, int64_t length, MVMString *replacement);
void MVM_string_say(struct MVMThreadContext *tc, MVMString *a);
void MVM_string_print(struct MVMThreadContext *tc, MVMString *a);
int64_t MVM_string_equal_at(struct MVMThreadContext *tc, MVMString *a, MVMString *b, int64_t offset);
int64_t MVM_string_equal_at_ignore_case(struct MVMThreadContext *tc, MVMString *a, MVMString *b, int64_t offset);
int64_t MVM_string_equal_at_ignore_mark(struct MVMThreadContext *tc, MVMString *Haystack, MVMString *needle, int64_t H_offset);
int64_t MVM_string_equal_at_ignore_case_ignore_mark(struct MVMThreadContext *tc, MVMString *a, MVMString *b, int64_t offset);
MVMGrapheme32 MVM_string_ord_basechar_at(struct MVMThreadContext *tc, MVMString *s, int64_t offset);
MVMGrapheme32 MVM_string_ord_at(struct MVMThreadContext *tc, MVMString *s, int64_t offset);
int64_t MVM_string_have_at(struct MVMThreadContext *tc, MVMString *a, int64_t starta, int64_t length, MVMString *b, int64_t startb);
int64_t MVM_string_get_grapheme_at(struct MVMThreadContext *tc, MVMString *a, int64_t index);
int64_t MVM_string_index_of_grapheme(struct MVMThreadContext *tc, MVMString *a, MVMGrapheme32 codepoint);
MVMString * MVM_string_uc(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_lc(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_tc(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_fc(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_decode(struct MVMThreadContext *tc, const MVMObject *type_object, char *Cbuf, int64_t byte_length, int64_t encoding_flag);
char * MVM_string_encode(struct MVMThreadContext *tc, MVMString *s, int64_t start, int64_t length, uint64_t *output_size, int64_t encoding_flag, MVMString *replacement, int32_t translate_newlines);
MVMObject * MVM_string_encode_to_buf(struct MVMThreadContext *tc, MVMString *s, MVMString *enc_name, MVMObject *buf, MVMString *replacement);
MVMObject * MVM_string_encode_to_buf_config(struct MVMThreadContext *tc, MVMString *s, MVMString *enc_name, MVMObject *buf, MVMString *replacement, int64_t bitmap);
MVMString * MVM_string_decode_from_buf(struct MVMThreadContext *tc, MVMObject *buf, MVMString *enc_name);
MVMString * MVM_string_decode_from_buf_config(struct MVMThreadContext *tc, MVMObject *buf,
        MVMString *enc_name, MVMString *replacement, int64_t bitmap);
MVMObject * MVM_string_split(struct MVMThreadContext *tc, MVMString *separator, MVMString *input);
MVMString * MVM_string_join(struct MVMThreadContext *tc, MVMString *separator, MVMObject *input);
int64_t MVM_string_char_at_in_string(struct MVMThreadContext *tc, MVMString *a, int64_t offset, MVMString *b);
int64_t MVM_string_offset_has_unicode_property_value(struct MVMThreadContext *tc, MVMString *s, int64_t offset, int64_t property_code, int64_t property_value_code);
int64_t MVM_unicode_codepoint_has_property_value(struct MVMThreadContext *tc, int64_t grapheme, int64_t property_code, int64_t property_value_code);
MVMString * MVM_unicode_codepoint_get_property_str(struct MVMThreadContext *tc, int64_t grapheme, int64_t property_code);
const char * MVM_unicode_codepoint_get_property_cstr(struct MVMThreadContext *tc, int64_t grapheme, int64_t property_code);
int64_t MVM_unicode_codepoint_get_property_int(struct MVMThreadContext *tc, int64_t grapheme, int64_t property_code);
int64_t MVM_unicode_codepoint_get_property_bool(struct MVMThreadContext *tc, int64_t grapheme, int64_t property_code);
MVMString * MVM_unicode_get_name(struct MVMThreadContext *tc, int64_t grapheme);
MVMString * MVM_string_indexing_optimized(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_escape(struct MVMThreadContext *tc, MVMString *s);
MVMString * MVM_string_flip(struct MVMThreadContext *tc, MVMString *s);
int64_t MVM_string_compare(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
MVMString * MVM_string_bitand(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
MVMString * MVM_string_bitor(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
MVMString * MVM_string_bitxor(struct MVMThreadContext *tc, MVMString *a, MVMString *b);
int64_t MVM_string_is_cclass(struct MVMThreadContext *tc, int64_t cclass, MVMString *s, int64_t offset);
int64_t MVM_string_find_cclass(struct MVMThreadContext *tc, int64_t cclass, MVMString *s, int64_t offset, int64_t count);
int64_t MVM_string_find_not_cclass(struct MVMThreadContext *tc, int64_t cclass, MVMString *s, int64_t offset, int64_t count);
uint8_t MVM_string_find_encoding(struct MVMThreadContext *tc, MVMString *name);
MVMString * MVM_string_chr(struct MVMThreadContext *tc, int64_t cp);
int64_t MVM_string_grapheme_is_cclass(struct MVMThreadContext *tc, int64_t cclass, MVMGrapheme32 g);
MVMString * MVM_string_ascii_from_buf_nocheck(struct MVMThreadContext *tc, MVMGrapheme8 *buf, MVMStringIndex len);
char * MVM_string_encoding_cname(struct MVMThreadContext *tc, int64_t encoding);
/* If MVM_DEBUG_NFG is 1, calls to NFG_CHECK will re_nfg the given string
 * and compare num_graphs before and after the normalization.
 * If it is different debug information will be printed out.*/
#define MVM_DEBUG_NFG 0
/* MVM_DEBUG_NFG_STRICT does as above but does not only rely on num_graphs. It
 * always checks every grapheme manually. Slower. (requires MVM_DEBUG_NFG)*/
#define MVM_DEBUG_NFG_STRICT 0
#if MVM_DEBUG_NFG
void NFG_check (struct MVMThreadContext *tc, MVMString *orig, char *varname);
void NFG_check_concat (struct MVMThreadContext *tc, MVMString *result, MVMString *a, MVMString *b, char *varname);
#define NFG_CHECK(tc, s, varname)              NFG_check(tc, s, varname);
#define NFG_CHECK_CONCAT(tc, s, a, b, varname) NFG_check_concat(tc, s, a, b, varname);
#else
#define NFG_CHECK(tc, s, varname)
#define NFG_CHECK_CONCAT(tc, s, a, b, varname)
#endif
