#include "statistics.h"

namespace ns3
{

SimulationStatistics::SimulationStatistics()
{
    Reset();
}

void SimulationStatistics::Reset()
{
    txPackets = 0;
    rxPackets = 0;
    lostPackets = 0;

    throughput = 0.0;
    delay = 0.0;
    jitter = 0.0;

    packetDeliveryRatio = 0.0;

    energyConsumed = 0.0;
}

void SimulationStatistics::Print() const
{
    std::cout << "\n========== PHOENIX Statistics ==========\n";

    std::cout << "Tx Packets : " << txPackets << std::endl;
    std::cout << "Rx Packets : " << rxPackets << std::endl;
    std::cout << "Lost Packets : " << lostPackets << std::endl;

    std::cout << "Throughput : " << throughput << " Kbps" << std::endl;

    std::cout << "Delay : " << delay << std::endl;

    std::cout << "Jitter : " << jitter << std::endl;

    std::cout << "PDR : " << packetDeliveryRatio << "%" << std::endl;

    std::cout << "Energy : " << energyConsumed << std::endl;

    std::cout << "========================================\n";
}

}