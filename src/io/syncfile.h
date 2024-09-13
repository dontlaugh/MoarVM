#ifndef SYNCFILE_H
#define SYNCFILE_H

MVMObject * MVM_file_open_fh(MVMThreadContext *tc, MVMString *filename, MVMString *mode);
MVMObject * MVM_file_handle_from_fd(MVMThreadContext *tc, uv_file fd);
#endif // SYNCFILE_H
