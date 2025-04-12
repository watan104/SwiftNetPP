#include "include/snpp/net/Host.h"
#include "include/snpp/ai/AIProtection.h"
#include "include/snpp/net/Peer.h"
#include "include/snpp/net/Endpoint.h"
#include <stdlib.h>
#include <string.h>

struct SNPP_Host {
    SNPP_Address address;
    uint32_t maxConnections;
    uint32_t bandwidthIncoming;
    uint32_t bandwidthOutgoing;
    bool aiProtectionEnabled;
    SNPP_Peer** peers;
    int socketHandle;
};

SNPP_Host* snpp_host_create(const SNPP_HostConfig* config) {
    SNPP_Host* host = (SNPP_Host*)snpp_malloc(sizeof(SNPP_Host));
    if (!host) return NULL;

    host->address = config->address;
    host->maxConnections = config->maxConnections;
    host->bandwidthIncoming = SNPP_HOST_BANDWIDTH_LIMIT_UNLIMITED;
    host->bandwidthOutgoing = SNPP_HOST_BANDWIDTH_LIMIT_UNLIMITED;
    host->aiProtectionEnabled = config->enableAIProtection;
    host->peers = (SNPP_Peer**)snpp_malloc(config->maxConnections * sizeof(SNPP_Peer*));
    memset(host->peers, 0, config->maxConnections * sizeof(SNPP_Peer*));

    host->socketHandle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (host->socketHandle < 0) {
        snpp_free(host->peers);
        snpp_free(host);
        return NULL;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(host->address.ipv4);
    addr.sin_port = htons(host->address.port);

    if (bind(host->socketHandle, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(host->socketHandle);
        snpp_free(host->peers);
        snpp_free(host);
        return NULL;
    }

    return host;
}

void snpp_host_destroy(SNPP_Host* host) {
    if (!host) return;

    for (uint32_t i = 0; i < host->maxConnections; i++) {
        if (host->peers[i]) {
            snpp_peer_disconnect(host->peers[i], 0);
        }
    }

    close(host->socketHandle);
    snpp_free(host->peers);
    snpp_free(host);
}

SNPP_Status snpp_host_service(SNPP_Host* host, SNPP_Event* event, uint32_t timeout) {
    if (!host || !event) return SNPP_ERROR_NETWORK;

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(host->socketHandle, &readSet);

    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    int result = select(host->socketHandle + 1, &readSet, NULL, NULL, &tv);
    if (result <= 0) {
        event->type = SNPP_EVENT_TYPE_NONE;
        return (result == 0) ? SNPP_SUCCESS : SNPP_ERROR_NETWORK;
    }

    char buffer[SNPP_PACKET_MTU];
    size_t received = recv(host->socketHandle, buffer, SNPP_PACKET_MTU, 0);
    if (received <= 0) {
        event->type = SNPP_EVENT_TYPE_NONE;
        return SNPP_ERROR_NETWORK;
    }

    event->type = SNPP_EVENT_TYPE_RECEIVED;
    event->data = buffer;
    event->dataLength = received;
    return SNPP_SUCCESS;
}

void snpp_host_flush(SNPP_Host* host) {
    if (!host) return;
}

void snpp_host_bandwidth_limit(SNPP_Host* host, uint32_t incoming, uint32_t outgoing) {
    if (!host) return;

    host->bandwidthIncoming = incoming;
    host->bandwidthOutgoing = outgoing;
}

void snpp_host_enable_ai_protection(SNPP_Host* host, bool enable) {
    if (host) {
        host->aiProtectionEnabled = enable;
    }
}