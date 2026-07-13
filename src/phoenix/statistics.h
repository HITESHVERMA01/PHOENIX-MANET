#ifndef PHOENIX_STATISTICS_H
#define PHOENIX_STATISTICS_H

#include <map>

#include "ns3/flow-monitor-module.h"

namespace ns3
{

class Statistics
{
public:

    static void PrintFlowStatistics(
        Ptr<FlowMonitor> monitor
    );

};

}

#endif