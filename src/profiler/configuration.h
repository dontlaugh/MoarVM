#define MVM_PROGRAM_ENTRYPOINT_PROFILER_STATIC 0
#define MVM_PROGRAM_ENTRYPOINT_PROFILER_DYNAMIC 1
#define MVM_PROGRAM_ENTRYPOINT_SPESH 2
#define MVM_PROGRAM_ENTRYPOINT_JIT 3
#define MVM_PROGRAM_ENTRYPOINT_HEAPSNAPSHOT 4

#define MVM_PROGRAM_ENTRYPOINT_COUNT 5

#define MVM_CONFPROG_SF_RESULT_TO_BE_DETERMINED 0
#define MVM_CONFPROG_SF_RESULT_NEVER 1
#define MVM_CONFPROG_SF_RESULT_DYNAMIC_SUGGEST_NO 2
#define MVM_CONFPROG_SF_RESULT_DYNAMIC_SUGGEST_YES 3
#define MVM_CONFPROG_SF_RESULT_ALWAYS 4

#define MVM_CONFPROG_HEAPSNAPSHOT_RESULT_NOTHING 0
#define MVM_CONFPROG_HEAPSNAPSHOT_RESULT_SNAPSHOT 1
#define MVM_CONFPROG_HEAPSNAPSHOT_RESULT_STATS 2
#define MVM_CONFPROG_HEAPSNAPSHOT_RESULT_SNAPSHOT_WITH_STATS 3

struct MVMConfigurationProgramEntryPoint {
    uint32_t offset;
};

struct MVMConfigurationProgram {
    uint8_t *bytecode;

    MVMObject *string_heap;

    uint8_t *reg_types;
    uint16_t reg_count;

    uint32_t bytecode_length;

    int16_t entrypoints[MVM_PROGRAM_ENTRYPOINT_COUNT];

    AO_t return_counts[16];
    AO_t last_return_time[16];
    AO_t invoke_counts[MVM_PROGRAM_ENTRYPOINT_COUNT];

    uint8_t debugging_level;
};

uint8_t MVM_confprog_has_entrypoint(MVMThreadContext *tc, uint8_t entrypoint);
int64_t MVM_confprog_run(MVMThreadContext *tc, void *subject, uint8_t entrypoint, int64_t initial_feature_value);
void MVM_confprog_mark(MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);
void MVM_confprog_install(MVMThreadContext *tc, MVMObject *bytecode, MVMObject *string_array, MVMObject *entrypoints);
