/* Pull in all of the core REPRs. */
#include "6model/reprs/MVMString.h"
#include "6model/reprs/VMArray.h"
#include "6model/reprs/MVMHash.h"
#include "6model/reprs/MVMCFunction.h"
#include "6model/reprs/KnowHOWREPR.h"
#include "6model/reprs/P6opaque.h"
#include "6model/reprs/MVMCode.h"
#include "6model/reprs/MVMOSHandle.h"
#include "6model/reprs/P6int.h"
#include "6model/reprs/P6num.h"
#include "6model/reprs/Uninstantiable.h"
#include "6model/reprs/HashAttrStore.h"
#include "6model/reprs/KnowHOWAttributeREPR.h"
#include "6model/reprs/P6str.h"
#include "6model/reprs/MVMThread.h"
#include "6model/reprs/MVMIter.h"
#include "6model/reprs/MVMContext.h"
#include "6model/reprs/SCRef.h"
#include "6model/reprs/MVMCapture.h"
#include "6model/reprs/P6bigint.h"
#include "6model/reprs/NFA.h"
#include "6model/reprs/MVMException.h"
#include "6model/reprs/MVMStaticFrame.h"
#include "6model/reprs/MVMCompUnit.h"
#include "6model/reprs/MVMDLLSym.h"
#include "6model/reprs/MVMContinuation.h"
#include "6model/reprs/NativeCall.h"
#include "6model/reprs/CPointer.h"
#include "6model/reprs/CStr.h"
#include "6model/reprs/CArray.h"
#include "6model/reprs/CStruct.h"
#include "6model/reprs/CUnion.h"
#include "6model/reprs/ReentrantMutex.h"
#include "6model/reprs/ConditionVariable.h"
#include "6model/reprs/Semaphore.h"
#include "6model/reprs/ConcBlockingQueue.h"
#include "6model/reprs/MVMAsyncTask.h"
#include "6model/reprs/MVMNull.h"
#include "6model/reprs/CPPStruct.h"
#include "6model/reprs/NativeRef.h"
#include "6model/reprs/MultiDimArray.h"
#include "6model/reprs/Decoder.h"
#include "6model/reprs/MVMSpeshLog.h"
#include "6model/reprs/MVMStaticFrameSpesh.h"
#include "6model/reprs/MVMSpeshCandidate.h"
#include "6model/reprs/MVMTracked.h"
#include "6model/reprs/MVMStat.h"

/* REPR related functions. */
void MVM_repr_initialize_registry(struct MVMThreadContext *tc);
uint32_t MVM_repr_name_to_id(struct MVMThreadContext *tc, MVMString *name);
const MVMREPROps * MVM_repr_get_by_id(struct MVMThreadContext *tc, uint32_t id);
const MVMREPROps * MVM_repr_get_by_name(struct MVMThreadContext *tc, MVMString *name);

/* Core representation IDs (determined by the order we add them
 * to the registery in reprs.c). */
#define MVM_REPR_ID_MVMString               0
#define MVM_REPR_ID_VMArray                 1
#define MVM_REPR_ID_MVMHash                 2
#define MVM_REPR_ID_MVMCFunction            3
#define MVM_REPR_ID_KnowHOWREPR             4
#define MVM_REPR_ID_P6opaque                5
#define MVM_REPR_ID_MVMCode                 6
#define MVM_REPR_ID_MVMOSHandle             7
#define MVM_REPR_ID_P6int                   8
#define MVM_REPR_ID_P6num                   9
#define MVM_REPR_ID_Uninstantiable          10
#define MVM_REPR_ID_HashAttrStore           11
#define MVM_REPR_ID_KnowHOWAttributeREPR    12
#define MVM_REPR_ID_P6str                   13
#define MVM_REPR_ID_MVMThread               14
#define MVM_REPR_ID_MVMIter                 15
#define MVM_REPR_ID_MVMContext              16
#define MVM_REPR_ID_SCRef                   17
#define MVM_REPR_ID_MVMSpeshLog             18
#define MVM_REPR_ID_P6bigint                19
#define MVM_REPR_ID_NFA                     20
#define MVM_REPR_ID_MVMException            21
#define MVM_REPR_ID_MVMStaticFrame          22
#define MVM_REPR_ID_MVMCompUnit             23
#define MVM_REPR_ID_MVMDLLSym               24
#define MVM_REPR_ID_MVMContinuation         25
#define MVM_REPR_ID_MVMNativeCall           26
#define MVM_REPR_ID_MVMCPointer             27
#define MVM_REPR_ID_MVMCStr                 28
#define MVM_REPR_ID_MVMCArray               29
#define MVM_REPR_ID_MVMCStruct              30
#define MVM_REPR_ID_ReentrantMutex          31
#define MVM_REPR_ID_ConditionVariable       32
#define MVM_REPR_ID_Semaphore               33
#define MVM_REPR_ID_ConcBlockingQueue       34
#define MVM_REPR_ID_MVMAsyncTask            35
#define MVM_REPR_ID_MVMNull                 36
#define MVM_REPR_ID_NativeRef               37
#define MVM_REPR_ID_MVMCUnion               38
#define MVM_REPR_ID_MultiDimArray           39
#define MVM_REPR_ID_MVMCPPStruct            40
#define MVM_REPR_ID_Decoder                 41
#define MVM_REPR_ID_MVMStaticFrameSpesh     42
#define MVM_REPR_ID_MVMSpeshCandidate       43
#define MVM_REPR_ID_MVMCapture              44
#define MVM_REPR_ID_MVMTracked              45
#define MVM_REPR_ID_MVMStat                 46

#define MVM_REPR_CORE_COUNT                 47
#define MVM_REPR_MAX_COUNT                  64

/* Default attribute functions for a REPR that lacks them. */
#define MVM_REPR_DEFAULT_ATTR_FUNCS \
{ \
    MVM_REPR_DEFAULT_GET_ATTRIBUTE, \
    MVM_REPR_DEFAULT_BIND_ATTRIBUTE, \
    MVM_REPR_DEFAULT_HINT_FOR, \
    MVM_REPR_DEFAULT_IS_ATTRIBUTE_INITIALIZED, \
    MVM_REPR_DEFAULT_ATTRIBUTE_AS_ATOMIC \
}

/* Default boxing functions for a REPR that lacks them. */
#define MVM_REPR_DEFAULT_BOX_FUNCS \
{ \
    MVM_REPR_DEFAULT_SET_INT, \
    MVM_REPR_DEFAULT_GET_INT, \
    MVM_REPR_DEFAULT_SET_NUM, \
    MVM_REPR_DEFAULT_GET_NUM, \
    MVM_REPR_DEFAULT_SET_STR, \
    MVM_REPR_DEFAULT_GET_STR, \
    MVM_REPR_DEFAULT_SET_UINT, \
    MVM_REPR_DEFAULT_GET_UINT, \
    MVM_REPR_DEFAULT_GET_BOXED_REF \
}

/* Default positional functions for a REPR that lacks them. */
#define MVM_REPR_DEFAULT_POS_FUNCS \
{ \
    MVM_REPR_DEFAULT_AT_POS, \
    MVM_REPR_DEFAULT_BIND_POS, \
    MVM_REPR_DEFAULT_SET_ELEMS, \
    MVM_REPR_DEFAULT_PUSH, \
    MVM_REPR_DEFAULT_POP, \
    MVM_REPR_DEFAULT_UNSHIFT, \
    MVM_REPR_DEFAULT_SHIFT, \
    MVM_REPR_DEFAULT_SLICE, \
    MVM_REPR_DEFAULT_SPLICE, \
    MVM_REPR_DEFAULT_AT_POS_MULTIDIM, \
    MVM_REPR_DEFAULT_BIND_POS_MULTIDIM, \
    MVM_REPR_DEFAULT_DIMENSIONS, \
    MVM_REPR_DEFAULT_SET_DIMENSIONS, \
    MVM_REPR_DEFAULT_GET_ELEM_STORAGE_SPEC, \
    MVM_REPR_DEFAULT_POS_AS_ATOMIC, \
    MVM_REPR_DEFAULT_POS_AS_ATOMIC_MULTIDIM, \
    MVM_REPR_DEFAULT_POS_WRITE_BUF, \
    MVM_REPR_DEFAULT_POS_READ_BUF \
}

/* Default associative functions for a REPR that lacks them. */
#define MVM_REPR_DEFAULT_ASS_FUNCS \
{ \
    MVM_REPR_DEFAULT_AT_KEY, \
    MVM_REPR_DEFAULT_BIND_KEY, \
    MVM_REPR_DEFAULT_EXISTS_KEY, \
    MVM_REPR_DEFAULT_DELETE_KEY, \
    MVM_REPR_DEFAULT_GET_VALUE_STORAGE_SPEC \
}

/* Register a representation at runtime, setting repr->ID to a dynamically
 * assigned value.
 *
 * Returns nonzero if the representation has been added successfully and
 * zero if a representation with the same name is already present.
 * In that case, the MVMREPROps structure is unused and may be deallocated.
 */
int MVM_repr_register_dynamic_repr(struct MVMThreadContext *tc, MVMREPROps *repr);

/* Default elems REPR function for a REPR that lacks it. */
uint64_t MVM_REPR_DEFAULT_ELEMS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);

/* Default attribute access REPR function for a REPR that lacks it. */
void MVM_REPR_DEFAULT_GET_ATTRIBUTE(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *class_handle, MVMString *name, int64_t hint, MVMRegister *result, uint16_t kind);
void MVM_REPR_DEFAULT_BIND_ATTRIBUTE(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *class_handle, MVMString *name, int64_t hint, MVMRegister value, uint16_t kind);
int64_t MVM_REPR_DEFAULT_IS_ATTRIBUTE_INITIALIZED(struct MVMThreadContext *tc, MVMSTable *st, void *data, MVMObject *class_handle, MVMString *name, int64_t hint);
int64_t MVM_REPR_DEFAULT_HINT_FOR(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *class_handle, MVMString *name);
atomic_uintptr_t * MVM_REPR_DEFAULT_ATTRIBUTE_AS_ATOMIC(struct MVMThreadContext *tc, MVMSTable *st, void *data, MVMObject *class_handle, MVMString *name, uint16_t kind);

/* Default boxing REPR function for a REPR that lacks it. */
void MVM_REPR_DEFAULT_SET_INT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t value);
int64_t MVM_REPR_DEFAULT_GET_INT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
void MVM_REPR_DEFAULT_SET_NUM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, double value);
double MVM_REPR_DEFAULT_GET_NUM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
void MVM_REPR_DEFAULT_SET_STR(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMString *value);
MVMString * MVM_REPR_DEFAULT_GET_STR(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
void MVM_REPR_DEFAULT_SET_UINT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, uint64_t value);
uint64_t MVM_REPR_DEFAULT_GET_UINT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data);
void * MVM_REPR_DEFAULT_GET_BOXED_REF(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, uint32_t repr_id);

/* Default positional indexing REPR function for a REPR that lacks it. */
void MVM_REPR_DEFAULT_AT_POS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t index, MVMRegister *value, uint16_t kind);
void MVM_REPR_DEFAULT_BIND_POS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t index, MVMRegister value, uint16_t kind);
void MVM_REPR_DEFAULT_SET_ELEMS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, uint64_t count);
void MVM_REPR_DEFAULT_PUSH(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister value, uint16_t kind);
void MVM_REPR_DEFAULT_POP(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister *value, uint16_t kind);
void MVM_REPR_DEFAULT_UNSHIFT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister value, uint16_t kind);
void MVM_REPR_DEFAULT_SHIFT(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMRegister *value, uint16_t kind);
void MVM_REPR_DEFAULT_SLICE(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *src, void *data, MVMObject *dest, int64_t start, int64_t end);
void MVM_REPR_DEFAULT_AT_POS_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_indices, int64_t *indices, MVMRegister *value, uint16_t kind);
void MVM_REPR_DEFAULT_BIND_POS_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_indices, int64_t *indices, MVMRegister value, uint16_t kind);
void MVM_REPR_DEFAULT_DIMENSIONS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t *num_dimensions, int64_t **dimensions);
void MVM_REPR_DEFAULT_SET_DIMENSIONS(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_dimensions, int64_t *dimensions);
void MVM_REPR_DEFAULT_POS_WRITE_BUF(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, char *from, int64_t offset, uint64_t elems);
int64_t MVM_REPR_DEFAULT_POS_READ_BUF(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t offset, uint64_t elems);
void MVM_REPR_DEFAULT_AT_POS_MULTIDIM_NO_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_indices, int64_t *indices, MVMRegister *value, uint16_t kind);
void MVM_REPR_DEFAULT_BIND_POS_MULTIDIM_NO_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_indices, int64_t *indices, MVMRegister value, uint16_t kind);
void MVM_REPR_DEFAULT_DIMENSIONS_NO_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t *num_dimensions, int64_t **dimensions);
void MVM_REPR_DEFAULT_SET_DIMENSIONS_NO_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, int64_t num_dimensions, int64_t *dimensions);
MVMStorageSpec MVM_REPR_DEFAULT_GET_ELEM_STORAGE_SPEC(struct MVMThreadContext *tc, MVMSTable *st);
atomic_uintptr_t * MVM_REPR_DEFAULT_POS_AS_ATOMIC(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root,
    void *data, int64_t index);
atomic_uintptr_t * MVM_REPR_DEFAULT_POS_AS_ATOMIC_MULTIDIM(struct MVMThreadContext *tc, MVMSTable *st,
    MVMObject *root, void *data, int64_t num_indices, int64_t *indices);

/* Default associative indexing REPR function for a REPR that lacks it. */
void MVM_REPR_DEFAULT_SPLICE(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *target_array, int64_t offset, uint64_t elems);
void MVM_REPR_DEFAULT_AT_KEY(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key, MVMRegister *result, uint16_t kind);
void MVM_REPR_DEFAULT_BIND_KEY(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key, MVMRegister value, uint16_t kind);
int64_t MVM_REPR_DEFAULT_EXISTS_KEY(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key);
void MVM_REPR_DEFAULT_DELETE_KEY(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *root, void *data, MVMObject *key);
MVMStorageSpec MVM_REPR_DEFAULT_GET_VALUE_STORAGE_SPEC(struct MVMThreadContext *tc, MVMSTable *st);

static inline int32_t MVM_code_iscode(struct MVMThreadContext *tc, MVMObject *code) {
    return REPR(code)->ID == MVM_REPR_ID_MVMCode && IS_CONCRETE(code);
}
