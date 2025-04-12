#include "include/snpp/net/Peer.h"
#include "include/snpp/net/Host.h"
#include "include/snpp/utils/Allocator.h"
#include "include/snpp/net/Endpoint.h"
#include <stdlib.h>
#include <time.h>

struct SNPP_Peer {
    SNPP_Address address;
    SNPP_Host* host;
    uint32_t roundTripTime;
    uint32_t lastReceiveTime;
    uint32_t packetLoss;
    uint32_t bandwidth;
    uint32_t throttle[3];
    uint32_t packetsSent;
    uint32_t packetsReceived;
};

SNPP_Peer* snpp_peer_create(void* host, const SNPP_Address* address) {
    SNPP_Peer* peer = (SNPP_Peer*)snpp_malloc(sizeof(SNPP_Peer));
    if (!peer) return NULL;

    peer->host = (SNPP_Host*)host;
    peer->address = *address;
    peer->roundTripTime = 0;
    peer->lastReceiveTime = 0;
    peer->packetLoss = 0;
    peer->bandwidth = 0;
    peer->throttle[0] = 100;
    peer->throttle[1] = 2;
    peer->throttle[2] = 2;
    peer->packetsSent = 0;
    peer->packetsReceived = 0;

    return peer;
}

void snpp_peer_disconnect(SNPP_Peer* peer, uint32_t data) {
    if (!peer) return;

    uint32_t disconnectData = htonl(data);
    snpp_endpoint_send((SNPP_Endpoint*)peer, &disconnectData, sizeof(disconnectData), SNPP_FLAG_RELIABLE);

    snpp_free(peer);
}

void snpp_peer_ping(SNPP_Peer* peer) {
    if (!peer) return;

    uint32_t startTime = (uint32_t)time(NULL) * 1000;
    uint32_t pingData = htonl(startTime);
    snpp_endpoint_send((SNPP_Endpoint*)peer, &pingData, sizeof(pingData), 0);

    peer->roundTripTime = (uint32_t)time(NULL) * 1000 - startTime;
}

void snpp_peer_get_stats(SNPP_Peer* peer, SNPP_PeerStats* stats) {
    if (!peer || !stats) return;

    stats->roundTripTime = peer->roundTripTime;
    stats->lastReceiveTime = peer->lastReceiveTime;
    stats->packetLoss = (peer->packetsSent > 0) ? 
        (100 * (peer->packetsSent - peer->packetsReceived) / peer->packetsSent) : 0;
    stats->bandwidth = peer->bandwidth;
}

void snpp_peer_throttle_configure(SNPP_Peer* peer, uint32_t interval, 
                                uint32_t acceleration, uint32_t deceleration) {
    if (!peer) return;

    peer->throttle[0] = interval;
    peer->throttle[1] = acceleration;
    peer->throttle[2] = deceleration;
}