/* Operations table for a certain type of asynchronous task that can be run on
 * the event loop. */
struct MVMAsyncTaskOps {
    /* How to set work up on the event loop. */
    void (*setup) (struct MVMThreadContext *tc, uv_loop_t *loop, MVMObject *async_task, void *data);

    /* How to grant emit permits, if possible. */
    void (*permit) (struct MVMThreadContext *tc, uv_loop_t *loop, MVMObject *async_task, void *data,
            int64_t channel, int64_t permits);

    /* How to cancel, if possible. */
    void (*cancel) (struct MVMThreadContext *tc, uv_loop_t *loop, MVMObject *async_task, void *data);

    /* How to mark the task's data, if needed. */
    void (*gc_mark) (struct MVMThreadContext *tc, void *data, MVMGCWorklist *worklist);

    /* How to free the task's data, if needed. */
    void (*gc_free) (struct MVMThreadContext *tc, MVMObject *t, void *data);
};

void MVM_io_eventloop_queue_work(struct MVMThreadContext *tc, MVMObject *work);
void MVM_io_eventloop_permit(struct MVMThreadContext *tc, MVMObject *task_obj,
    int64_t channel, int64_t permits);
void MVM_io_eventloop_cancel_work(struct MVMThreadContext *tc, MVMObject *task_obj,
    MVMObject *notify_queue, MVMObject *notify_schedulee);
void MVM_io_eventloop_send_cancellation_notification(struct MVMThreadContext *tc, MVMAsyncTask *task_obj);

int MVM_io_eventloop_add_active_work(struct MVMThreadContext *tc, MVMObject *async_task);
MVMAsyncTask * MVM_io_eventloop_get_active_work(struct MVMThreadContext *tc, int work_idx);
void MVM_io_eventloop_remove_active_work(struct MVMThreadContext *tc, int *work_idx_to_clear);

void MVM_io_eventloop_start(struct MVMThreadContext *tc);
void MVM_io_eventloop_stop(struct MVMThreadContext *tc);
void MVM_io_eventloop_join(struct MVMThreadContext *tc);
void MVM_io_eventloop_destroy(struct MVMThreadContext *tc);
