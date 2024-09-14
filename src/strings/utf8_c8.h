MVM_PUBLIC MVMString * MVM_string_utf8_c8_decode(MVMThreadContext *tc, const MVMObject *result_type, const char *utf8, size_t bytes);
MVM_PUBLIC uint32_t MVM_string_utf8_c8_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps, int32_t eof);
MVM_PUBLIC char * MVM_string_utf8_c8_encode_substr(MVMThreadContext *tc,
        MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement);
MVM_PUBLIC char * MVM_string_utf8_c8_encode(MVMThreadContext *tc, MVMString *str, uint64_t *output_size);
MVM_PUBLIC char * MVM_string_utf8_c8_encode_C_string(MVMThreadContext *tc, MVMString *str);
