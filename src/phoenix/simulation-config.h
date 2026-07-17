#ifndef PHOENIX_SIMULATION_CONFIG_H
#define PHOENIX_SIMULATION_CONFIG_H

#include <cstdint>
#include <string>

namespace ns3
{

struct SimulationConfig
{
    // Network
    uint32_t numberOfNodes = 20;

    double simulationTime = 100.0;
    throughput =(totalRxBytes * 8.0) /(simulationTime * 1000000.0);

    double areaWidth = 1000.0;

    double areaHeight = 1000.0;

    // Mobility

    double minimumSpeed = 1.0;

    double maximumSpeed = 5.0;

    double pauseTime = 0.0;

    // WiFi

    std::string wifiStandard = "802.11g";

    // Traffic

    uint32_t packetSize = 1024;

    double packetInterval = 0.05;

    uint32_t numberOfFlows = 10;

    // Routing

    std::string routingProtocol = "AODV";

    // Random Seed

    uint32_t seed = 1;
};

}

#endif