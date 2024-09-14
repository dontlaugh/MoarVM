#include "moar.h"
#include <stdarg.h>

void MVM_spesh_debug_printf(struct MVMThreadContext *tc, const char *format, ...) {
    va_list list;
    va_start(list, format);
    vfprintf(tc->instance->spesh_log_fh, format, list);
    va_end(list);
}

size_t MVM_spesh_debug_tell(struct MVMThreadContext *tc) {
    return ftell(tc->instance->spesh_log_fh);
}

void MVM_spesh_debug_flush(struct MVMThreadContext *tc) {
    fflush(tc->instance->spesh_log_fh);
}
