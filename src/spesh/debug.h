void MVM_spesh_debug_printf(struct MVMThreadContext *tc, const char *format, ...);
void MVM_spesh_debug_flush(struct MVMThreadContext *tc);
size_t MVM_spesh_debug_tell(struct MVMThreadContext *tc);

static inline int32_t MVM_spesh_debug_enabled(struct MVMThreadContext *tc) {
    return tc->instance->spesh_log_fh != NULL &&
        (tc->instance->spesh_limit == 0 ||
         tc->instance->spesh_produced == tc->instance->spesh_limit);
}
