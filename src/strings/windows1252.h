/* Bitmap controling whether we throw on codepoints which don't have mappings (yet still
 * fit in one byte). If use loose we we pass through the codepoint unchanged if it fits
 * in one byte. */
#define MVM_ENCODING_PERMISSIVE 1
#define MVM_ENCODING_CONFIG_STRICT(config) (!(config & MVM_ENCODING_PERMISSIVE))
#define MVM_ENCODING_CONFIG_PERMISSIVE(config) (config & MVM_ENCODING_PERMISSIVE)
MVMString * MVM_string_windows1252_decode(struct MVMThreadContext *tc, const MVMObject *result_type, char *windows1252, size_t bytes);
 uint32_t MVM_string_windows1252_decodestream(struct MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
char * MVM_string_windows1252_encode_substr(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);
char * MVM_string_windows1252_encode_substr_config(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines, int64_t bitmap);
char * MVM_string_windows1251_encode_substr_config(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines, int64_t bitmap);
MVMString * MVM_string_windows1251_decode(struct MVMThreadContext *tc, const MVMObject *result_type, char *windows1252, size_t bytes);
 uint32_t MVM_string_windows1251_decodestream(struct MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
char * MVM_string_windows1251_encode_substr(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);
MVMString * MVM_string_windows1252_decode_config(struct MVMThreadContext *tc,
        const MVMObject *result_type, char *windows125X_c, size_t bytes,
        MVMString *replacement, int64_t bitmap);
MVMString * MVM_string_windows1251_decode_config(struct MVMThreadContext *tc,
        const MVMObject *result_type, char *windows125X_c, size_t bytes,
        MVMString *replacement, int64_t bitmap);
