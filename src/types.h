/* struct and union types are forward-declared for convenience */

typedef struct MVMActiveHandler MVMActiveHandler;
typedef struct MVMArgInfo MVMArgInfo;
typedef struct MVMArgProcContext MVMArgProcContext;
typedef struct MVMArgs MVMArgs;
typedef struct MVMArray MVMArray;
typedef struct MVMArrayBody MVMArrayBody;
typedef struct MVMArrayREPRData MVMArrayREPRData;
typedef struct MVMAsyncTask MVMAsyncTask;
typedef struct MVMAsyncTaskBody MVMAsyncTaskBody;
typedef struct MVMAsyncTaskOps MVMAsyncTaskOps;
typedef struct MVMAttributeIdentifier MVMAttributeIdentifier;
typedef struct MVMBoolificationSpec MVMBoolificationSpec;
typedef struct MVMBootTypes MVMBootTypes;
typedef struct MVMEventSubscriptions MVMEventSubscriptions;
typedef struct MVMBytecodeAnnotation MVMBytecodeAnnotation;
typedef struct MVMCallCapture MVMCallCapture;
typedef struct MVMCallCaptureBody MVMCallCaptureBody;
typedef struct MVMCapture MVMCapture;
typedef struct MVMCaptureBody MVMCaptureBody;
typedef struct MVMCallsite MVMCallsite;
typedef struct MVMCallsiteInterns MVMCallsiteInterns;
typedef struct MVMCallStackRecord MVMCallStackRecord;
typedef struct MVMCallStackStart MVMCallStackStart;
typedef struct MVMCallStackRegionStart MVMCallStackRegionStart;
typedef struct MVMCallStackFrame MVMCallStackFrame;
typedef struct MVMCallStackHeapFrame MVMCallStackHeapFrame;
typedef struct MVMCallStackPromotedFrame MVMCallStackPromotedFrame;
typedef struct MVMCallStackContinuationTag MVMCallStackContinuationTag;
typedef struct MVMCallStackFlattening MVMCallStackFlattening;
typedef struct MVMCallStackDispatchRecord MVMCallStackDispatchRecord;
typedef struct MVMCallStackDispatchRun MVMCallStackDispatchRun;
typedef struct MVMCallStackBindFailure MVMCallStackBindFailure;
typedef struct MVMCallStackRegion MVMCallStackRegion;
typedef struct MVMCallStackIterator MVMCallStackIterator;
typedef struct MVMCFunction MVMCFunction;
typedef struct MVMCFunctionBody MVMCFunctionBody;
typedef struct MVMCode MVMCode;
typedef struct MVMCodeBody MVMCodeBody;
typedef struct MVMCollectable MVMCollectable;
typedef struct MVMCompUnit MVMCompUnit;
typedef struct MVMCompUnitBody MVMCompUnitBody;
typedef struct MVMConcatState MVMConcatState;
typedef struct MVMContainerConfigurer MVMContainerConfigurer;
typedef struct MVMContainerSpec MVMContainerSpec;
typedef struct MVMContainerRegistry MVMContainerRegistry;
typedef struct MVMContext MVMContext;
typedef struct MVMContextBody MVMContextBody;
typedef struct MVMDecoder MVMDecoder;
typedef struct MVMDecoderBody MVMDecoderBody;
typedef struct MVMDLLRegistry MVMDLLRegistry;
typedef struct MVMDLLSym MVMDLLSym;
typedef struct MVMDLLSymBody MVMDLLSymBody;
typedef struct MVMException MVMException;
typedef struct MVMExceptionBody MVMExceptionBody;
typedef struct MVMExtOpRecord MVMExtOpRecord;
typedef struct MVMExtOpRegistry MVMExtOpRegistry;
typedef struct MVMExtRegistry MVMExtRegistry;
typedef struct MVMRegionAlloc MVMRegionAlloc;
typedef struct MVMRegionBlock MVMRegionBlock;
typedef struct MVMFixedSizeAlloc MVMFixedSizeAlloc;
typedef struct MVMFixedSizeAllocFreeListEntry MVMFixedSizeAllocFreeListEntry;
typedef struct MVMFixedSizeAllocSafepointFreeListEntry MVMFixedSizeAllocSafepointFreeListEntry;
typedef struct MVMFixedSizeAllocSizeClass MVMFixedSizeAllocSizeClass;
typedef struct MVMFixedSizeAllocThread MVMFixedSizeAllocThread;
typedef struct MVMFixedSizeAllocThreadSizeClass MVMFixedSizeAllocThreadSizeClass;
typedef struct MVMFrame MVMFrame;
typedef struct MVMFrameExtra MVMFrameExtra;
typedef struct MVMFrameHandler MVMFrameHandler;
typedef struct MVMGen2Allocator MVMGen2Allocator;
typedef struct MVMGen2SizeClass MVMGen2SizeClass;
typedef struct MVMGCPassedWork MVMGCPassedWork;
typedef struct MVMGCWorklist MVMGCWorklist;
typedef struct MVMHash MVMHash;
typedef struct MVMHashAttrStore MVMHashAttrStore;
typedef struct MVMHashAttrStoreBody MVMHashAttrStoreBody;
typedef struct MVMHashBody MVMHashBody;
typedef struct MVMHashEntry MVMHashEntry;
typedef struct MVMHLLConfig MVMHLLConfig;
typedef struct MVMIntConstCache MVMIntConstCache;
typedef struct MVMInstance MVMInstance;
typedef struct MVMInvocationSpec MVMInvocationSpec;
typedef struct MVMIter MVMIter;
typedef struct MVMIterBody MVMIterBody;
typedef struct MVMKnowHOWAttributeREPR MVMKnowHOWAttributeREPR;
typedef struct MVMKnowHOWAttributeREPRBody MVMKnowHOWAttributeREPRBody;
typedef struct MVMKnowHOWREPR MVMKnowHOWREPR;
typedef struct MVMKnowHOWREPRBody MVMKnowHOWREPRBody;
typedef struct MVMNFA MVMNFA;
typedef struct MVMNFABody MVMNFABody;
typedef struct MVMNFAStateInfo MVMNFAStateInfo;
typedef struct MVMNFGState MVMNFGState;
typedef struct MVMNFGSynthetic MVMNFGSynthetic;
typedef struct MVMNFGTrieNode MVMNFGTrieNode;
typedef struct MVMNFGTrieNodeEntry MVMNFGTrieNodeEntry;
typedef struct MVMNativeCall MVMNativeCall;
typedef struct MVMNativeCallBody MVMNativeCallBody;
typedef struct MVMNativeRef MVMNativeRef;
typedef struct MVMNativeRefBody MVMNativeRefBody;
typedef struct MVMNativeRefREPRData MVMNativeRefREPRData;
typedef struct MVMNormalizer MVMNormalizer;
typedef struct MVMNull MVMNull;
typedef struct MVMNullBody MVMNullBody;
typedef struct MVMCStr MVMCStr;
typedef struct MVMCStrBody MVMCStrBody;
typedef struct MVMCPointer MVMCPointer;
typedef struct MVMCPointerBody MVMCPointerBody;
typedef struct MVMCArray MVMCArray;
typedef struct MVMCArrayBody MVMCArrayBody;
typedef struct MVMCArrayREPRData MVMCArrayREPRData;
typedef struct MVMCStruct MVMCStruct;
typedef struct MVMCStructBody MVMCStructBody;
typedef struct MVMCStructNameMap MVMCStructNameMap;
typedef struct MVMCStructREPRData MVMCStructREPRData;
typedef struct MVMCPPStruct MVMCPPStruct;
typedef struct MVMCPPStructBody MVMCPPStructBody;
typedef struct MVMCPPStructNameMap MVMCPPStructNameMap;
typedef struct MVMCPPStructREPRData MVMCPPStructREPRData;
typedef struct MVMCUnion MVMCUnion;
typedef struct MVMCUnionBody MVMCUnionBody;
typedef struct MVMCUnionNameMap MVMCUnionNameMap;
typedef struct MVMCUnionREPRData MVMCUnionREPRData;
typedef struct MVMMultiDimArray MVMMultiDimArray;
typedef struct MVMMultiDimArrayBody MVMMultiDimArrayBody;
typedef struct MVMMultiDimArrayREPRData MVMMultiDimArrayREPRData;
typedef struct MVMContinuation MVMContinuation;
typedef struct MVMContinuationBody MVMContinuationBody;
typedef struct MVMReentrantMutex MVMReentrantMutex;
typedef struct MVMReentrantMutexBody MVMReentrantMutexBody;
typedef struct MVMConditionVariable MVMConditionVariable;
typedef struct MVMConditionVariableBody MVMConditionVariableBody;
typedef struct MVMSemaphore MVMSemaphore;
typedef struct MVMSemaphoreBody MVMSemaphoreBody;
typedef struct MVMConcBlockingQueue MVMConcBlockingQueue;
typedef struct MVMConcBlockingQueueBody MVMConcBlockingQueueBody;
typedef struct MVMConcBlockingQueueNode MVMConcBlockingQueueNode;
typedef struct MVMObject MVMObject;
typedef struct MVMObjectStooge MVMObjectStooge;
typedef struct MVMOpInfo MVMOpInfo;
typedef struct MVMOSHandle MVMOSHandle;
typedef struct MVMOSHandleBody MVMOSHandleBody;
typedef struct MVMP6bigint MVMP6bigint;
typedef struct MVMP6bigintBody MVMP6bigintBody;
typedef struct MVMP6int MVMP6int;
typedef struct MVMP6intBody MVMP6intBody;
typedef struct MVMP6intREPRData MVMP6intREPRData;
typedef struct MVMP6num MVMP6num;
typedef struct MVMP6numBody MVMP6numBody;
typedef struct MVMP6numREPRData MVMP6numREPRData;
typedef struct MVMP6opaque MVMP6opaque;
typedef struct MVMP6opaqueBody MVMP6opaqueBody;
typedef struct MVMP6opaqueBoxedTypeMap MVMP6opaqueBoxedTypeMap;
typedef struct MVMP6opaqueNameMap MVMP6opaqueNameMap;
typedef struct MVMP6opaqueREPRData MVMP6opaqueREPRData;
typedef struct MVMP6str MVMP6str;
typedef struct MVMP6strBody MVMP6strBody;
typedef union  MVMRegister MVMRegister;
typedef struct MVMREPROps MVMREPROps;
typedef struct MVMREPROps_Associative MVMREPROps_Associative;
typedef struct MVMREPROps_Attribute MVMREPROps_Attribute;
typedef struct MVMREPROps_Boxing MVMREPROps_Boxing;
typedef struct MVMREPROps_Positional MVMREPROps_Positional;
typedef struct MVMSerializationContext MVMSerializationContext;
typedef struct MVMSerializationContextBody MVMSerializationContextBody;
typedef struct MVMSerializationReader MVMSerializationReader;
typedef struct MVMDeserializeWorklist MVMDeserializeWorklist;
typedef struct MVMSerializationRoot MVMSerializationRoot;
typedef struct MVMSerializationWriter MVMSerializationWriter;
typedef struct MVMSpeshGraph MVMSpeshGraph;
typedef struct MVMSpeshMemBlock MVMSpeshMemBlock;
typedef struct MVMSpeshTemporary MVMSpeshTemporary;
typedef struct MVMSpeshBB MVMSpeshBB;
typedef struct MVMSpeshIns MVMSpeshIns;
typedef union MVMSpeshOperand MVMSpeshOperand;
typedef struct MVMSpeshAnn MVMSpeshAnn;
typedef struct MVMSpeshFacts MVMSpeshFacts;
typedef struct MVMSpeshCode MVMSpeshCode;
typedef struct MVMSpeshCandidate MVMSpeshCandidate;
typedef struct MVMSpeshCandidateBody MVMSpeshCandidateBody;
typedef struct MVMSpeshLogGuard MVMSpeshLogGuard;
typedef struct MVMSpeshCallInfo MVMSpeshCallInfo;
typedef struct MVMSpeshInline MVMSpeshInline;
typedef struct MVMSpeshIterator MVMSpeshIterator;
typedef struct MVMSpeshLog MVMSpeshLog;
typedef struct MVMSpeshLogBody MVMSpeshLogBody;
typedef struct MVMSpeshLogEntry MVMSpeshLogEntry;
typedef struct MVMSpeshStats MVMSpeshStats;
typedef struct MVMSpeshStatsByCallsite MVMSpeshStatsByCallsite;
typedef struct MVMSpeshStatsByType MVMSpeshStatsByType;
typedef struct MVMSpeshStatsType MVMSpeshStatsType;
typedef struct MVMSpeshStatsByOffset MVMSpeshStatsByOffset;
typedef struct MVMSpeshStatsTypeCount MVMSpeshStatsTypeCount;
typedef struct MVMSpeshStatsInvokeCount MVMSpeshStatsInvokeCount;
typedef struct MVMSpeshStatsTypeTupleCount MVMSpeshStatsTypeTupleCount;
typedef struct MVMSpeshStatsDispatchResultCount MVMSpeshStatsDispatchResultCount;
typedef struct MVMSpeshStatsStatic MVMSpeshStatsStatic;
typedef struct MVMSpeshSimStack MVMSpeshSimStack;
typedef struct MVMSpeshSimStackFrame MVMSpeshSimStackFrame;
typedef struct MVMSpeshSimCallType MVMSpeshSimCallType;
typedef struct MVMSpeshPlan MVMSpeshPlan;
typedef struct MVMSpeshPlanned MVMSpeshPlanned;
typedef struct MVMSpeshArgGuard MVMSpeshArgGuard;
typedef struct MVMSpeshArgGuardNode MVMSpeshArgGuardNode;
typedef struct MVMSpeshUsages MVMSpeshUsages;
typedef struct MVMSpeshUseChainEntry MVMSpeshUseChainEntry;
typedef struct MVMSpeshDeoptUseEntry MVMSpeshDeoptUseEntry;
typedef struct MVMSpeshFrameWalker MVMSpeshFrameWalker;
typedef struct MVMSpeshPEAInfo MVMSpeshPEAInfo;
typedef struct MVMSpeshPEAAllocation MVMSpeshPEAAllocation;
typedef struct MVMSpeshPEADeopt MVMSpeshPEADeopt;
typedef struct MVMSpeshPEAMaterializeInfo MVMSpeshPEAMaterializeInfo;
typedef struct MVMSpeshPEADeoptPoint MVMSpeshPEADeoptPoint;
typedef struct MVMConfigurationProgram MVMConfigurationProgram;
typedef struct MVMSTable MVMSTable;
typedef struct MVMStaticFrame MVMStaticFrame;
typedef struct MVMStaticFrameBody MVMStaticFrameBody;
typedef struct MVMStaticFrameInstrumentation MVMStaticFrameInstrumentation;
typedef struct MVMStaticFrameDebugLocal MVMStaticFrameDebugLocal;
typedef struct MVMStaticFrameSpesh MVMStaticFrameSpesh;
typedef struct MVMStaticFrameSpeshBody MVMStaticFrameSpeshBody;
typedef struct MVMStorageSpec MVMStorageSpec;
typedef struct MVMString MVMString;
typedef struct MVMStringBody MVMStringBody;
typedef struct MVMStringConsts MVMStringConsts;
typedef struct MVMStringStrand MVMStringStrand;
typedef struct MVMGraphemeIter MVMGraphemeIter;
typedef struct MVMCodepointIter MVMCodepointIter;
typedef struct MVMThread MVMThread;
typedef struct MVMThreadBody MVMThreadBody;
typedef struct MVMThreadContext MVMThreadContext;
typedef struct MVMUnicodeNamedValue MVMUnicodeNamedValue;
typedef struct MVMUninstantiable MVMUninstantiable;
typedef struct MVMWorkThread MVMWorkThread;
typedef struct MVMIOOps MVMIOOps;
typedef struct MVMIOClosable MVMIOClosable;
typedef struct MVMIOSyncReadable MVMIOSyncReadable;
typedef struct MVMIOSyncWritable MVMIOSyncWritable;
typedef struct MVMIOAsyncReadable MVMIOAsyncReadable;
typedef struct MVMIOAsyncWritable MVMIOAsyncWritable;
typedef struct MVMIOAsyncWritableTo MVMIOAsyncWritableTo;
typedef struct MVMIOSeekable MVMIOSeekable;
typedef struct MVMIOSockety MVMIOSockety;
typedef struct MVMIOIntrospection MVMIOIntrospection;
typedef struct MVMIOLockable MVMIOLockable;
typedef struct MVMDecodeStream MVMDecodeStream;
typedef struct MVMDecodeStreamBytes MVMDecodeStreamBytes;
typedef struct MVMDecodeStreamChars MVMDecodeStreamChars;
typedef struct MVMDecodeStreamSeparators MVMDecodeStreamSeparators;
typedef struct MVMNativeCallback MVMNativeCallback;
typedef struct MVMNativeCallbackCacheHead MVMNativeCallbackCacheHead;
typedef struct MVMJitGraph MVMJitGraph;
typedef struct MVMJitNode MVMJitNode;
typedef struct MVMJitDeopt MVMJitDeopt;
typedef struct MVMJitInline MVMJitInline;
typedef struct MVMJitHandler MVMJitHandler;
typedef struct MVMJitPrimitive MVMJitPrimitive;
typedef struct MVMJitBranch MVMJitBranch;
typedef struct MVMJitCallC MVMJitCallC;
typedef struct MVMJitCallArg MVMJitCallArg;
typedef struct MVMJitLabel MVMJitLabel;
typedef struct MVMJitGuard MVMJitGuard;
typedef struct MVMJitInvoke MVMJitInvoke;
typedef struct MVMJitJumpList MVMJitJumpList;
typedef struct MVMJitControl MVMJitControl;
typedef struct MVMJitData MVMJitData;
typedef struct MVMJitStackSlot MVMJitStackSlot;
typedef struct MVMJitCode MVMJitCode;
typedef struct MVMJitCompiler MVMJitCompiler;
typedef struct MVMJitExprTree MVMJitExprTree;
typedef struct MVMJitExprInfo MVMJitExprInfo;
typedef struct MVMJitExprTemplate MVMJitExprTemplate;
typedef struct MVMJitTreeTraverser MVMJitTreeTraverser;
typedef struct MVMJitTile MVMJitTile;
typedef struct MVMJitTileList MVMJitTileList;
typedef struct MVMJitTileTemplate MVMJitTileTemplate;
typedef struct MVMProfileThreadData MVMProfileThreadData;
typedef struct MVMProfileGC MVMProfileGC;
typedef struct MVMProfileCallNode MVMProfileCallNode;
typedef struct MVMProfileAllocationCount MVMProfileAllocationCount;
typedef struct MVMProfileDeallocationCount MVMProfileDeallocationCount;
typedef struct MVMProfileContinuationData MVMProfileContinuationData;
typedef struct MVMHeapSnapshotCollection MVMHeapSnapshotCollection;
typedef struct MVMHeapDumpIndex MVMHeapDumpIndex;
typedef struct MVMHeapDumpIndexSnapshotEntry MVMHeapDumpIndexSnapshotEntry;
typedef struct MVMHeapDumpTableOfContents MVMHeapDumpTableOfContents;
typedef struct MVMHeapSnapshotStats MVMHeapSnapshotStats;
typedef struct MVMHeapSnapshot MVMHeapSnapshot;
typedef struct MVMHeapSnapshotType MVMHeapSnapshotType;
typedef struct MVMHeapSnapshotStaticFrame MVMHeapSnapshotStaticFrame;
typedef struct MVMHeapSnapshotCollectable MVMHeapSnapshotCollectable;
typedef struct MVMHeapSnapshotReference MVMHeapSnapshotReference;
typedef struct MVMHeapSnapshotState MVMHeapSnapshotState;
typedef struct MVMHeapSnapshotWorkItem MVMHeapSnapshotWorkItem;
typedef struct MVMDebugServerBreakpointInfo MVMDebugServerBreakpointInfo;
typedef struct MVMDebugServerBreakpointFileTable MVMDebugServerBreakpointFileTable;
typedef struct MVMDebugServerBreakpointTable MVMDebugServerBreakpointTable;
typedef struct MVMDebugServerHandleTable MVMDebugServerHandleTable;
typedef struct MVMDebugServerHandleTableEntry MVMDebugServerHandleTableEntry;
typedef struct MVMDebugServerRequestData MVMDebugServerRequestData;
typedef struct MVMDebugServerRequestEntry MVMDebugServerRequestEntry;
typedef struct MVMDebugServerData MVMDebugServerData;
typedef struct MVMRunloopState MVMRunloopState;
typedef struct MVMStrHashTable MVMStrHashTable;
typedef struct MVMFixKeyHashTable MVMFixKeyHashTable;
typedef struct MVMIndexHashTable MVMIndexHashTable;
typedef struct MVMPtrHashTable MVMPtrHashTable;
typedef struct MVMUniHashTable MVMUniHashTable;
typedef struct MVMDispDefinition MVMDispDefinition;
typedef struct MVMDispRegistry MVMDispRegistry;
typedef struct MVMDispRegistryTable MVMDispRegistryTable;
typedef struct MVMDispInlineCache MVMDispInlineCache;
typedef struct MVMDispInlineCacheEntry MVMDispInlineCacheEntry;
typedef struct MVMDispInlineCacheEntryResolvedGetLexStatic MVMDispInlineCacheEntryResolvedGetLexStatic;
typedef struct MVMDispInlineCacheEntryMonomorphicDispatch MVMDispInlineCacheEntryMonomorphicDispatch;
typedef struct MVMDispInlineCacheEntryMonomorphicDispatchFlattening MVMDispInlineCacheEntryMonomorphicDispatchFlattening;
typedef struct MVMDispInlineCacheEntryPolymorphicDispatch MVMDispInlineCacheEntryPolymorphicDispatch;
typedef struct MVMDispInlineCacheEntryPolymorphicDispatchFlattening MVMDispInlineCacheEntryPolymorphicDispatchFlattening;
typedef struct MVMDispProgramOutcome MVMDispProgramOutcome;
typedef struct MVMDispProgramRecording MVMDispProgramRecording;
typedef struct MVMDispProgramRecordingValue MVMDispProgramRecordingValue;
typedef struct MVMDispProgramRecordingCapture MVMDispProgramRecordingCapture;
typedef struct MVMDispProgramRecordingResumeInit MVMDispProgramRecordingResumeInit;
typedef struct MVMDispProgramRecordingResumption MVMDispProgramRecordingResumption;
typedef struct MVMDispProgram MVMDispProgram;
typedef union MVMDispProgramConstant MVMDispProgramConstant;
typedef struct MVMDispProgramOp MVMDispProgramOp;
typedef struct MVMDispProgramResumption MVMDispProgramResumption;
typedef struct MVMDispProgramResumptionInitValue MVMDispProgramResumptionInitValue;
typedef struct MVMDispResumptionData MVMDispResumptionData;
typedef struct MVMDispResumptionState MVMDispResumptionState;
typedef struct MVMDispSysCall MVMDispSysCall;
typedef struct MVMDispSysCallHashEntry MVMDispSysCallHashEntry;
typedef struct MVMTracked MVMTracked;
typedef struct MVMTrackedBody MVMTrackedBody;
