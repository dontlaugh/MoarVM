#pragma once

/* Exception handler actions. */
#define MVM_EX_ACTION_GOTO                0
#define MVM_EX_ACTION_GOTO_WITH_PAYLOAD   1
#define MVM_EX_ACTION_INVOKE              2

/* Exception categories. */
#define MVM_EX_CAT_CATCH         1
#define MVM_EX_CAT_CONTROL       2
#define MVM_EX_CAT_NEXT          4
#define MVM_EX_CAT_REDO          8
#define MVM_EX_CAT_LAST          16
#define MVM_EX_CAT_RETURN        32
#define MVM_EX_CAT_UNWIND        64
#define MVM_EX_CAT_TAKE          128
#define MVM_EX_CAT_WARN          256
#define MVM_EX_CAT_SUCCEED       512
#define MVM_EX_CAT_PROCEED       1024
#define MVM_EX_CAT_LABELED       4096
#define MVM_EX_CAT_AWAIT         8192
#define MVM_EX_CAT_EMIT          16384
#define MVM_EX_CAT_DONE          32768

/* Not a real category, but marks an inline boundary. */
#define MVM_EX_INLINE_BOUNDARY   2147483648

/* Ways to throw an exception. */
#define MVM_EX_THROW_DYN         0
#define MVM_EX_THROW_LEX         1
#define MVM_EX_THROW_LEXOTIC     2
#define MVM_EX_THROW_LEX_CALLER  3

/* Information associated with an exception handler. */
struct MVMFrameHandler {
    /* Start offset into the frame's bytecode for the handler, inclusive. */
    uint32_t start_offset;

    /* End offset into the frame's bytecode for the handler, exclusive. */
    uint32_t end_offset;

    /* Category mask or inline boundary indicator. */
    uint32_t category_mask;

    /* The kind of handler it is. */
    uint16_t action;

    /* Register containing block to invoke, for invokey handlers. */
    uint16_t block_reg;

    /* Offset into the frame's bytecode of the handler, for goto handlers. */
    uint32_t goto_offset;

    /* Register containing a label in case we have a labeled loop. We need to
     * be able to check for its identity when handling e.g. `next LABEL`. */
    uint16_t label_reg;

    /* The inlinee that this handler is associated with. Set to -1 for the
     * top-level handlers of a frame. Used both to skip non-top-level
     * handlers, but also to indicate, for a inline boundary indicator
     * entry in the table, the inline whose handlers end at this point. */
    int16_t inlinee;
};

/* An active (currently executing) exception handler. */
struct MVMActiveHandler {
    /* The frame the handler was found in. */
    struct MVMFrame *frame;

    /* The handler information itself. */
    struct MVMFrameHandler *handler;

    /* Handler information for a JITted handler */
    struct MVMJitHandler *jit_handler;

    /* The exception object. */
    struct MVMObject *ex_obj;

    /* The next active handler in the chain. */
    struct MVMActiveHandler *next_handler;
};

/* Exception related functions. */
struct MVMObject * MVM_exception_backtrace(struct MVMThreadContext *tc, struct MVMObject *ex_obj);
struct MVMObject * MVM_exception_backtrace_strings(struct MVMThreadContext *tc, struct MVMObject *exObj);
void MVM_dump_backtrace(struct MVMThreadContext *tc);
void MVM_exception_throwcat(struct MVMThreadContext *tc, uint8_t mode, uint32_t cat, MVMRegister *resume_result);
void MVM_exception_die(struct MVMThreadContext *tc, MVMString *str, MVMRegister *rr);
void MVM_exception_throwobj(struct MVMThreadContext *tc, uint8_t mode,struct MVMObject *exObj, MVMRegister *resume_result);
void MVM_exception_throwpayload(struct MVMThreadContext *tc, uint8_t mode, uint32_t cat,struct MVMObject *payload, MVMRegister *resume_result);
void MVM_exception_resume(struct MVMThreadContext *tc,struct MVMObject *exObj);
  void MVM_panic_allocation_failed(size_t len) ;
  void MVM_panic(int32_t exitCode, const char *messageFormat, ...);
  void MVM_oops(struct MVMThreadContext *tc, const char *messageFormat, ...);
  void MVM_exception_throw_adhoc(struct MVMThreadContext *tc, const char *messageFormat, ...);
 void MVM_exception_throw_adhoc_va(struct MVMThreadContext *tc, const char *messageFormat, va_list args) ;
  void MVM_exception_throw_adhoc_free(struct MVMThreadContext *tc, char **waste, const char *messageFormat, ...);
 void MVM_exception_throw_adhoc_free_va(struct MVMThreadContext *tc, char **waste, const char *messageFormat, va_list args);
 void MVM_crash_on_error(void);
char * MVM_exception_backtrace_line(struct MVMThreadContext *tc, MVMFrame *cur_frame, uint16_t not_top, uint8_t *throw_address);
int32_t MVM_get_exception_category(struct MVMThreadContext *tc,struct MVMObject *ex);
void MVM_bind_exception_category(struct MVMThreadContext *tc,struct MVMObject *ex, int32_t category);
struct MVMObject * MVM_get_exception_payload(struct MVMThreadContext *tc,struct MVMObject *ex);
void MVM_bind_exception_payload(struct MVMThreadContext *tc,struct MVMObject *ex,struct MVMObject *payload);
struct MVMString * MVM_get_exception_message(struct MVMThreadContext *tc,struct MVMObject *ex);
void MVM_bind_exception_message(struct MVMThreadContext *tc,struct MVMObject *ex, MVMString *message);
void MVM_exception_returnafterunwind(struct MVMThreadContext *tc,struct MVMObject *ex);

/* Exit codes for panic. */
#define MVM_exitcode_NYI            12
#define MVM_exitcode_compunit       13
#define MVM_exitcode_invalidopcode  14
#define MVM_exitcode_gcalloc        15
#define MVM_exitcode_gcroots        16
#define MVM_exitcode_gcnursery      17
#define MVM_exitcode_threads        18
#define MVM_exitcode_gcorch         19
