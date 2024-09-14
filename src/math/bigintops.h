MVMObject * MVM_bigint_abs(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
MVMObject * MVM_bigint_neg(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
MVMObject * MVM_bigint_not(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
/* unused */
/* void MVM_bigint_sqrt(MVMObject *b, MVMObject *a); */

MVMObject * MVM_bigint_add(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_add(MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_sub(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_sub(MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_mul(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_mul(MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_div(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_mod(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_pow(MVMThreadContext *tc, MVMObject *a, MVMObject *b,
        MVMObject *num_type, MVMObject *int_type);
MVMObject * MVM_bigint_gcd(MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_lcm(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_or(MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_xor(MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_and(MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_shl(MVMThreadContext *tc, MVMObject *result, MVMObject *a, int64_t n);
MVMObject * MVM_bigint_shr(MVMThreadContext *tc, MVMObject *result, MVMObject *a, int64_t n);

MVMObject * MVM_bigint_expmod(MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b, MVMObject *c);

int64_t MVM_bigint_cmp(MVMThreadContext *tc, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_from_bigint(MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
void MVM_bigint_from_str(MVMThreadContext *tc, MVMObject *a, const char *buf);
MVMString * MVM_bigint_to_str(MVMThreadContext *tc, MVMObject *a, int base);
double MVM_bigint_to_num(MVMThreadContext *tc, MVMObject *a);
MVMObject *MVM_bigint_from_num(MVMThreadContext *tc, MVMObject *result_type, double n);
double MVM_bigint_div_num(MVMThreadContext *tc, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_rand(MVMThreadContext *tc, MVMObject *a, MVMObject *b);
int64_t MVM_bigint_is_prime(MVMThreadContext *tc, MVMObject *a);
MVMObject * MVM_bigint_radix(MVMThreadContext *tc, int64_t radix, MVMString *str, int64_t offset, int64_t flag, MVMObject *type);
int64_t MVM_bigint_is_big(MVMThreadContext *tc, MVMObject *a);
int64_t MVM_bigint_bool(MVMThreadContext *tc, MVMObject *a);
MVMObject * MVM_coerce_sI(MVMThreadContext *tc, MVMString *s, MVMObject *type);
