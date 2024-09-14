int64_t MVM_unicode_string_compare(struct MVMThreadContext *tc, MVMString *a, MVMString *b,
    int64_t collation_mode, int64_t lang_mode, int64_t country_mode);

MVMString * MVM_unicode_string_from_name(struct MVMThreadContext *tc, MVMString *name);
