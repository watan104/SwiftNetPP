#pragma once
#include "../Core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SNPP_Host SNPP_Host;

typedef struct {
    SNPP_Address address;
    uint32_t maxConnections;
    uint32_t packetThrottleInterval;
    uint32_t packetThrottleAcceleration;
    uint32_t packetThrottleDeceleration;
    uint32_t peerTimeout;
    bool enableAIProtection;
} SNPP_HostConfig;

SNPP_API SNPP_Host* snpp_host_create(const SNPP_HostConfig* config);
SNPP_API void snpp_host_destroy(SNPP_Host* host);
SNPP_API SNPP_Status snpp_host_service(SNPP_Host* host, SNPP_Event* event, uint32_t timeout);
SNPP_API void snpp_host_flush(SNPP_Host* host);
SNPP_API void snpp_host_bandwidth_limit(SNPP_Host* host, uint32_t incoming, uint32_t outgoing);
SNPP_API void snpp_host_enable_ai_protection(SNPP_Host* host, bool enable);

#ifdef __cplusplus
}
#endif