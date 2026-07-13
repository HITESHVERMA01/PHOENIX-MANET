#include "statistics.h"

#include <iostream>

namespace ns3
{

void
Statistics::PrintFlowStatistics(
    Ptr<FlowMonitor> monitor
)
{
    monitor->CheckForLostPackets();

    std::map<FlowId, FlowMonitor::FlowStats> stats =
        monitor->GetFlowStats();

    std::cout
        << "\n========== FLOW SUMMARY ==========\n";

    std::cout
        << "Total Flows : "
        << stats.size()
        << std::endl;

    std::cout
        << "==================================\n";
}

}