MVMObject * MVM_disp_boot_constant_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_value_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_code_constant_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_code_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_syscall_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_resume_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_resume_caller_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_lang_call_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_lang_meth_call_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_lang_find_meth_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_lang_meth_not_found_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_boot_boolify_dispatch(MVMThreadContext *tc);
MVMObject * MVM_disp_lang_hllize_dispatch(MVMThreadContext *tc);
