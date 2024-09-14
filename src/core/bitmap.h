/* basic bitmap implementation */
typedef uint64_t MVMBitmap;

/* Efficient find-first-set; on x86, using `bsf` primitive operation; something
 * else on other architectures. */
#ifdef __GNUC__
/* also works for clang and friends */
#define MVM_FFS(x) __builtin_ffsll(x)
#elif defined(_MSC_VER)
static inline uint32_t MVM_FFS(MVMBitmap x) {
    uint32_t i = 0;
    if (_BitScanForward64(&i, x) == 0)
        return 0;
    return i + 1;
}
#else
/* fallback, note that i=0 if no bits are set */
static inline uint32_t MVM_FFS(MVMBitmap x) {
    uint32_t i = 0;
    while (x) {
        if (x & (1 << i++))
            break;
    }
    return i;
}
#endif


/* NB - make this a separate 'library', use it for register bitmap */
/* Witness the elegance of the bitmap for our purposes. */
static inline void MVM_bitmap_set(MVMBitmap *bits, int32_t idx) {
    bits[idx >> 6] |= (UINT64_C(1) << (idx & 0x3f));
}

static inline void MVM_bitmap_set_low(MVMBitmap *bits, int32_t idx) {
    *bits |= (UINT64_C(1) << (idx & 0x3f));
}

static inline uint64_t MVM_bitmap_get(MVMBitmap *bits, int32_t idx) {
    return bits[idx >> 6] & (UINT64_C(1) << (idx & 0x3f));
}

static inline uint64_t MVM_bitmap_get_low(MVMBitmap bits, int32_t idx ) {
    return bits & (UINT64_C(1) << (idx & 0x3f));
}

static inline void MVM_bitmap_delete(MVMBitmap *bits, int32_t idx) {
    bits[idx >> 6] &= ~(UINT64_C(1) << (idx & 0x3f));
}

static inline void MVM_bitmap_union(MVMBitmap *out, MVMBitmap *a, MVMBitmap *b, int32_t n) {
    int32_t i;
    for (i = 0; i < n; i++) {
        out[i] = a[i] | b[i];
    }
}

static inline void MVM_bitmap_difference(MVMBitmap *out, MVMBitmap *a, MVMBitmap *b, int32_t n) {
    int32_t i;
    for (i = 0; i < n; i++) {
        out[i] = a[i] ^ b[i];
    }
}

static inline void MVM_bitmap_intersection(MVMBitmap *out, MVMBitmap *a, MVMBitmap *b, int32_t n) {
    int32_t i;
    for (i = 0; i < n; i++) {
        out[i] = a[i] & b[i];
    }
}

static inline MVMBitmap MVM_bitmap_with_set(MVMBitmap bitmap, uint8_t bit) {
    return bitmap | (1 << bit);
}

static inline MVMBitmap MVM_bitmap_with_clear(MVMBitmap bitmap, uint8_t bit) {
    return bitmap & ~(1 << bit);
}
