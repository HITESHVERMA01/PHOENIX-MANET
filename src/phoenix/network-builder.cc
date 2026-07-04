#include "network-builder.h"

namespace ns3
{

NetworkBuilder::NetworkBuilder(const SimulationConfig& config)
    : m_config(config)
{
}

void NetworkBuilder::Build()
{
    m_nodes.Create(m_config.numberOfNodes);
}

NodeContainer NetworkBuilder::GetNodes() const
{
    return m_nodes;
}

}