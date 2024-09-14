/* Representation for code in the VM. Holds an MVMStaticFrame along
 * with an optional outer pointer if this is a closure. */
struct MVMCodeBody {
    MVMStaticFrame *sf;
    MVMFrame       *outer;
    MVMObject      *code_object;
    MVMString      *name;
    MVMRegister    *state_vars;
    uint16_t       is_static;
    uint16_t       is_compiler_stub;
};
struct MVMCode {
    MVMObject common;
    MVMCodeBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMCode_initialize(struct MVMThreadContext *tc);

 MVMObject * MVM_code_location(struct MVMThreadContext *tc, MVMObject *code);
void MVM_code_location_out(struct MVMThreadContext *tc, MVMObject *code,
                           MVMString **file_out, int32_t *line_out);
