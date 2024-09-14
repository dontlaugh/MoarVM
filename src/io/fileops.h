#include <sys/types.h>

#define MVM_FILE_FLOCK_SHARED        1       /* Shared lock. Read lock */
#define MVM_FILE_FLOCK_EXCLUSIVE     2       /* Exclusive lock. Write lock. */
#define MVM_FILE_FLOCK_TYPEMASK      0x000F  /* a mask of lock type */
#define MVM_FILE_FLOCK_NONBLOCK      0x0010  /* asynchronous block during
                                              * locking the file */
#define MVM_STAT_EXISTS              0
#define MVM_STAT_FILESIZE            1
#define MVM_STAT_ISDIR               2
#define MVM_STAT_ISREG               3
#define MVM_STAT_ISDEV               4
#define MVM_STAT_CREATETIME          5
#define MVM_STAT_ACCESSTIME          6
#define MVM_STAT_MODIFYTIME          7
#define MVM_STAT_CHANGETIME          8
#define MVM_STAT_BACKUPTIME          9
#define MVM_STAT_UID                10
#define MVM_STAT_GID                11
#define MVM_STAT_ISLNK              12
#define MVM_STAT_PLATFORM_DEV       -1
#define MVM_STAT_PLATFORM_INODE     -2
#define MVM_STAT_PLATFORM_MODE      -3
#define MVM_STAT_PLATFORM_NLINKS    -4
#define MVM_STAT_PLATFORM_DEVTYPE   -5
#define MVM_STAT_PLATFORM_BLOCKSIZE -6
#define MVM_STAT_PLATFORM_BLOCKS    -7

int64_t MVM_file_stat(struct MVMThreadContext *tc, MVMString *filename, int64_t status, int32_t use_lstat);
double MVM_file_time(struct MVMThreadContext *tc, MVMString *filename, int64_t status, int32_t use_lstat);
void MVM_file_copy(struct MVMThreadContext *tc, MVMString *src, MVMString *dest);
void MVM_file_rename(struct MVMThreadContext *tc, MVMString *src, MVMString *dest);
void MVM_file_delete(struct MVMThreadContext *tc, MVMString *f);
void MVM_file_chmod(struct MVMThreadContext *tc, MVMString *f, int64_t flag);
void MVM_file_chown(struct MVMThreadContext *tc, MVMString *f, uint64_t uid, uint64_t gid);
uv_stat_t MVM_file_info(struct MVMThreadContext *tc, MVMString *filename, int32_t use_lstat);
int64_t MVM_file_info_with_error(struct MVMThreadContext *tc, uv_stat_t *stat, MVMString *filename, int32_t use_lstat);
int64_t MVM_file_exists(struct MVMThreadContext *tc, MVMString *f, int32_t use_lstat);
int64_t MVM_file_isreadable(struct MVMThreadContext *tc, MVMString *filename, int32_t use_lstat);
int64_t MVM_file_iswritable(struct MVMThreadContext *tc, MVMString *filename, int32_t use_lstat);
int64_t MVM_file_isexecutable(struct MVMThreadContext *tc, MVMString *filename, int32_t use_lstat);
MVMObject * MVM_file_get_stdstream(struct MVMThreadContext *tc, int32_t type);
MVMString * MVM_file_in_libpath(struct MVMThreadContext *tc, MVMString *orig);
void MVM_file_link(struct MVMThreadContext *tc, MVMString *oldpath, MVMString *newpath);
void MVM_file_symlink(struct MVMThreadContext *tc, MVMString *oldpath, MVMString *newpath);
MVMString * MVM_file_readlink(struct MVMThreadContext *tc, MVMString *path);
#ifndef _WIN32
int64_t MVM_are_we_group_member(struct MVMThreadContext *tc, gid_t group);
#endif
