#include "include/snpp/net/Client.h"

struct SNPP_Client {
    int dummy;
};

SNPP_Client* snpp_client_create() {
    return calloc(1, sizeof(SNPP_Client));
}