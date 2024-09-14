#ifndef MATH_NUM_H
#define MATH_NUM_H

#ifdef _WIN32
#include <float.h>
#endif
#include <math.h>

#if defined(INFINITY) && !defined(_AIX)
static const double MVM_NUM_POSINF =  INFINITY;
static const double MVM_NUM_NEGINF = -INFINITY;
#else
#  ifdef _MSC_VER
#define MVM_NUM_POSINF  (DBL_MAX+DBL_MAX)
#define MVM_NUM_NEGINF -(DBL_MAX+DBL_MAX)
#  else
static const double MVM_NUM_POSINF =  1.0 / 0.0;
static const double MVM_NUM_NEGINF = -1.0 / 0.0;
#  endif
#endif

#if defined(NAN) && !defined(_AIX)
static const double MVM_NUM_NAN = NAN;
#else
#  ifdef _MSC_VER
#define MVM_NUM_NAN (MVM_NUM_POSINF-MVM_NUM_POSINF)
#  else
static const double MVM_NUM_NAN = 0.0 / 0.0;
#  endif
#endif

static inline int MVM_num_isnanorinf(struct MVMThreadContext *tc, double n) {
#if defined(MVM_HAS_ISINF) && defined(MVM_HAS_ISNAN)
    return isinf(n) || isnan(n);
#else
    return n == MVM_NUM_POSINF || n == MVM_NUM_NEGINF || n != n;
#endif
}

static inline double MVM_num_posinf(struct MVMThreadContext *tc) {
    return MVM_NUM_POSINF;
}

static inline double MVM_num_neginf(struct MVMThreadContext *tc) {
    return MVM_NUM_NEGINF;
}

static inline double MVM_num_nan(struct MVMThreadContext *tc) {
    return MVM_NUM_NAN;
}

static inline int MVM_num_isnegzero(struct MVMThreadContext *tc, double n) {
#ifdef MVM_HAS_SIGNBIT
    return n == 0 && signbit(n);
#else
    return n == 0 && 1.0 / n == MVM_NUM_NEGINF;
#endif
}

#endif
