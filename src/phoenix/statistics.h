#ifndef PHOENIX_STATISTICS_H
#define PHOENIX_STATISTICS_H

#include <cstdint>
#include "statistics.h"

namespace ns3
{

struct SimulationStatistics
{
    uint32_t txPackets = 0;
    uint32_t rxPackets = 0;
    uint32_t lostPackets = 0;

    double throughput = 0.0;
    double delay = 0.0;
    double jitter = 0.0;
    double packetDeliveryRatio = 0.0;
};

}

#endif