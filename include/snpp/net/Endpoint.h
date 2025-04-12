#pragma once
#include "../Core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SNPP_Endpoint SNPP_Endpoint;

SNPP_API SNPP_Status snpp_endpoint_send(SNPP_Endpoint* ep, const void* data, size_t size, uint8_t flags);
SNPP_API SNPP_Status snpp_endpoint_receive(SNPP_Endpoint* ep, void* buffer, size_t* received);
SNPP_API void snpp_endpoint_throttle_configure(SNPP_Endpoint* ep, uint32_t interval, uint32_t acceleration, uint32_t deceleration);

#ifdef __cplusplus
}
#endif