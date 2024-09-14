#define MVM_BC_ILLEGAL_OFFSET ((uint32_t)-1)

enum {
    MVM_BC_branch_target = 1 << 0,
    MVM_BC_op_boundary   = 1 << 1,
};

void MVM_validate_static_frame(struct MVMThreadContext *tc, MVMStaticFrame *static_frame);
