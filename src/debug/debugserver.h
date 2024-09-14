/* Debugserver requests:
 *
 * Empty
 * -----
 * 
 * When the kind of request is set to empty, the debugserver is
 * ready to initiate a new request from the client.
 * 
 * Invoke
 * ------
 * 
 * The Debugserver requests that a thread invokes a given
 * code object. The debugserver sets up as much as it can
 * on its own, then notifies the thread to wake up and do
 * the rest.
 * 
 * The thread will hold the ID of the request in Special Return Data,
 * so that returning from the code or unwinding with an exception or
 * taking a continuation can be handled properly.
 * 
 * Since we want the thread to function normally with respect to break
 * points and stepping, the thread will behave essentially as if
 * fully resumed.
 * 
 */

typedef enum {
    MVM_DebugRequest_empty,
    MVM_DebugRequest_invoke,
} MVMDebugServerRequestKind;

typedef enum {
    MVM_DebugRequestStatus_sender_is_waiting,
    MVM_DebugRequestStatus_receiver_acknowledged,
    MVM_DebugRequestStatus_receiver_finished,
    MVM_DebugRequestStatus_sender_acknowledged
} MVMDebugServerRequestStatus;

struct MVMDebugServerHandleTableEntry {
    uint64_t id;
    MVMObject *target;
};

struct MVMDebugServerHandleTable {
    uint32_t allocated;
    uint32_t used;

    uint64_t next_id;

    MVMDebugServerHandleTableEntry *entries;
};

struct MVMDebugServerBreakpointInfo {
    uint64_t breakpoint_id;
    uint32_t line_no;

    uint8_t shall_suspend;
    uint8_t send_backtrace;
};

struct MVMDebugServerBreakpointFileTable {
    char *filename;
    uint32_t filename_length;
    uint32_t lines_active_alloc;

    uint8_t *lines_active;

    MVMDebugServerBreakpointInfo *breakpoints;
    uint32_t breakpoints_alloc;
    uint32_t breakpoints_used;
};

struct MVMDebugServerBreakpointTable {
    MVMDebugServerBreakpointFileTable *files;
    uint32_t files_used;
    uint32_t files_alloc;
};

/* This struct holds all data used for communication between
 * the Debugserver and a thread.
 * 
 * * Invoke a code object
 */
struct MVMDebugServerRequestData {
    MVMDebugServerRequestKind kind;

    /* The ID of the request taken from the network packet,
     * to be used for responses to the client. */
    uint64_t request_id;

    MVMThreadContext *target_tc;

    atomic_uintptr_t status;

    union
    {
        struct {
            MVMCode *target;
            MVMArgs *args;
        } invoke;
        MVMObject *o;
    } data;
};

struct MVMDebugServerData {
    /* Debug Server thread */
    uv_thread_t thread;

    /* Protect the debugserver-related condvars */
    uv_mutex_t mutex_cond;

    /* Protect sending data on the network */
    uv_mutex_t mutex_network_send;

    /* Protect the open requests list */
    uv_mutex_t mutex_request_list;

    /* Condition variable to tell threads to check their state for changes
     * like "i should suspend" */
    uv_cond_t tell_threads;

    /* Condition variable to tell the worker to check thread states
     * for changes like "i just suspended" */
    uv_cond_t tell_worker;

    MVMDebugServerRequestData request_data;

    uint32_t port;
    uint32_t thread_id;

    uint64_t event_id;

    MVMDebugServerHandleTable *handle_table;

    MVMDebugServerBreakpointTable *breakpoints;
    uint32_t any_breakpoints_at_all;
    uint32_t breakpoints_alloc;
    uint32_t breakpoints_used;
    uv_mutex_t mutex_breakpoints;

    void *messagepack_data;

    uint8_t debugspam_network;
    uint8_t debugspam_protocol;
};

MVM_PUBLIC void MVM_debugserver_init(MVMThreadContext *tc, uint32_t port);
MVM_PUBLIC void MVM_debugserver_mark_handles(MVMThreadContext *tc, MVMGCWorklist *worklist, MVMHeapSnapshotState *snapshot);

MVM_PUBLIC void MVM_debugserver_notify_thread_creation(MVMThreadContext *tc);
MVM_PUBLIC void MVM_debugserver_notify_thread_destruction(MVMThreadContext *tc);

MVM_PUBLIC void MVM_debugserver_notify_unhandled_exception(MVMThreadContext *tc, MVMException *ex);

MVM_PUBLIC void MVM_debugserver_register_line(MVMThreadContext *tc, char *filename, uint32_t filename_len, uint32_t line_no,  uint32_t *file_idx);
MVM_PUBLIC int32_t MVM_debugserver_breakpoint_check(MVMThreadContext *tc, uint32_t file_idx, uint32_t line_no);
