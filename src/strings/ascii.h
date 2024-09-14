 MVMString * MVM_string_ascii_decode(struct MVMThreadContext *tc, const MVMObject *result_type, const char *ascii, size_t bytes);
 MVMString * MVM_string_ascii_decode_nt(struct MVMThreadContext *tc, const MVMObject *result_type, const char *ascii);
 uint32_t MVM_string_ascii_decodestream(struct MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
 char * MVM_string_ascii_encode_substr(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);
 char * MVM_string_ascii_encode(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int32_t translate_newlines);
char * MVM_string_ascii_encode_any(struct MVMThreadContext *tc, MVMString *str);
char * MVM_string_ascii_encode_malloc(struct MVMThreadContext *tc, MVMString *str);
