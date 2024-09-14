MVMString * MVM_string_gb2312_decode(struct MVMThreadContext *tc, const MVMObject *result_type, const char *gb2312, size_t bytes);
uint32_t MVM_string_gb2312_decodestream(struct MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
char * MVM_string_gb2312_encode_substr(struct MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);


