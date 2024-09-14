MVMCompUnit * MVM_cu_from_bytes(struct MVMThreadContext *tc, uint8_t *bytes, uint32_t size);
MVMCompUnit * MVM_cu_map_from_file(struct MVMThreadContext *tc, const char *filename, int32_t free_filename);
MVMCompUnit * MVM_cu_map_from_file_handle(struct MVMThreadContext *tc, uv_file fd, uint64_t pos);
uint16_t MVM_cu_callsite_add(struct MVMThreadContext *tc, MVMCompUnit *cu, MVMCallsite *cs);
uint32_t MVM_cu_string_add(struct MVMThreadContext *tc, MVMCompUnit *cu, MVMString *str);
MVMString * MVM_cu_obtain_string(struct MVMThreadContext *tc, MVMCompUnit *cu, uint32_t idx);

static inline MVMString * MVM_cu_string(struct MVMThreadContext *tc, MVMCompUnit *cu, uint32_t idx) {
    MVMString *s = cu->body.strings[idx];
    return s ? s : MVM_cu_obtain_string(tc, cu, idx);
}

static inline void MVM_cu_ensure_string_decoded(struct MVMThreadContext *tc, MVMCompUnit *cu, uint32_t idx) {
    if (!cu->body.strings[idx])
        MVM_cu_obtain_string(tc, cu, idx);
}
