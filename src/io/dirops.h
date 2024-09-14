void MVM_dir_mkdir(struct MVMThreadContext *tc, MVMString *path, int64_t mode);
void MVM_dir_rmdir(struct MVMThreadContext *tc, MVMString *path);
MVMObject * MVM_dir_open(struct MVMThreadContext *tc, MVMString *dirname);
MVMString * MVM_dir_read(struct MVMThreadContext *tc, MVMObject *oshandle);
void MVM_dir_close(struct MVMThreadContext *tc, MVMObject *oshandle);
MVMString * MVM_dir_cwd(struct MVMThreadContext *tc);
int MVM_dir_chdir_C_string(struct MVMThreadContext *tc, const char *dirstring);
void MVM_dir_chdir(struct MVMThreadContext *tc, MVMString *dir);
