MVMObject * MVM_file_open_fh(struct MVMThreadContext *tc, MVMString *filename, MVMString *mode);
MVMObject * MVM_file_handle_from_fd(struct MVMThreadContext *tc, uv_file fd);
