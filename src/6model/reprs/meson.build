

sixmodel_reprs_srcs = [
'CArray.c',
'ConcBlockingQueue.c',
'ConditionVariable.c',
'CPointer.c',
'CPPStruct.c',
'CStr.c',
'CStruct.c',
'CUnion.c',
'Decoder.c',
'HashAttrStore.c',
'KnowHOWAttributeREPR.c',
'KnowHOWREPR.c',
'MultiDimArray.c',
'MVMAsyncTask.c',
'MVMCapture.c',
'MVMCFunction.c',
'MVMCode.c',
'MVMCompUnit.c',
'MVMContext.c',
'MVMContinuation.c',
'MVMDLLSym.c',
'MVMException.c',
'MVMHash.c',
'MVMIter.c',
'MVMNull.c',
'MVMOSHandle.c',
'MVMSpeshCandidate.c',
'MVMSpeshLog.c',
'MVMStat.c',
'MVMStaticFrame.c',
'MVMStaticFrameSpesh.c',
'MVMString.c',
'MVMThread.c',
'MVMTracked.c',
'NativeCall.c',
'NativeRef.c',
'NFA.c',
'P6bigint.c',
'P6int.c',
'P6num.c',
'P6opaque.c',
'P6str.c',
'ReentrantMutex.c',
'SCRef.c',
'Semaphore.c',
'Uninstantiable.c',
'VMArray.c',
]

sixmodel_reprs_deps = [
    dynload_s_lib,
    mimalloc_dep,
    libuv_dep,
    libtommath_lib,
]

sixmodel_reprs_lib = static_library('sixmodel', sixmodel_reprs_srcs,
                                    dependencies : sixmodel_reprs_deps,
                                    include_directories : [config_h_inc, dynload_inc_dir])

sixmodel_reprs_dep = declare_dependency(link_with : sixmodel_reprs_lib)

