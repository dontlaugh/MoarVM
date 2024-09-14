/* Information held about a dispatch with resume initialization arguments. */
struct MVMSpeshResumeInit {
    /* The dispatch program. */
    MVMDispProgram *dp;

    /* The deopt index of the dispatch. Deoptimization already knows how to
     * locate this, so we can reuse that logic in order to work out the
     * deopt index of the current dispatch. */
    int32_t deopt_idx;

    /* The index of the resumption within the dispatch program. */
    uint16_t res_idx;

    /* The number of the register reserved to hold resumption init
     * state. Populated at code-gen time. */
    uint16_t state_register;

    /* Lookup table of resume init arguments to registers in the case the
     * resume init arg is from either an argument or a tempoary; junk for
     * constants, which should be read out of the dispatch program. Also
     * populated at code-gen time. */
    uint16_t *init_registers;
};

size_t MVM_spesh_disp_dispatch_op_info_size(MVMThreadContext *tc,
        const MVMOpInfo *base_info, MVMCallsite *callsite);
void MVM_spesh_disp_initialize_dispatch_op_info(MVMThreadContext *tc,
        const MVMOpInfo *base_info, MVMCallsite *cs, MVMOpInfo *dispatch_info);
size_t MVM_spesh_disp_resumption_op_info_size(MVMThreadContext *tc,
        MVMDispProgram *dp, uint16_t res_idx);
MVMOpInfo * MVM_spesh_disp_initialize_resumption_op_info(MVMThreadContext *tc,
        MVMDispProgram *dp, uint16_t res_idx, MVMOpInfo *res_info);
MVMCallsite * MVM_spesh_disp_callsite_for_dispatch_op(uint16_t opcode, uint8_t *args,
        MVMCompUnit *cu);
int MVM_spesh_disp_optimize(MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshPlanned *p, MVMSpeshIns *ins, MVMSpeshIns **next_ins);
