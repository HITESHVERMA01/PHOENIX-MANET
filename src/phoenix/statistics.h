#ifndef PHOENIX_STATISTICS_H
#define PHOENIX_STATISTICS_H

#include <string>
#include <iostream>

namespace ns3
{

class SimulationStatistics
{
public:

    uint64_t txPackets;
    uint64_t rxPackets;
    uint64_t lostPackets;

    double throughput;
    double delay;
    double jitter;

    double packetDeliveryRatio;

    double energyConsumed;

    SimulationStatistics();

    void Reset();

    void Print() const;
};

}

#endif