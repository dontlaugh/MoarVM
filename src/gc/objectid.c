#include "moar.h"

/* Gets a stable identifier for an object, which will not change even if the
 * GC moves the object. */
uint64_t MVM_gc_object_id(MVMThreadContext *tc, MVMObject *obj) {
    uint64_t id;

    /* If it's already in the old generation, just use memory address, as
     * gen2 objects never move. */
    if (obj->header.flags2 & MVM_CF_SECOND_GEN) {
        id = (uintptr_t)obj;
    }

    /* Otherwise, see if we already have a persistent object ID. */
    else {
        uv_mutex_lock(&tc->instance->mutex_object_ids);
        if (obj->header.flags1 & MVM_CF_HAS_OBJECT_ID) {
            /* Has one, so just look up by address in the hash ID hash. */

            struct MVMPtrHashEntry *entry = MVM_ptr_hash_fetch(tc, &tc->instance->object_ids, obj);
            assert(entry);
            id = entry->value;
        }
        else {
            /* Hasn't got one; allocate it a place in gen2 and make an entry
             * in the persistent object ID hash. */
            id = (uintptr_t)MVM_gc_gen2_allocate_zeroed(tc->gen2, obj->header.size);
            MVM_ptr_hash_insert(tc, &tc->instance->object_ids, obj, id);
            obj->header.flags1 |= MVM_CF_HAS_OBJECT_ID;
        }
        uv_mutex_unlock(&tc->instance->mutex_object_ids);
    }

    return id;
}

/* If an object with an entry here lives long enough to be promoted to gen2,
 * this removes the hash entry for it and returns the pre-allocated gen2
 * address. */
void * MVM_gc_object_id_use_allocation(MVMThreadContext *tc, MVMCollectable *item) {
    uv_mutex_lock(&tc->instance->mutex_object_ids);
    void *addr = (void *) MVM_ptr_hash_fetch_and_delete(tc, &tc->instance->object_ids, item);
    item->flags1 ^= MVM_CF_HAS_OBJECT_ID;
    uv_mutex_unlock(&tc->instance->mutex_object_ids);
    return addr;
}

/* Clears hash entry for a persistent object ID when an object dies in the
 * nursery. */
void MVM_gc_object_id_clear(MVMThreadContext *tc, MVMCollectable *item) {
    uv_mutex_lock(&tc->instance->mutex_object_ids);
    (void) MVM_ptr_hash_fetch_and_delete(tc, &tc->instance->object_ids, item);
    uv_mutex_unlock(&tc->instance->mutex_object_ids);
}
