#ifndef PHOENIX_NETWORK_BUILDER_H
#define PHOENIX_NETWORK_BUILDER_H

#include "simulation-config.h"

#include "ns3/network-module.h"

namespace ns3
{

class NetworkBuilder
{
public:

    explicit NetworkBuilder(const SimulationConfig& config);

    void Build();

    NodeContainer GetNodes() const;

private:

    SimulationConfig m_config;

    NodeContainer m_nodes;
};

}

#endif