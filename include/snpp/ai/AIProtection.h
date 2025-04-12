#pragma once
#include "../Core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float trafficThreshold;
    float anomalyScore;
    uint32_t banDuration;
    uint32_t sampleWindow;
} SNPP_AIConfig;

typedef struct {
    uint32_t totalPackets;
    uint32_t suspiciousPackets;
    uint32_t lastBanTime;
} SNPP_AIMetrics;

SNPP_API void snpp_ai_protection_init(const SNPP_AIConfig* config);
SNPP_API bool snpp_ai_detect_attack(const SNPP_Address* address, float trafficLevel);
SNPP_API void snpp_ai_get_metrics(SNPP_AIMetrics* metrics);

#ifdef __cplusplus
}
#endif