/* Maximum args a call can take for us to consider it for optimization. A
 * named arg counts as 2 for these purposes. */
#define MAX_ARGS_FOR_OPT 8

/* Percentage of type tuples that we expect to match at a callsite in order to
 * consider those what we should guard agaisnt (causing a deopt on mismatch).
 * So if this is 99, then we expect 1% of calls may deopt. */
#define MVM_SPESH_CALLSITE_STABLE_PERCENT 99

/* Information we've gathered about the current call we're optimizing, and the
 * arguments it will take. */
struct MVMSpeshCallInfo {
    MVMCallsite   *cs;
    int8_t        arg_is_const[MAX_ARGS_FOR_OPT];
    MVMSpeshFacts *arg_facts[MAX_ARGS_FOR_OPT];
    MVMSpeshIns   *prepargs_ins;
    MVMSpeshBB    *prepargs_bb;
    MVMSpeshIns   *arg_ins[MAX_ARGS_FOR_OPT];
};

void MVM_spesh_optimize(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshPlanned *p);
 int16_t MVM_spesh_add_spesh_slot(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMCollectable *c);
int16_t MVM_spesh_add_spesh_slot_try_reuse(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMCollectable *c);
void MVM_spesh_copy_facts(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshOperand to,
        MVMSpeshOperand from);
void MVM_spesh_copy_facts_resolved(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshFacts *to,
        MVMSpeshFacts *from);
 MVMSpeshFacts * MVM_spesh_get_and_use_facts(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshOperand o);
 MVMSpeshFacts * MVM_spesh_get_facts(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshOperand o);
 void MVM_spesh_use_facts(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshFacts *f);
 MVMString * MVM_spesh_get_string(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshOperand o);
