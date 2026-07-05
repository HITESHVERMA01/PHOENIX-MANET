#ifndef PHOENIX_STATISTICS_H
#define PHOENIX_STATISTICS_H

#include <string>
#include <iostream>

namespace ns3
{

struct SimulationStatistics
{
    // Packet Statistics
    uint32_t txPackets = 0;
    uint32_t rxPackets = 0;
    uint32_t lostPackets = 0;

    // Performance Metrics
    double throughput = 0.0;
    double delay = 0.0;
    double jitter = 0.0;
    double pdr = 0.0;

    // Energy
    double initialEnergy = 0.0;
    double remainingEnergy = 0.0;
    double consumedEnergy = 0.0;

    void Reset();

    void Print() const;
};

}

#endif