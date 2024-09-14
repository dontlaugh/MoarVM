#include "moar.h"
/* Stub file to mark our lack of support for this architecture. We
   should probably stub dasm, too, rather than include x86 */

int32_t MVM_jit_support() {
    return 0;
}

MVMJitGraph* MVM_jit_try_make_graph(struct MVMThreadContext *tc, MVMSpeshGraph *sg) {
    return NULL;
}
MVMJitCode * MVM_jit_compile_graph(struct MVMThreadContext *tc, MVMJitGraph *jg) {
    return NULL;
}

MVMJitCode* MVM_jit_code_copy(struct MVMThreadContext *tc, MVMJitCode * const code) {
    return NULL;
}

void MVM_jit_graph_destroy(struct MVMThreadContext *tc, MVMJitGraph *graph) {
    return;
}

void MVM_jit_code_destroy(struct MVMThreadContext *tc, MVMJitCode *code) {
    return;
}

void MVM_jit_code_enter(struct MVMThreadContext *tc, MVMJitCode *code, MVMCompUnit *cu) {
    return;
}

uint32_t MVM_jit_code_get_active_handlers(struct MVMThreadContext *tc, MVMJitCode *code, void *current_position, uint32_t i) {
    return 0;
}

uint32_t MVM_jit_code_get_active_inlines(struct MVMThreadContext *tc, MVMJitCode *code, void *current_position, uint32_t i) {
    return 0;
}

uint32_t MVM_jit_code_get_active_deopt_idx(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame) {
    return 0;
}

void MVM_jit_code_set_current_position(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame, void *position) {
}

void * MVM_jit_code_get_current_position(struct MVMThreadContext *tc, MVMJitCode *code, MVMFrame *frame) {
    return NULL;
}

void MVM_jit_code_trampoline(struct MVMThreadContext *tc) {}
