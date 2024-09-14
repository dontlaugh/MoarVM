/* Aqcuiqre labels */
int32_t MVM_jit_label_before_graph(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshGraph *sg);
int32_t MVM_jit_label_after_graph(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshGraph *sg);
int32_t MVM_jit_label_before_bb(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshBB *bb);
int32_t MVM_jit_label_before_ins(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshBB *bb, MVMSpeshIns *ins);
int32_t MVM_jit_label_after_ins(MVMThreadContext *tc, MVMJitGraph *jg, MVMSpeshBB *bb, MVMSpeshIns *ins);
/* GIANT HACK UNFORTUNATELY */
int32_t MVM_jit_label_for_obj(MVMThreadContext *tc, MVMJitGraph *jg, void *obj);

/* Test label category */
int32_t MVM_jit_label_is_for_graph(MVMThreadContext *tc, MVMJitGraph *jg, int32_t label);
int32_t MVM_jit_label_is_for_bb(MVMThreadContext *tc, MVMJitGraph *jg, int32_t label);
int32_t MVM_jit_label_is_for_ins(MVMThreadContext *tc, MVMJitGraph *jg, int32_t label);

/* Internal labels aren't actually assigned their 'final number' before
 * compilation, so this is *NOT VALID* during graph building */
int32_t MVM_jit_label_is_internal(MVMThreadContext *tc, MVMJitGraph *jg, int32_t label);
