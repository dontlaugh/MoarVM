
#include <stdint.h>

 void MVM_telemetry_timestamp(struct MVMThreadContext *threadID, const char *description);

 unsigned int MVM_telemetry_interval_start(struct MVMThreadContext *threadID, const char *description);
 void MVM_telemetry_interval_stop(struct MVMThreadContext *threadID, int intervalID, const char *description);
 void MVM_telemetry_interval_annotate(uintptr_t subject, int intervalID, const char *description);
 void MVM_telemetry_interval_annotate_dynamic(uintptr_t subject, int intervalID, char *description);

 void MVM_telemetry_init(FILE *outfile);
 void MVM_telemetry_finish(void);
