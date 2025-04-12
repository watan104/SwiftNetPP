#include "include/snpp/ai/AIProtection.h"
#include <math.h>
#include <string.h>
#include <time.h>

typedef struct {
    SNPP_Address address;
    float trafficLevel;
    uint32_t lastUpdate;
} TrafficRecord;

static SNPP_AIConfig currentConfig;
static SNPP_AIMetrics metrics;
static TrafficRecord* trafficRecords = NULL;
static uint32_t recordCount = 0;

void snpp_ai_protection_init(const SNPP_AIConfig* config) {
    currentConfig = *config;
    memset(&metrics, 0, sizeof(metrics));

    if (trafficRecords) {
        snpp_free(trafficRecords);
    }
    trafficRecords = (TrafficRecord*)snpp_malloc(currentConfig.sampleWindow * sizeof(TrafficRecord));
    recordCount = 0;
}

bool snpp_ai_detect_attack(const SNPP_Address* address, float trafficLevel) {
    metrics.totalPackets++;

    uint32_t currentTime = (uint32_t)time(NULL);
    float avgTraffic = 0.0f;
    uint32_t validRecords = 0;

    for (uint32_t i = 0; i < recordCount; i++) {
        if (currentTime - trafficRecords[i].lastUpdate <= currentConfig.sampleWindow) {
            avgTraffic += trafficRecords[i].trafficLevel;
            validRecords++;
        }
    }

    if (validRecords > 0) {
        avgTraffic /= validRecords;
    }

    float anomalyScore = fabsf(trafficLevel - avgTraffic) / (avgTraffic > 0.0f ? avgTraffic : 1.0f);

    if (recordCount < currentConfig.sampleWindow) {
        trafficRecords[recordCount].address = *address;
        trafficRecords[recordCount].trafficLevel = trafficLevel;
        trafficRecords[recordCount].lastUpdate = currentTime;
        recordCount++;
    } else {
        memmove(trafficRecords, trafficRecords + 1, (currentConfig.sampleWindow - 1) * sizeof(TrafficRecord));
        trafficRecords[currentConfig.sampleWindow - 1].address = *address;
        trafficRecords[currentConfig.sampleWindow - 1].trafficLevel = trafficLevel;
        trafficRecords[currentConfig.sampleWindow - 1].lastUpdate = currentTime;
    }

    if (anomalyScore > currentConfig.anomalyScore) {
        metrics.suspiciousPackets++;
        return true;
    }

    return false;
}

void snpp_ai_get_metrics(SNPP_AIMetrics* outMetrics) {
    if (outMetrics) {
        *outMetrics = metrics;
    }
}