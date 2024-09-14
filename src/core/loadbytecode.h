void MVM_load_bytecode_buffer(struct MVMThreadContext *tc, MVMObject *buf);
void MVM_load_bytecode_buffer_to_cu(struct MVMThreadContext *tc, MVMObject *buf, MVMRegister *res);
void MVM_load_bytecode(struct MVMThreadContext *tc, MVMString *filename);
void MVM_load_bytecode_fh(struct MVMThreadContext *tc, MVMObject *oshandle, MVMString *filename);
