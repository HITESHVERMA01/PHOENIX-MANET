#ifndef PHOENIX_TYPES_H
#define PHOENIX_TYPES_H

#include <vector>

namespace ns3
{

struct NodeState
{
    uint32_t nodeId;

    double x;

    double y;

    double speed;

    double direction;

    double residualEnergy;

    double queueLength;

    double rssi;

    double collisionRate;

    double hopCount;

    double packetSuccessRate;
};

struct Prediction
{
    double futureRSSI;

    double linkExpirationTime;

    double futureDistance;

    double futureEnergy;
};

struct PriorityVector
{
    double energy;

    double stability;

    double throughput;

    double delay;

    double reliability;

    double lifetime;
};

struct RouteInfo
{
    std::vector<uint32_t> path;

    double fitness;

    double confidence;

    double delay;

    double energy;

    double overhead;
};

}

#endif
