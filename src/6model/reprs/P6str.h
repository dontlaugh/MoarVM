/* Representation used by P6 native strings. */
struct MVMP6strBody {
    MVMString *value;
};
struct MVMP6str {
    MVMObject common;
    MVMP6strBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMP6str_initialize(struct MVMThreadContext *tc);

void MVMP6str_set_str(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMString *value);
MVMString * MVMP6str_get_str(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
