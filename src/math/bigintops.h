#pragma once



MVMObject * MVM_bigint_abs(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
MVMObject * MVM_bigint_neg(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
MVMObject * MVM_bigint_not(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
/* unused */
/* void MVM_bigint_sqrt(MVMObject *b, MVMObject *a); */

MVMObject * MVM_bigint_add(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_add(struct MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_sub(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_sub(struct MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_mul(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
void MVM_bigint_fallback_mul(struct MVMThreadContext *tc, MVMP6bigintBody *ba, MVMP6bigintBody *bb, MVMP6bigintBody *bc);
MVMObject * MVM_bigint_div(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_mod(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_pow(struct MVMThreadContext *tc, MVMObject *a, MVMObject *b,
        MVMObject *num_type, MVMObject *int_type);
MVMObject * MVM_bigint_gcd(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_lcm(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_or(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_xor(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_and(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_shl(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, int64_t n);
MVMObject * MVM_bigint_shr(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, int64_t n);

MVMObject * MVM_bigint_expmod(struct MVMThreadContext *tc, MVMObject *result, MVMObject *a, MVMObject *b, MVMObject *c);

int64_t MVM_bigint_cmp(struct MVMThreadContext *tc, MVMObject *a, MVMObject *b);

MVMObject * MVM_bigint_from_bigint(struct MVMThreadContext *tc, MVMObject *result_type, MVMObject *a);
void MVM_bigint_from_str(struct MVMThreadContext *tc, MVMObject *a, const char *buf);
MVMString * MVM_bigint_to_str(struct MVMThreadContext *tc, MVMObject *a, int base);
double MVM_bigint_to_num(struct MVMThreadContext *tc, MVMObject *a);
MVMObject *MVM_bigint_from_num(struct MVMThreadContext *tc, MVMObject *result_type, double n);
double MVM_bigint_div_num(struct MVMThreadContext *tc, MVMObject *a, MVMObject *b);
MVMObject * MVM_bigint_rand(struct MVMThreadContext *tc, MVMObject *a, MVMObject *b);
int64_t MVM_bigint_is_prime(struct MVMThreadContext *tc, MVMObject *a);
MVMObject * MVM_bigint_radix(struct MVMThreadContext *tc, int64_t radix, MVMString *str, int64_t offset, int64_t flag, MVMObject *type);
int64_t MVM_bigint_is_big(struct MVMThreadContext *tc, MVMObject *a);
int64_t MVM_bigint_bool(struct MVMThreadContext *tc, MVMObject *a);
MVMObject * MVM_coerce_sI(struct MVMThreadContext *tc, MVMString *s, MVMObject *type);
