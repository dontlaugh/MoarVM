/* Aqcuiqre labels */
#pragma once

int32_t MVM_jit_label_before_graph(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshGraph *sg);
int32_t MVM_jit_label_after_graph(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshGraph *sg);
int32_t MVM_jit_label_before_bb(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb);
int32_t MVM_jit_label_before_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb, struct MVMSpeshIns *ins);
int32_t MVM_jit_label_after_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, struct MVMSpeshBB *bb, struct MVMSpeshIns *ins);
/* GIANT HACK UNFORTUNATELY */
int32_t MVM_jit_label_for_obj(struct MVMThreadContext *tc, struct MVMJitGraph *jg, void *obj);

/* Test label category */
int32_t MVM_jit_label_is_for_graph(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label);
int32_t MVM_jit_label_is_for_bb(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label);
int32_t MVM_jit_label_is_for_ins(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label);

/* Internal labels aren't actually assigned their 'final number' before
 * compilation, so this is *NOT VALID* during graph building */
int32_t MVM_jit_label_is_internal(struct MVMThreadContext *tc, struct MVMJitGraph *jg, int32_t label);
