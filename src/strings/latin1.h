MVMString * MVM_string_latin1_decode(MVMThreadContext *tc, const MVMObject *result_type, char *latin1, size_t bytes);
MVM_PUBLIC uint32_t MVM_string_latin1_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds, const uint32_t *stopper_chars, MVMDecodeStreamSeparators *seps);
char * MVM_string_latin1_encode_substr(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement, int32_t translate_newlines);
char * MVM_string_latin1_encode(MVMThreadContext *tc, MVMString *str, uint64_t *output_size, int32_t translate_newlines);
