/* This represents the root of the serialization data; everything hangs
 * off this. In read mode, we don't do much besides populate and then
 * read this. In write mode, however, the tables and data chunks will be
 * filled out and grown as needed. */
struct MVMSerializationRoot {
    /* The version of the serialization format. */
    int32_t version;

    /* How many parameterized type intern entries we have */
    int32_t  num_param_interns;

    /* The SC we're serializing/deserializing. */
    MVMSerializationContext *sc;

    /* List of the serialization context objects that we depend on. */
    MVMSerializationContext **dependent_scs;

    /* The number of dependencies, as well as a pointer to the
     * dependencies table. */
    char     *dependencies_table;
    uint32_t num_dependencies;

    /* The number of STables, as well as pointers to the STables
     * table and data chunk. */
    int32_t  num_stables;
    char     *stables_table;
    char     *stables_data;

    /* The number of objects, as well as pointers to the objects
     * table and data chunk. */
    char     *objects_table;
    char     *objects_data;
    int32_t  num_objects;

    /* The number of closures, as we as a pointer to the closures
     * table. */
    int32_t  num_closures;
    char     *closures_table;

    /* The number of contexts (e.g. frames), as well as pointers to
     * the contexts table and data chunk. */
    char     *contexts_table;
    char     *contexts_data;
    uint32_t num_contexts;

    /* The number of repossessions and pointer to repossessions table. */
    int32_t  num_repos;
    char     *repos_table;

    /* The the data segment containing them parameterized type intern entries */
    char     *param_interns_data;

    /* Array of strings making up the string heap we are constructing. If we
     * are reading, this will either contain a string heap array or be NULL
     * and the next field will be set. */
    MVMObject *string_heap;

    /* The compilation unit whose string heap we will use to locate strings.
     * This must be set of string_heap about is set to NULL. */
    MVMCompUnit *string_comp_unit;
};

/* Indexes the deserializer still has to work on. */
struct MVMDeserializeWorklist {
    uint32_t *indexes;
    uint32_t  num_indexes;
    uint32_t  alloc_indexes;
};

/* Represents the serialization reader and the various functions available
 * on it. */
struct MVMSerializationReader {
    /* Serialization root data. */
    MVMSerializationRoot root;

    /* Current offsets for the data chunks (also correspond to the amount of
     * data written in to them). */
    int32_t stables_data_offset;
    int32_t objects_data_offset;
    int32_t contexts_data_offset;
    int32_t param_interns_data_offset;

    /* Limits up to where we can read stables, objects and contexts data. */
    char *stables_data_end;
    char *objects_data_end;
    char *contexts_data_end;
    char *param_interns_data_end;

    /* Where to find details related to the current buffer we're reading from:
     * the buffer pointer itself, the current offset and the amount that is
     * allocated. These are all pointers back into this data structure. */
    char     **cur_read_buffer;
    int32_t  *cur_read_offset;
    char     **cur_read_end;

    /* List of code objects (static first, then all the closures). */
    MVMObject *codes_list;

    /* Number of static code objects. */
    uint32_t num_static_codes;

    /* Whether we're already working on these worklists. */
    atomic_uintptr_t working;

    /* Array of contexts (num_contexts in length). */
    MVMFrame **contexts;

    /* Set of current worklists, for things we need to fully desrialize. When
     * they are all empty, the current (usually lazy) deserialization work is
     * done, and we have the required object graph. */
    MVMDeserializeWorklist wl_objects;
    MVMDeserializeWorklist wl_stables;

    /* The current object we're deserializing. */
    MVMObject *current_object;

    /* The data, which we may want to free when the SC goes away; a flag
     * indicates when it should be. */
    char      *data;
    uint32_t  data_needs_free;
};

/* Represents the serialization writer and the various functions available
 * on it. */
struct MVMSerializationWriter {
    /* Serialization root data. */
    MVMSerializationRoot root;

    /* The code refs we're working through/adding to. */
    MVMObject *codes_list;

    /* Frames that we're to serialize, along with memory management. */
    MVMFrame **contexts_list;
    uint32_t num_contexts;
    uint32_t alloc_contexts;

    /* Current position in the stables, objects and contexts lists. */
    uint64_t stables_list_pos;
    uint64_t objects_list_pos;
    uint64_t contexts_list_pos;

    /* Hash of strings we've already seen while serializing to the index they
     * are placed at in the string heap. */
    MVMObject *seen_strings;

    /* Amount of memory allocated for various things. */
    uint32_t dependencies_table_alloc;
    uint32_t stables_table_alloc;
    uint32_t stables_data_alloc;
    uint32_t objects_table_alloc;
    uint32_t objects_data_alloc;
    uint32_t closures_table_alloc;
    uint32_t contexts_table_alloc;
    uint32_t contexts_data_alloc;
    uint32_t repos_table_alloc;
    uint32_t param_interns_data_alloc;

    /* Current offsets for the data chunks (also correspond to the amount of
     * data written in to them). */
    uint32_t stables_data_offset;
    uint32_t objects_data_offset;
    uint32_t contexts_data_offset;
    uint32_t param_interns_data_offset;

    /* Where to find details related to the current buffer we're writing in
     * to: the buffer pointer itself, the current offset and the amount that
     * is allocated. These are all pointers back into this data structure. */
    char      **cur_write_buffer;
    uint32_t  *cur_write_offset;
    uint32_t  *cur_write_limit;
};

/* Core serialize and deserialize functions. */
void MVM_serialization_deserialize(struct MVMThreadContext *tc, MVMSerializationContext *sc,
    MVMObject *string_heap, MVMObject *codes_static, MVMObject *repo_conflicts,
    MVMString *data);
MVMString * MVM_sha1(struct MVMThreadContext *tc, MVMString *str);
MVMObject * MVM_serialization_serialize(struct MVMThreadContext *tc, MVMSerializationContext *sc,
    MVMObject *empty_string_heap, MVMObject *type);

/* Functions for demanding an object/STable/code be made available (that is,
 * by lazily deserializing it). */
MVMObject * MVM_serialization_demand_object(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
MVMSTable * MVM_serialization_demand_stable(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);
MVMObject * MVM_serialization_demand_code(struct MVMThreadContext *tc, MVMSerializationContext *sc, int64_t idx);

/* Reader/writer functions. */
int64_t MVM_serialization_read_int64(struct MVMThreadContext *tc, MVMSerializationReader *reader);
int64_t MVM_serialization_read_int(struct MVMThreadContext *tc, MVMSerializationReader *reader);
double MVM_serialization_read_num(struct MVMThreadContext *tc, MVMSerializationReader *reader);
MVMString * MVM_serialization_read_str(struct MVMThreadContext *tc, MVMSerializationReader *reader);
void * MVM_serialization_read_ptr(struct MVMThreadContext *tc, MVMSerializationReader *reader, size_t *size);
char * MVM_serialization_read_cstr(struct MVMThreadContext *tc, MVMSerializationReader *reader, size_t *len);
 MVMObject * MVM_serialization_read_ref(struct MVMThreadContext *tc, MVMSerializationReader *reader);
MVMSTable * MVM_serialization_read_stable_ref(struct MVMThreadContext *tc, MVMSerializationReader *reader);
void MVM_serialization_force_stable(struct MVMThreadContext *tc, MVMSerializationReader *reader, MVMSTable *st);
void MVM_serialization_write_int(struct MVMThreadContext *tc, MVMSerializationWriter *writer, int64_t value);
void MVM_serialization_write_num(struct MVMThreadContext *tc, MVMSerializationWriter *writer, double value);
void MVM_serialization_write_str(struct MVMThreadContext *tc, MVMSerializationWriter *writer, MVMString *value);
void MVM_serialization_write_ptr(struct MVMThreadContext *tc, MVMSerializationWriter *writer, const void *ptr, size_t size);
void MVM_serialization_write_cstr(struct MVMThreadContext *tc, MVMSerializationWriter *writer, const char *cstr);
 void MVM_serialization_write_ref(struct MVMThreadContext *tc, MVMSerializationWriter *writer, MVMObject *ref);
void MVM_serialization_write_stable_ref(struct MVMThreadContext *tc, MVMSerializationWriter *writer, MVMSTable *st);
