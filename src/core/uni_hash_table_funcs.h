/* These are private. We need them out here for the inline functions. Use those.
 */
/* See comments in hash_allocate_common (and elsewhere) before changing the
 * load factor, or UNI_MIN_SIZE_BASE_2 or MVM_HASH_INITIAL_BITS_IN_METADATA,
 * and test with assertions enabled. The current choices permit certain
 * optimisation assumptions in parts of the code. */
#define MVM_UNI_HASH_LOAD_FACTOR 0.75
MVM_STATIC_INLINE uint32_t MVM_uni_hash_official_size(const struct MVMUniHashTableControl *control) {
    return 1 << (uint32_t)control->official_size_log2;
}
/* -1 because...
 * probe distance of 1 is the correct bucket.
 * hence for a value whose ideal slot is the last bucket, it's *in* the official
 * allocation.
 * probe distance of 2 is the first extra bucket beyond the official allocation
 * probe distance of 255 is the 254th beyond the official allocation.
 */
MVM_STATIC_INLINE uint32_t MVM_uni_hash_allocated_items(const struct MVMUniHashTableControl *control) {
    return MVM_uni_hash_official_size(control) + control->max_probe_distance_limit - 1;
}
MVM_STATIC_INLINE uint32_t MVM_uni_hash_max_items(const struct MVMUniHashTableControl *control) {
    return MVM_uni_hash_official_size(control) * MVM_UNI_HASH_LOAD_FACTOR;
}
MVM_STATIC_INLINE MVMuint8 *MVM_uni_hash_metadata(const struct MVMUniHashTableControl *control) {
    return (MVMuint8 *) control + sizeof(struct MVMUniHashTableControl);
}
MVM_STATIC_INLINE MVMuint8 *MVM_uni_hash_entries(const struct MVMUniHashTableControl *control) {
    return (MVMuint8 *) control - sizeof(struct MVMUniHashEntry);
}

/* Frees the entire contents of the hash, leaving you just the hashtable itself,
 * which you allocated (heap, stack, inside another struct, wherever) */
void MVM_uni_hash_demolish(MVMThreadContext *tc, MVMUniHashTable *hashtable);
/* and then free memory if you allocated it */

void MVM_uni_hash_build(MVMThreadContext *tc,
                        struct MVMUniHashTable *hashtable,
                        uint32_t entries);

MVM_STATIC_INLINE int MVM_uni_hash_is_empty(MVMThreadContext *tc,
                                            MVMUniHashTable *hashtable) {
    struct MVMUniHashTableControl *control = hashtable->table;
    return !control || control->cur_items == 0;
}

/* Unicode names (etc) are not under the control of an external attacker [:-)]
 * so we don't need a cryptographic hash function here. I'm assuming that
 * 32 bit FNV1a is good enough and fast enough to be useful. */
MVM_STATIC_INLINE uint32_t MVM_uni_hash_code(const char *key, size_t len) {
    const char *const end = key + len;
    uint32_t hash = 0x811c9dc5;
    while (key < end) {
        hash ^= *key++;
        hash *= 0x01000193;
    }
    /* "finalise" it by multiplying by the constant for Fibonacci bucket
       determination. */
    return hash * 0x9e3779b7;
}

MVM_STATIC_INLINE struct MVM_hash_loop_state
MVM_uni_hash_create_loop_state(struct MVMUniHashTableControl *control,
                               uint32_t hash_val) {
    struct MVM_hash_loop_state retval;
    retval.entry_size = sizeof(struct MVMUniHashEntry);
    retval.metadata_increment = 1 << control->metadata_hash_bits;
    retval.metadata_hash_mask = retval.metadata_increment - 1;
    retval.probe_distance_shift = control->metadata_hash_bits;
    retval.max_probe_distance = control->max_probe_distance;

    unsigned int used_hash_bits = hash_val >> control->key_right_shift;
    retval.probe_distance = retval.metadata_increment | (used_hash_bits & retval.metadata_hash_mask);
    MVMHashNumItems bucket = used_hash_bits >> control->metadata_hash_bits;
    if (!control->metadata_hash_bits) {
        assert(retval.probe_distance == 1);
        assert(retval.metadata_hash_mask == 0);
        assert(bucket == used_hash_bits);
    }

    retval.entry_raw = MVM_uni_hash_entries(control) - bucket * retval.entry_size;
    retval.metadata = MVM_uni_hash_metadata(control) + bucket;
    return retval;
}

/* UNCONDITIONALLY creates a new hash entry with the given key and value.
 * Doesn't check if the key already exists. Use with care. */
void MVM_uni_hash_insert(MVMThreadContext *tc,
                         MVMUniHashTable *hashtable,
                         const char *key,
                         int32_t value);

MVM_STATIC_INLINE struct MVMUniHashEntry *MVM_uni_hash_fetch(MVMThreadContext *tc,
                                                              MVMUniHashTable *hashtable,
                                                              const char *key) {
    if (MVM_uni_hash_is_empty(tc, hashtable)) {
        return NULL;
    }

    struct MVMUniHashTableControl *control = hashtable->table;
    uint32_t hash_val = MVM_uni_hash_code(key, strlen(key));
    struct MVM_hash_loop_state ls = MVM_uni_hash_create_loop_state(control,
                                                                   hash_val);

    while (1) {
        if (*ls.metadata == ls.probe_distance) {
            struct MVMUniHashEntry *entry = (struct MVMUniHashEntry *) ls.entry_raw;
            if (entry->hash_val == hash_val && 0 == strcmp(entry->key, key)) {
                return entry;
            }
        }
        /* There's a sentinel at the end. This will terminate: */
        else if (*ls.metadata < ls.probe_distance) {
            /* So, if we hit 0, the bucket is empty. "Not found".
               If we hit something with a lower probe distance then...
               consider what would have happened had this key been inserted into
               the hash table - it would have stolen this slot, and the key we
               find here now would have been displaced further on. Hence, the key
               we seek can't be in the hash table. */
            return NULL;
        }
        ls.probe_distance += ls.metadata_increment;
        ++ls.metadata;
        ls.entry_raw -= ls.entry_size;
        assert(ls.probe_distance < (ls.max_probe_distance + 2) * ls.metadata_increment);
        assert(ls.metadata < MVM_uni_hash_metadata(control) + MVM_uni_hash_official_size(control) + MVM_uni_hash_max_items(control));
        assert(ls.metadata < MVM_uni_hash_metadata(control) + MVM_uni_hash_official_size(control) + 256);
    }
}
