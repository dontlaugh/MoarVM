struct MVMIntConstCache {
    MVMObject *types[4];
    MVMObject *cache[4][16];
};

#define MVM_INTCACHE_RANGE_CHECK(value) ((value) >= -1 && (value) < 15)

void MVM_intcache_for(struct MVMThreadContext *tc, MVMObject *type);
MVMObject *MVM_intcache_get(struct MVMThreadContext *tc, MVMObject *type, int64_t value);
int32_t MVM_intcache_type_index(struct MVMThreadContext *tc, MVMObject *type);
