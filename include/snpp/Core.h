#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#if defined(_WIN32)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SNPP_API extern
#define SNPP_PACKET_MTU 1400
#define SNPP_HOST_BANDWIDTH_LIMIT_UNLIMITED 0

typedef enum {
    SNPP_SUCCESS = 0,
    SNPP_ERROR_NETWORK,
    SNPP_ERROR_MEMORY,
    SNPP_ERROR_AI_PROTECTION
} SNPP_Status;

typedef enum {
    SNPP_FLAG_ACK = 1 << 0,
    SNPP_FLAG_RELIABLE = 1 << 1,
    SNPP_FLAG_UNSEQUENCED = 1 << 2,
    SNPP_FLAG_COMPRESSED = 1 << 3
} SNPP_PacketFlags;

typedef enum {
    SNPP_EVENT_TYPE_NONE = 0,
    SNPP_EVENT_TYPE_RECEIVED,
    SNPP_EVENT_TYPE_CONNECTED,
    SNPP_EVENT_TYPE_DISCONNECTED,
    SNPP_EVENT_TYPE_PEER_ADDED,
    SNPP_EVENT_TYPE_THROTTLE
} SNPP_EventType;

#pragma pack(push, 1)
typedef struct {
    union {
        uint32_t ipv4;
        uint8_t ipv6[16];
    };
    uint16_t port;
} SNPP_Address;

typedef struct {
    uint32_t sequence;
    uint16_t ack;
    uint8_t flags;
} SNPP_PacketHeader;

typedef struct {
    SNPP_EventType type;
    void* peer;
    const void* data;
    size_t dataLength;
    uint32_t timestamp;
} SNPP_Event;
#pragma pack(pop)

static_assert(sizeof(SNPP_Address) == 18, "Address size mismatch");
static_assert(sizeof(SNPP_PacketHeader) == 7, "Header size mismatch");

#ifdef __cplusplus
}
#endif