 void MVM_spesh_manipulate_delete_ins(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins);
 void MVM_spesh_manipulate_cleanup_ins_deps(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshIns *ins);
 void MVM_spesh_manipulate_insert_ins(struct MVMThreadContext *tc, MVMSpeshBB *bb, MVMSpeshIns *previous, MVMSpeshIns *to_insert);
 void MVM_spesh_manipulate_insert_goto(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins, MVMSpeshBB *target);
void MVM_spesh_manipulate_add_successor(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshBB *succ);
void MVM_spesh_manipulate_remove_successor(struct MVMThreadContext *tc, MVMSpeshBB *bb, MVMSpeshBB *succ);
 void MVM_spesh_manipulate_remove_handler_successors(struct MVMThreadContext *tc, MVMSpeshBB *bb);
MVMSpeshOperand MVM_spesh_manipulate_get_temp_reg(struct MVMThreadContext *tc, MVMSpeshGraph *g, uint16_t kind);
void MVM_spesh_manipulate_release_temp_reg(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshOperand temp);
MVMSpeshOperand MVM_spesh_manipulate_new_version(struct MVMThreadContext *tc, MVMSpeshGraph *g, uint16_t orig);
MVMSpeshOperand MVM_spesh_manipulate_split_version(struct MVMThreadContext *tc, MVMSpeshGraph *g,
        MVMSpeshOperand split, MVMSpeshBB *bb, MVMSpeshIns *at);
uint16_t MVM_spesh_manipulate_get_unique_reg(struct MVMThreadContext *tc, MVMSpeshGraph *g, uint16_t kind);
uint16_t MVM_spesh_manipulate_get_current_version(struct MVMThreadContext *tc, MVMSpeshGraph *g,
        uint16_t orig);
MVMSpeshBB *MVM_spesh_manipulate_split_BB_at(struct MVMThreadContext *tc, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins);
