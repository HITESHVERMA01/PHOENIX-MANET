#include "statistics.h"

namespace ns3
{

void SimulationStatistics::Reset()
{
    txPackets = 0;
    rxPackets = 0;
    lostPackets = 0;

    throughput = 0.0;
    delay = 0.0;
    jitter = 0.0;
    pdr = 0.0;

    initialEnergy = 0.0;
    remainingEnergy = 0.0;
    consumedEnergy = 0.0;
}

void SimulationStatistics::Print() const
{
    std::cout << "\n========== Simulation Statistics ==========\n";

    std::cout << "Tx Packets        : " << txPackets << std::endl;
    std::cout << "Rx Packets        : " << rxPackets << std::endl;
    std::cout << "Lost Packets      : " << lostPackets << std::endl;

    std::cout << "Throughput (Kbps) : " << throughput << std::endl;
    std::cout << "Delay (s)         : " << delay << std::endl;
    std::cout << "Jitter (s)        : " << jitter << std::endl;
    std::cout << "PDR (%)           : " << pdr << std::endl;

    std::cout << "Initial Energy    : " << initialEnergy << std::endl;
    std::cout << "Remaining Energy  : " << remainingEnergy << std::endl;
    std::cout << "Consumed Energy   : " << consumedEnergy << std::endl;

    std::cout << "===========================================\n";
}

}