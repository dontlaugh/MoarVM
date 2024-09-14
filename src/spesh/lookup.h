MVMObject * MVM_spesh_try_get_how(MVMThreadContext *tc, MVMObject *obj);
int8_t MVM_spesh_get_reg_type(MVMThreadContext *tc, MVMSpeshGraph *sg, uint16_t reg);
int8_t MVM_spesh_get_lex_type(MVMThreadContext *tc, MVMSpeshGraph *sg, uint16_t outers, uint16_t idx);
uint8_t MVM_spesh_get_opr_type(MVMThreadContext *tc, MVMSpeshGraph *sg, MVMSpeshIns *ins, int32_t i);
