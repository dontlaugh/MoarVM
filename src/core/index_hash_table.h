/* A Better Hash.

A C implementation of https://github.com/martinus/robin-hood-hashing
by Martin Ankerl <http://martin.ankerl.com>

Better than what we had. Not better than his. His is hard to beat.

His design is for a Robin Hood hash (ie open addressing, Robin Hood probing)
with:

* a contiguous block of memory
* hash into 2**n slots
* instead of wrapping round from the end to the start of the array when
  probing, *actually* allocate some extra slots at the end, sufficient to cover
  the maximum permitted probe length
* store metadata for free/used (with the offset from the ideal slot) in a byte
  array immediately after the data slots
* store the offset in the top n bits of the byte, use the lower 8-n bits
  (possibly 0) to store (more) bits of the key's hash in the rest.
  (where n might be 0 - n is updated dynamically whenever a probe would overflow
   the currently permitted maximum)
  (so m bits of the hash are used to pick the ideal slot, and a different n are
   in the metadata, meaning that misses can be rejected more often)
* sentinel byte at the end of the metadata to cause the iterator to efficiently
  terminate
* setting max_items to 0 to force a resize before even trying another allocation
* when inserting and stealing a slot, move the next items up in bulk
  (ie don't implement it as "swap the new element with the evicted element and
  carry on inserting - the rest of the elements are already in a valid probe
  order, so just update *all* their metadata bytes, and then memmove them)

it's incredibly flexible (up to, automatically choosing whether to allocate
the value object inline in the hash, or indrected via a pointer), but
implemented as a C++ template.

Whereas we need something in C. Only for small structures, so they can always
go inline. And it turns out, our keys are always pointers, and easily "hashed"
(either because they are, because they point to something that caches its
hash value, or because we fake it and explicitly store the hash value.)

Not all the optimisations described above are in place yet. Starting with
"minimum viable product", with a design that should support adding them.

*/
#ifndef INDEX_HASH_TABLE_H
#define INDEX_HASH_TABLE_H

struct MVMIndexHashTableControl {
    MVMHashNumItems cur_items;
    MVMHashNumItems max_items; /* hit this and we grow */
    MVMuint8 official_size_log2;
    MVMuint8 key_right_shift;
    /* This is the maximum probe distance we can use without updating the
     * metadata. It might not *yet* be the maximum probe distance possible for
     * the official_size. */
    MVMuint8 max_probe_distance;
    /* This is the maximum probe distance possible for the official size.
     * We can (re)calcuate this from other values in the struct, but it's easier
     * to cache it as we have the space. */
    MVMuint8 max_probe_distance_limit;
    MVMuint8 metadata_hash_bits;
};

struct MVMIndexHashTable {
    struct MVMIndexHashTableControl *table;
};

struct MVMIndexHashEntry {
    MVMuint32 index;
};

#define MVM_INDEX_HASH_NOT_FOUND 0xFFFFFFFF
#endif // INDEX_HASH_TABLE_H
