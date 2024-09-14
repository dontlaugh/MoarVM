void MVM_line_coverage_instrument(MVMThreadContext *tc, MVMStaticFrame *static_frame);
void MVM_line_coverage_report(MVMThreadContext *tc, MVMString *filename, uint32_t line_number, MVMuint16 cache_slot, char *cache);

void MVM_breakpoint_instrument(MVMThreadContext *tc, MVMStaticFrame *static_frame);
