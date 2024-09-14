char * MVM_string_shiftjis_encode_substr(MVMThreadContext *tc, MVMString *str,
        uint64_t *output_size, int64_t start, int64_t length, MVMString *replacement,
        int32_t translate_newlines, int64_t config);
uint32_t MVM_string_shiftjis_decodestream(MVMThreadContext *tc, MVMDecodeStream *ds,
                                         const uint32_t *stopper_chars,
                                         MVMDecodeStreamSeparators *seps);
MVMString * MVM_string_shiftjis_decode(MVMThreadContext *tc,
        const MVMObject *result_type, char *windows125X_c, size_t num_bytes,
        MVMString *replacement, int64_t config);
