/* Container specification information, for types that serve as containers.
 * A container is something that can be assigned into. It may be some kind
 * of container object (like Perl 6's Scalar) or it may be a reference to a
 * native lexical or object field. The function table determines the way it
 * behaves. */
struct MVMContainerSpec {
    /* Name of this container specification. */
    char *name;

    /* Fetches a value out of a container. Used for decontainerization. */
    void (*fetch) (struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);

    /* Native value fetches. */
    void (*fetch_i) (struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
    void (*fetch_u) (struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
    void (*fetch_n) (struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
    void (*fetch_s) (struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);

    /* Stores a value in a container. Used for assignment. */
    void (*store) (struct MVMThreadContext *tc, MVMObject *cont, MVMObject *obj);

    /* Native container stores. */
    void (*store_i) (struct MVMThreadContext *tc, MVMObject *cont, int64_t value);
    void (*store_u) (struct MVMThreadContext *tc, MVMObject *cont, uint64_t value);
    void (*store_n) (struct MVMThreadContext *tc, MVMObject *cont, double value);
    void (*store_s) (struct MVMThreadContext *tc, MVMObject *cont, MVMString *value);

    /* Stores a value in a container, without any checking of it (this
     * assumes an optimizer or something else already did it). Used for
     * assignment. */
    void (*store_unchecked) (struct MVMThreadContext *tc, MVMObject *cont, MVMObject *obj);

    /* Allow the Container Spec to emit better bytecode, for example for
     * a decont operation. */
    void (*spesh) (struct MVMThreadContext *tc, MVMSTable *st, MVMSpeshGraph *g, MVMSpeshBB *bb, MVMSpeshIns *ins);

    /* Marks container data, if any. */
    void (*gc_mark_data) (struct MVMThreadContext *tc, MVMSTable *st, MVMGCWorklist *worklist);

    /* Frees container data, if any. */
    void (*gc_free_data) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Serializes the container data, if any. */
    void (*serialize) (struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationWriter *writer);

    /* Deserializes the container data, if any. */
    void (*deserialize) (struct MVMThreadContext *tc, MVMSTable *st, MVMSerializationReader *reader);

    /* Called when the whole of the REPR data has been deserialized (so the
     * offsets of attributes could be queried, for example). */
    void (*post_deserialize) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Returns a non-zero value if we can store to the container. */
    int32_t (*can_store) (struct MVMThreadContext *tc, MVMObject *cont);

    /* If available, reference atomic compare and swap operation, atomic load
     * operation, and atomic store operation. */
    void (*cas) (struct MVMThreadContext *tc, MVMObject *cont, MVMObject *expected,
        MVMObject *value, MVMRegister *result);
    MVMObject * (*load_atomic) (struct MVMThreadContext *tc, MVMObject *cont);
    void (*store_atomic) (struct MVMThreadContext *tc, MVMObject *cont, MVMObject *value);

    /* Set this to a non-zero value if a fetch promises to never invoke any
     * code. This means the VM knows it can safely decontainerize in places
     * it would not be safe or practical to return to the interpreter. */
    uint8_t fetch_never_invokes;
};

/* A container configurer knows how to attach a certain type of container
 * to an STable and configure it. */
struct MVMContainerConfigurer {
    MVMString *name;
    /* Sets this container spec in place for the specified STable. */
    void (*set_container_spec) (struct MVMThreadContext *tc, MVMSTable *st);

    /* Configures the container spec with the specified info. */
    void (*configure_container_spec) (struct MVMThreadContext *tc, MVMSTable *st, MVMObject *config);
};

const MVMContainerConfigurer * MVM_6model_get_container_config(struct MVMThreadContext *tc, MVMString *name);
void MVM_6model_containers_setup(struct MVMThreadContext *tc);
int64_t MVM_6model_container_iscont_rw(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_iscont_i(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_iscont_u(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_iscont_n(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_iscont_s(struct MVMThreadContext *tc, MVMObject *cont);
void MVM_6model_container_decont_i(struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
void MVM_6model_container_decont_n(struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
void MVM_6model_container_decont_s(struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
void MVM_6model_container_decont_u(struct MVMThreadContext *tc, MVMObject *cont, MVMRegister *res);
void MVM_6model_container_assign_i(struct MVMThreadContext *tc, MVMObject *cont, int64_t value);
void MVM_6model_container_assign_u(struct MVMThreadContext *tc, MVMObject *cont, uint64_t value);
void MVM_6model_container_assign_n(struct MVMThreadContext *tc, MVMObject *cont, double value);
void MVM_6model_container_assign_s(struct MVMThreadContext *tc, MVMObject *cont, MVMString *value);
void MVM_6model_container_cas(struct MVMThreadContext *tc, MVMObject *cont,
    MVMObject *expected, MVMObject *value, MVMRegister *result);
MVMObject * MVM_6model_container_atomic_load(struct MVMThreadContext *tc, MVMObject *cont);
void MVM_6model_container_atomic_store(struct MVMThreadContext *tc, MVMObject *cont, MVMObject *value);
int64_t MVM_6model_container_cas_i(struct MVMThreadContext *tc, MVMObject *cont,
    int64_t expected, int64_t value);
int64_t MVM_6model_container_atomic_load_i(struct MVMThreadContext *tc, MVMObject *cont);
void MVM_6model_container_atomic_store_i(struct MVMThreadContext *tc, MVMObject *cont, int64_t value);
int64_t MVM_6model_container_atomic_inc(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_atomic_dec(struct MVMThreadContext *tc, MVMObject *cont);
int64_t MVM_6model_container_atomic_add(struct MVMThreadContext *tc, MVMObject *cont, int64_t value);

void *MVM_container_devirtualize_fetch_for_jit(struct MVMThreadContext *tc, MVMSTable *st, uint16_t type);
void *MVM_container_devirtualize_store_for_jit(struct MVMThreadContext *tc, MVMSTable *st, uint16_t type);
