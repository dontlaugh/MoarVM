MVM_PUBLIC MVMString * MVM_string_ascii_decode(MVMThreadContext *tc, const MVMObject *result_type, const char *ascii, size_t bytes);
MVM_PUBLIC MVMString * MVM_string_ascii_decode_nt(MVMThreadContext *tc, const MVMObject *result_type, const char *ascii);
MVM_PUBLIC uint32_t MVM_string_ascii_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
MVM_PUBLIC char * MVM_string_ascii_encode_substr(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);
MVM_PUBLIC char * MVM_string_ascii_encode(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int32_t translate_newlines);
char * MVM_string_ascii_encode_any(MVMThreadContext *tc, MVMString *str);
char * MVM_string_ascii_encode_malloc(MVMThreadContext *tc, MVMString *str);
