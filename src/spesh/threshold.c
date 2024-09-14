#include "moar.h"

/* Choose the threshold for a given static frame before we start applying
 * specialization to it. */
uint32_t MVM_spesh_threshold(struct MVMThreadContext *tc, MVMStaticFrame *sf) {
    uint32_t bs = sf->body.bytecode_size;
    if (tc->instance->spesh_nodelay)
        return 1;
    if (bs <= 2048)
        return 150;
    else if (bs <= 8192)
        return 200;
    else
        return 300;
}
