void MVM_6model_parametric_setup(struct MVMThreadContext *tc, MVMObject *type, MVMObject *parameterizer);
void MVM_6model_parametric_parameterize(struct MVMThreadContext *tc, MVMObject *type, MVMObject *params,
    MVMRegister *result);
MVMObject * MVM_6model_parametric_try_find_parameterization(struct MVMThreadContext *tc, MVMSTable *st, MVMObject *params);
MVMObject * MVM_6model_parametric_type_parameterized(struct MVMThreadContext *tc, MVMObject *type);
MVMObject * MVM_6model_parametric_type_parameters(struct MVMThreadContext *tc, MVMObject *type);
MVMObject * MVM_6model_parametric_type_parameter_at(struct MVMThreadContext *tc, MVMObject *type, int64_t idx);
