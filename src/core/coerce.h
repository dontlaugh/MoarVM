/* Boolification. */
int64_t MVM_coerce_istrue_s(struct MVMThreadContext *tc, MVMString *str);

/* Stringification. */
MVMString * MVM_coerce_i_s(struct MVMThreadContext *tc, int64_t i);
MVMString * MVM_coerce_u_s(struct MVMThreadContext *tc, uint64_t i);
MVMString * MVM_coerce_n_s(struct MVMThreadContext *tc, double n);

/* Numification. */
int64_t MVM_coerce_s_i(struct MVMThreadContext *tc, MVMString *s);
uint64_t MVM_coerce_s_u(struct MVMThreadContext *tc, MVMString *s);
int64_t MVM_coerce_simple_intify(struct MVMThreadContext *tc, MVMObject *obj);
MVMObject* MVM_radix(struct MVMThreadContext *tc, int64_t radix, MVMString *str, int64_t offset, int64_t flag);

/* Size of the int to string coercion cache (we cache 0 ..^ this). */
#define MVM_INT_TO_STR_CACHE_SIZE 65
