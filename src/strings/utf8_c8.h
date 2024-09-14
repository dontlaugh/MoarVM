 MVMString * MVM_string_utf8_c8_decode(struct MVMThreadContext *tc, const MVMObject *result_type, const char *utf8, size_t bytes);
 uint32_t MVM_string_utf8_c8_decodestream(struct MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps, int32_t eof);
 char * MVM_string_utf8_c8_encode_substr(struct MVMThreadContext *tc,
        MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement);
 char * MVM_string_utf8_c8_encode(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size);
 char * MVM_string_utf8_c8_encode_C_string(struct MVMThreadContext *tc, MVMString *str);
