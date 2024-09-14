#pragma once

struct MVMString * MVM_string_utf8_decode(struct MVMThreadContext *tc, const struct MVMObject *result_type, const char *utf8, size_t bytes);
struct MVMString * MVM_string_utf8_decode_strip_bom(struct MVMThreadContext *tc, const struct MVMObject *result_type, const char *utf8, size_t bytes);
uint32_t MVM_string_utf8_decodestream(struct MVMThreadContext *tc, struct  MVMDecodeStream *ds, const uint32_t *stopper_chars, struct MVMDecodeStreamSeparators *seps);
char * MVM_string_utf8_encode_substr(struct MVMThreadContext *tc,
        struct MVMString *str, uint64_t *output_size, int64_t start, int64_t length, struct MVMString *replacement, int32_t translate_newlines);
char * MVM_string_utf8_encode(struct MVMThreadContext *tc, struct  MVMString *str, uint64_t *output_size, int32_t translate_newlines);
char * MVM_string_utf8_encode_C_string(struct MVMThreadContext *tc, struct MVMString *str);
char * MVM_string_utf8_encode_C_string_malloc(struct MVMThreadContext *tc, struct  MVMString *str);
char * MVM_string_utf8_maybe_encode_C_string(struct MVMThreadContext *tc, struct  MVMString *str);
void MVM_string_utf8_throw_encoding_exception(struct MVMThreadContext *tc, int32_t cp);
