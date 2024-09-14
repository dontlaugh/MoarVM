#include "moar.h"
#include <stdint.h>
#include "graph.h"
#include "label.h"



int32_t MVM_jit_label_before_bb(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb) {
    return bb->idx;
}

int32_t MVM_jit_label_before_graph(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshGraph *sg) {
    return 0;
}

int32_t MVM_jit_label_after_graph(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshGraph *sg) {
    /* Larger than all basic block labels */
    return sg->num_bbs;
}


int32_t MVM_jit_label_before_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb, struct MVMSpeshIns *ins) {
    /* PHI nodes are always at the start of a basic block, so this not is really necessary */
    while (ins->prev && ins->prev->info->opcode == MVM_SSA_PHI) {
        ins = ins->prev;
    }
    /* Without predecessor instructions this is the same as the basic block */
    if (!ins->prev) {
        return MVM_jit_label_before_bb(tc, jg, bb);
    }
    /* Requires a separate label */
    return MVM_jit_label_for_obj(tc, jg, ins);
}

int32_t MVM_jit_label_after_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb, struct MVMSpeshIns *ins) {
    /* A label after this instruction equals a label before the next */
    if (ins->next) {
        return MVM_jit_label_before_ins(tc, jg, bb, ins->next);
    }
    /* Or before the next basic block */
    if (bb->linear_next) {
        return MVM_jit_label_before_bb(tc, jg, bb->linear_next);
    }
    /* And in the edge case after the graph */
    return MVM_jit_label_after_graph(tc, jg, jg->sg);
}

int32_t MVM_jit_label_for_obj(struct MVMThreadContext *tc, struct MVMJitGraph *jg, void *obj) {
    int32_t i;
    /* Reverse search; it is pretty likely we've seen this ins just before */
    i      = jg->obj_labels_num;
    while (i--) {
        if (jg->obj_labels[i] == obj) {
            return i + jg->obj_label_ofs;
        }
    }
    /* Add and return new instruction label */
    MVM_VECTOR_PUSH(jg->obj_labels, obj);
    return jg->obj_labels_num - 1 + jg->obj_label_ofs;
}

int32_t MVM_jit_label_is_for_graph(struct struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label) {
    return label == 0 || (uint32_t)label == jg->sg->num_bbs;
}

int32_t MVM_jit_label_is_for_bb(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label) {
    return label > 0 && (uint32_t)label < jg->sg->num_bbs;
}

int32_t MVM_jit_label_is_for_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label) {
    return (uint32_t)label > jg->sg->num_bbs && (uint32_t)label <= jg->sg->num_bbs + jg->obj_labels_num;
}

int32_t MVM_jit_label_is_internal(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label) {
    /* WARNING: This is *NOT VALID* during jit graph building */
    return label >= jg->num_labels;
}
