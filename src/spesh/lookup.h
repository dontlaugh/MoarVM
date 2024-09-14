MVMObject * MVM_spesh_try_get_how(struct MVMThreadContext *tc, MVMObject *obj);
int8_t MVM_spesh_get_reg_type(struct MVMThreadContext *tc, MVMSpeshGraph *sg, uint16_t reg);
int8_t MVM_spesh_get_lex_type(struct MVMThreadContext *tc, MVMSpeshGraph *sg, uint16_t outers, uint16_t idx);
uint8_t MVM_spesh_get_opr_type(struct MVMThreadContext *tc, MVMSpeshGraph *sg, MVMSpeshIns *ins, int32_t i);
