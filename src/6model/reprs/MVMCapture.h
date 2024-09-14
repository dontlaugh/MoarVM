/* An argument capture carries a callsite and a set of arguments. The capture
 * indicates how to interpret the arguments. There will never be any flattening
 * arguments at this point; those are resolved earlier. */
struct MVMCaptureBody {
    /* The callsite. */
    MVMCallsite *callsite;
    /* Argument buffer. */
    MVMRegister *args;
};
struct MVMCapture {
    MVMObject common;
    MVMCaptureBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMCapture_initialize(struct MVMThreadContext *tc);

/* Operations for creating MVMCapture objects from args and vice versa. */
MVMObject * MVM_capture_from_args(struct MVMThreadContext *tc, MVMArgs args);
MVMArgs MVM_capture_to_args(struct MVMThreadContext *tc, MVMObject *capture);

/* Operations for accessing arguments in MVMCapture objects. */
int64_t MVM_capture_num_pos_args(struct MVMThreadContext *tc, MVMObject *capture);
int64_t MVM_capture_num_args(struct MVMThreadContext *tc, MVMObject *capture);
int64_t MVM_capture_arg_pos_primspec(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
int64_t MVM_capture_arg_primspec(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
void MVM_capture_arg_pos(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx,
        MVMRegister *arg_out, MVMCallsiteFlags *arg_type_out);
void MVM_capture_arg(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx,
        MVMRegister *arg_out, MVMCallsiteFlags *arg_type_out);
MVMObject * MVM_capture_arg_pos_o(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
MVMObject * MVM_capture_arg_o(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
MVMString * MVM_capture_arg_pos_s(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
int64_t MVM_capture_arg_pos_i(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
uint64_t MVM_capture_arg_pos_u(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
double MVM_capture_arg_pos_n(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);
int64_t MVM_capture_has_named_arg(struct MVMThreadContext *tc, MVMObject *capture, MVMString *name);
MVMObject * MVM_capture_get_names_list(struct MVMThreadContext *tc, MVMObject *capture);
MVMObject * MVM_capture_get_nameds(struct MVMThreadContext *tc, MVMObject *capture);
int64_t MVM_capture_has_nameds(struct MVMThreadContext *tc, MVMObject *capture);
void MVM_capture_arg_by_flag_index(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx,
        MVMRegister *arg_out, MVMCallsiteFlags *arg_type_out);
int64_t MVM_capture_is_literal_arg(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx);

/* Operations for deriving a new MVMCapture from an existing one. */
MVMObject * MVM_capture_drop_args(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx, uint32_t count);
MVMObject * MVM_capture_insert_arg(struct MVMThreadContext *tc, MVMObject *capture, uint32_t idx,
        MVMCallsiteFlags kind, MVMRegister value);
MVMObject * MVM_capture_replace_arg(struct MVMThreadContext *tc, MVMObject *capture_obj, uint32_t idx,
        MVMCallsiteEntry kind, MVMRegister value);
