#include "include/snpp/net/Endpoint.h"
#include "include/snpp/net/Host.h"
#include "include/snpp/utils/Allocator.h"
#include "include/snpp/ai/AIProtection.h"
#include <string.h>
#include <time.h>

struct SNPP_Endpoint {
    SNPP_Address address;
    SNPP_Host* host;
    uint32_t throttleInterval;
    uint32_t throttleAcceleration;
    uint32_t throttleDeceleration;
    uint32_t lastSendTime;
    uint32_t sequenceNumber;
    int socketHandle;
};

SNPP_Status snpp_endpoint_send(SNPP_Endpoint* ep, const void* data, size_t size, uint8_t flags) {
    if (!ep || !data || size == 0 || size > SNPP_PACKET_MTU) {
        return SNPP_ERROR_NETWORK;
    }

    uint32_t currentTime = (uint32_t)time(NULL) * 1000;
    if (currentTime - ep->lastSendTime < ep->throttleInterval) {
        return SNPP_ERROR_NETWORK;
    }

    SNPP_PacketHeader header = {
        .flags = flags,
        .sequence = ep->sequenceNumber++,
        .ack = 0
    };

    char packetBuffer[SNPP_PACKET_MTU];
    memcpy(packetBuffer, &header, sizeof(SNPP_PacketHeader));
    memcpy(packetBuffer + sizeof(SNPP_PacketHeader), data, size);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ep->address.ipv4);
    addr.sin_port = htons(ep->address.port);

    size_t sentBytes = sendto(ep->socketHandle, packetBuffer, 
                              sizeof(SNPP_PacketHeader) + size, 0,
                              (struct sockaddr*)&addr, sizeof(addr));

    if (sentBytes <= 0) {
        return SNPP_ERROR_NETWORK;
    }

    ep->lastSendTime = currentTime;
    return SNPP_SUCCESS;
}

SNPP_Status snpp_endpoint_receive(SNPP_Endpoint* ep, void* buffer, size_t* received) {
    if (!ep || !buffer || !received) {
        return SNPP_ERROR_NETWORK;
    }

    char packetBuffer[SNPP_PACKET_MTU];
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);

    size_t receivedBytes = recvfrom(ep->socketHandle, packetBuffer, SNPP_PACKET_MTU, 0,
                                    (struct sockaddr*)&addr, &addrLen);

    if (receivedBytes <= 0) {
        return SNPP_ERROR_NETWORK;
    }

    if (receivedBytes < sizeof(SNPP_PacketHeader)) {
        return SNPP_ERROR_NETWORK;
    }

    SNPP_PacketHeader* header = (SNPP_PacketHeader*)packetBuffer;
    size_t dataSize = receivedBytes - sizeof(SNPP_PacketHeader);

    if (ep->host->aiProtectionEnabled) {
        if (snpp_ai_detect_attack(&ep->address, (float)dataSize)) {
            return SNPP_ERROR_AI_PROTECTION;
        }
    }

    memcpy(buffer, packetBuffer + sizeof(SNPP_PacketHeader), dataSize);
    *received = dataSize;
    return SNPP_SUCCESS;
}

void snpp_endpoint_throttle_configure(SNPP_Endpoint* ep, uint32_t interval, 
                                    uint32_t acceleration, uint32_t deceleration) {
    if (ep) {
        ep->throttleInterval = interval;
        ep->throttleAcceleration = acceleration;
        ep->throttleDeceleration = deceleration;
    }
}