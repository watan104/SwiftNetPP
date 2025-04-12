#pragma once
#include "../Core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SNPP_Client SNPP_Client;

SNPP_API SNPP_Client* snpp_client_create();
SNPP_API void snpp_client_destroy(SNPP_Client* client);
SNPP_API SNPP_Status snpp_client_connect(SNPP_Client* client, const SNPP_Address* addr);

#ifdef __cplusplus
}
#endif