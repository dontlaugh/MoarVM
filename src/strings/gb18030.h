MVMString * MVM_string_gb18030_decode(MVMThreadContext *tc, const MVMObject *result_type, const char *gb18030, size_t bytes);
uint32_t MVM_string_gb18030_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
char * MVM_string_gb18030_encode_substr(MVMThreadContext *tc, MVMString *str, MVMuint64 *output_size, MVMint64 start, MVMint64 length, MVMString *replacement, int32_t translate_newlines);


