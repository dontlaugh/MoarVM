int32_t MVM_unicode_lookup_by_name(struct MVMThreadContext *tc, MVMString *name);
int64_t MVM_unicode_has_property_value(struct MVMThreadContext *tc, MVMGrapheme32 codepoint, int64_t property_code, int64_t property_value_code);
uint32_t MVM_unicode_get_case_change(struct MVMThreadContext *tc, MVMCodepoint codepoint, int32_t case_, const MVMCodepoint **result);
int64_t MVM_unicode_name_to_property_code(struct MVMThreadContext *tc, MVMString *name);
int64_t MVM_unicode_name_to_property_value_code(struct MVMThreadContext *tc, int64_t property_code, MVMString *name);
int32_t MVM_unicode_cname_to_property_value_code(struct MVMThreadContext *tc, int64_t property_code, const char *cname, size_t cname_length);
MVMCodepoint MVM_unicode_find_primary_composite(struct MVMThreadContext *tc, MVMCodepoint l, MVMCodepoint c);

#define MVM_unicode_case_change_type_upper 0
#define MVM_unicode_case_change_type_lower 1
#define MVM_unicode_case_change_type_title 2
#define MVM_unicode_case_change_type_fold  3

void MVM_unicode_init(struct MVMThreadContext *tc);
