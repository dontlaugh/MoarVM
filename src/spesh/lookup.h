#ifndef LOOKUP_H
#define LOOKUP_H

MVMObject * MVM_spesh_try_get_how(MVMThreadContext *tc, MVMObject *obj);
MVMint8 MVM_spesh_get_reg_type(MVMThreadContext *tc, MVMSpeshGraph *sg, MVMuint16 reg);
MVMint8 MVM_spesh_get_lex_type(MVMThreadContext *tc, MVMSpeshGraph *sg, MVMuint16 outers, MVMuint16 idx);
MVMuint8 MVM_spesh_get_opr_type(MVMThreadContext *tc, MVMSpeshGraph *sg, MVMSpeshIns *ins, MVMint32 i);
#endif // LOOKUP_H
