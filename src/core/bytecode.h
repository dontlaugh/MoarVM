/* Bytecode annotation, post-resolution. */
struct MVMBytecodeAnnotation {
    uint32_t bytecode_offset;
    uint32_t filename_string_heap_index;
    uint32_t line_number;
    uint32_t ann_offset;
    uint32_t ann_index;
};

void MVM_bytecode_unpack(struct MVMThreadContext *tc, MVMCompUnit *cu);
MVMBytecodeAnnotation * MVM_bytecode_resolve_annotation(struct MVMThreadContext *tc, MVMStaticFrameBody *sfb, uint32_t offset);
void MVM_bytecode_advance_annotation(struct MVMThreadContext *tc, MVMStaticFrameBody *sfb, MVMBytecodeAnnotation *ba);
void MVM_bytecode_finish_frame(struct MVMThreadContext *tc, MVMCompUnit *cu, MVMStaticFrame *sf, int32_t dump_only);
uint8_t MVM_bytecode_find_static_lexical_scref(struct MVMThreadContext *tc, MVMCompUnit *cu, MVMStaticFrame *sf, uint16_t index, uint32_t *sc, uint32_t *id);

/* Looks up op info including for ext ops; doesn't sanity check, since we
 * should be working on code that already pass validation. */
static inline const MVMOpInfo * MVM_bytecode_get_validated_op_info(struct MVMThreadContext *tc,
        MVMCompUnit *cu, uint16_t opcode) {
    return opcode < MVM_OP_EXT_BASE
        ? MVM_op_get_op(opcode)
        : MVM_ext_resolve_extop_record_in_cu(tc, cu, opcode);
}
