void MVM_continuation_reset(MVMThreadContext *tc, MVMObject *tag,
                            MVMObject *code, MVMRegister *res_reg);
void MVM_continuation_control(MVMThreadContext *tc, int64_t protect,
                              MVMObject *tag, MVMObject *code,
                              MVMRegister *res_reg);
void MVM_continuation_invoke(MVMThreadContext *tc, MVMContinuation *cont,
                             MVMObject *code, MVMRegister *res_reg,
                             MVMObject *insert_tag);
