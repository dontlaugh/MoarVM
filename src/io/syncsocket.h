#define MVM_SOCKET_FAMILY_UNSPEC 0
#define MVM_SOCKET_FAMILY_INET   1
#define MVM_SOCKET_FAMILY_INET6  2
#define MVM_SOCKET_FAMILY_UNIX   3

#define MVM_SOCKET_TYPE_ANY       0
#define MVM_SOCKET_TYPE_STREAM    1
#define MVM_SOCKET_TYPE_DGRAM     2
#define MVM_SOCKET_TYPE_RAW       3
#define MVM_SOCKET_TYPE_RDM       4
#define MVM_SOCKET_TYPE_SEQPACKET 5

#define MVM_SOCKET_PROTOCOL_ANY 0
#define MVM_SOCKET_PROTOCOL_TCP 1
#define MVM_SOCKET_PROTOCOL_UDP 2

MVMObject * MVM_io_socket_create(MVMThreadContext *tc, int64_t listen);
/* TODO: uint16_t can be too small for the machine's value for the
 *       given family, which this function doesn't use anymore in the
 *       first place and can be any Int from Raku land; it should be an
 *       int64_t instead. */
struct sockaddr * MVM_io_resolve_host_name(MVMThreadContext *tc,
        MVMString *host, int64_t port,
        uint16_t family, int64_t type, int64_t protocol,
        int32_t passive);
MVMString * MVM_io_get_hostname(MVMThreadContext *tc);
