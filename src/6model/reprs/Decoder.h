/* Representation used for a VM-provided decoder. */
struct MVMDecoderBody {
    atomic_uintptr_t in_use;
    MVMDecodeStream *ds;
    MVMDecodeStreamSeparators *sep_spec;
};
struct MVMDecoder {
    MVMObject common;
    MVMDecoderBody body;
};

/* Function for REPR setup. */
const MVMREPROps * MVMDecoder_initialize(MVMThreadContext *tc);

/* Operations on a Decoder object. */
void MVM_decoder_ensure_decoder(MVMThreadContext *tc, MVMObject *decoder, const char *op);
void MVM_decoder_configure(MVMThreadContext *tc, MVMDecoder *decoder,
                           MVMString *encoding, MVMObject *config);
void MVM_decoder_set_separators(MVMThreadContext *tc, MVMDecoder *decoder, MVMObject *sep_strings);
int64_t MVM_decoder_empty(MVMThreadContext *tc, MVMDecoder *decoder);
void MVM_decoder_add_bytes(MVMThreadContext *tc, MVMDecoder *decoder, MVMObject *blob);
MVMString * MVM_decoder_take_all_chars(MVMThreadContext *tc, MVMDecoder *decoder);
MVMString * MVM_decoder_take_available_chars(MVMThreadContext *tc, MVMDecoder *decoder);
MVMString * MVM_decoder_take_chars(MVMThreadContext *tc, MVMDecoder *decoder, int64_t chars,
                                   int64_t eof);
MVMString * MVM_decoder_take_line(MVMThreadContext *tc, MVMDecoder *decoder,
                                  int64_t chomp, int64_t incomplete_ok);
int64_t MVM_decoder_bytes_available(MVMThreadContext *tc, MVMDecoder *decoder);
MVMObject * MVM_decoder_take_bytes(MVMThreadContext *tc, MVMDecoder *decoder,
                                   MVMObject *buf_type, int64_t bytes);
