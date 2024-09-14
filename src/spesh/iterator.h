struct MVMSpeshIterator {
    MVMSpeshGraph *graph;
    MVMSpeshBB    *bb;
    MVMSpeshIns   *ins;
};

void MVM_spesh_iterator_init(struct MVMThreadContext *tc, MVMSpeshIterator *iterator, MVMSpeshGraph *graph);
void MVM_spesh_iterator_copy(struct MVMThreadContext *tc, MVMSpeshIterator *a, MVMSpeshIterator *b);
MVMSpeshIns * MVM_spesh_iterator_next_ins(struct MVMThreadContext *tc, MVMSpeshIterator *iterator);
MVMSpeshBB  * MVM_spesh_iterator_next_bb(struct MVMThreadContext *tc, MVMSpeshIterator *iterator);
void MVM_spesh_iterator_skip_phi(struct MVMThreadContext *tc, MVMSpeshIterator *iterator);
