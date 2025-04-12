#pragma once
#include "../Core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SNPP_Peer SNPP_Peer;

typedef struct {
    uint32_t roundTripTime;
    uint32_t lastReceiveTime;
    uint32_t packetLoss;
    uint32_t bandwidth;
} SNPP_PeerStats;

SNPP_API SNPP_Peer* snpp_peer_create(void* host, const SNPP_Address* address);
SNPP_API void snpp_peer_disconnect(SNPP_Peer* peer, uint32_t data);
SNPP_API void snpp_peer_ping(SNPP_Peer* peer);
SNPP_API void snpp_peer_get_stats(SNPP_Peer* peer, SNPP_PeerStats* stats);
SNPP_API void snpp_peer_throttle_configure(SNPP_Peer* peer, uint32_t interval, uint32_t acceleration, uint32_t deceleration);

#ifdef __cplusplus
}
#endif