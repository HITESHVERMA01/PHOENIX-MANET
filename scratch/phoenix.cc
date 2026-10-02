

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of Kansas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Justin Rohrer <rohrej@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

/*
 * This example program allows one to run ns-3 DSDV, AODV, or OLSR under
 * a typical random waypoint mobility model.
 *
 * By default, the simulation runs for 200 simulated seconds, of which
 * the first 50 are used for start-up time.  The number of nodes is 50.
 * Nodes move according to RandomWaypointMobilityModel with a speed of
 * 20 m/s and no pause time within a 300x1500 m region.  The WiFi is
 * in ad hoc mode with a 2 Mb/s rate (802.11b) and a Friis loss model.
 * The transmit power is set to 7.5 dBm.
 *
 * It is possible to change the mobility and density of the network by
 * directly modifying the speed and the number of nodes.  It is also
 * possible to change the characteristics of the network by changing
 * the transmit power (as power increases, the impact of mobility
 * decreases and the effective density increases).
 *
 * By default, OLSR is used, but specifying a value of 2 for the protocol
 * will cause AODV to be used, and specifying a value of 3 will cause
 * DSDV to be used.
 *
 * By default, there are 10 source/sink data pairs sending UDP data
 * at an application rate of 2.048 Kb/s each.    This is typically done
 * at a rate of 4 64-byte packets per second.  Application data is
 * started at a random time between 50 and 51 seconds and continues
 * to the end of the simulation.
 *
 * The program outputs a few items:
 * - packet receptions are notified to stdout such as:
 *   <timestamp> <node-id> received one packet from <src-address>
 * - each second, the data reception statistics are tabulated and output
 *   to a comma-separated value (csv) file
 * - some tracing and flow monitor configuration that used to work is
 *   left commented inline in the program
 */

#include <fstream>
#include <iostream>
#include <filesystem>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/aodv-module.h"
#include "ns3/olsr-module.h"
#include "ns3/dsdv-module.h"
#include "ns3/dsr-module.h"
#include "ns3/applications-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/energy-module.h"
#include "ns3/wifi-radio-energy-model.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor.h"



using namespace ns3;
using namespace dsr;



NS_LOG_COMPONENT_DEFINE ("manet-routing-compare");

/**
 * Routing experiment class.
 *
 * It handles the creation and run of an experiment.
 */
class RoutingExperiment
{
public:
  RoutingExperiment ();
/**
 * Run the experiment.
 * \param CSVfileName The output CSV filename.
 */
void Run (std::string CSVfileName);
  //static void SetMACParam (ns3::NetDeviceContainer & devices,
  //                                 int slotDistance);
  /**
   * Handles the command-line parmeters.
   * \param argc The argument count.
   * \param argv The argument vector.
   * \return the CSV filename.
   */
  std::string CommandSetup (int argc, char **argv);

private:
  /**
   * Setup the receiving socket in a Sink Node.
   * \param addr The address of the node.
   * \param node The node pointer.
   * \return the socket.
   */
  Ptr<Socket> SetupPacketReceive (Ipv4Address addr, Ptr<Node> node);
  /**
   * Receive a packet.
   * \param socket The receiving socket.
   */
  void ReceivePacket (Ptr<Socket> socket);
  /**
   * Compute the throughput.
   */
  void CheckThroughput ();

  uint32_t port;            //!< Receiving port number.
  uint32_t bytesTotal;      //!< Total received bytes.
  uint32_t packetsReceived; //!< Total received packets.

  std::string m_CSVfileName;  //!< CSV filename.
  uint32_t m_nSinks;               //!< Number of sink nodes.
  std::string m_protocolName; //!< Protocol name.
  double m_txp;               //!< Tx power.
  bool m_traceMobility;       //!< Enavle mobility tracing.
  uint32_t m_protocol;
    // ===============================
  // Experiment Configuration
  // ===============================
  uint32_t m_nNodes;
  double m_totalTime;
  double m_nodeSpeed;
  double m_nodePause;
  double m_appStartMin;
  std::string m_dataRate;
  uint32_t m_packetSize;
  uint32_t m_rngSeed;
  uint32_t m_rngRun;

FlowMonitorHelper m_flowHelper;

Ptr<FlowMonitor> m_flowMonitor;    //!< Protocol type.
};

RoutingExperiment::RoutingExperiment ()
  : port (9),
    bytesTotal (0),
    packetsReceived (0),
    m_CSVfileName ("manet-routing.output.csv"),
    m_nSinks (1),
    m_protocolName ("AODV"),
    m_txp (20.0),
    m_traceMobility (false),
    m_protocol (2),
    m_nNodes (50),
    m_totalTime (1000.0),
    m_nodeSpeed (5.0),
    m_nodePause (0.0),
    m_appStartMin (5.0),
    m_dataRate ("2Mbps"),
    m_packetSize (512),
    m_rngSeed (12345),
    m_rngRun (1)
{
}

static inline std::string
PrintReceivedPacket (Ptr<Socket> socket, Ptr<Packet> packet, Address senderAddress)
{
  std::ostringstream oss;

  oss << Simulator::Now ().GetSeconds () << " " << socket->GetNode ()->GetId ();

  if (InetSocketAddress::IsMatchingType (senderAddress))
    {
      InetSocketAddress addr = InetSocketAddress::ConvertFrom (senderAddress);
      oss << " received one packet from " << addr.GetIpv4 ();
    }
  else
    {
      oss << " received one packet!";
    }
  return oss.str ();
}

void
RoutingExperiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address senderAddress;
  while ((packet = socket->RecvFrom (senderAddress)))
    {
      bytesTotal += packet->GetSize ();
      packetsReceived += 1;
      NS_LOG_UNCOND (PrintReceivedPacket (socket, packet, senderAddress));
    }
}

void
RoutingExperiment::CheckThroughput ()
{
  double kbs = (bytesTotal * 8.0) / 1000;
  bytesTotal = 0;

  std::ofstream out (m_CSVfileName.c_str (), std::ios::app);

  out << (Simulator::Now ()).GetSeconds () << ","
      << kbs << ","
      << packetsReceived << ","
      << m_nSinks << ","
      << m_protocolName << ","
      << m_txp << ""
      << std::endl;

  out.close ();
  packetsReceived = 0;
  Simulator::Schedule (Seconds (1.0), &RoutingExperiment::CheckThroughput, this);
}

Ptr<Socket>
RoutingExperiment::SetupPacketReceive (Ipv4Address addr, Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  InetSocketAddress local = InetSocketAddress (addr, port);
  sink->Bind (local);
  sink->SetRecvCallback (MakeCallback (&RoutingExperiment::ReceivePacket, this));

  return sink;
}
std::string
RoutingExperiment::CommandSetup (int argc, char **argv)
{
  CommandLine cmd (__FILE__);

  // Output
  cmd.AddValue ("CSVfileName",
                "The name of the CSV output file",
                m_CSVfileName);

  // Routing
  cmd.AddValue ("protocol",
                "1=OLSR;2=AODV;3=DSDV;4=DSR",
                m_protocol);

  // Experiment configuration
  cmd.AddValue ("nodes",
                "Number of wireless nodes",
                m_nNodes);

  cmd.AddValue ("simTime",
                "Simulation time in seconds",
                m_totalTime);

  cmd.AddValue ("speed",
                "Maximum node speed in m/s",
                m_nodeSpeed);

  cmd.AddValue ("pause",
                "Node pause time in seconds",
                m_nodePause);

  cmd.AddValue ("flows",
                "Number of source/sink flows",
                m_nSinks);

  cmd.AddValue ("txPower",
                "Transmission power in dBm",
                m_txp);

  cmd.AddValue ("dataRate",
                "Application data rate",
                m_dataRate);

  cmd.AddValue ("packetSize",
                "Application packet size in bytes",
                m_packetSize);

  // Reproducibility
  cmd.AddValue ("seed",
                "Global random seed",
                m_rngSeed);

  cmd.AddValue ("run",
                "Random-number-generator run number",
                m_rngRun);

  cmd.AddValue ("appStart",
              "Minimum application start time in seconds",
              m_appStartMin);

  cmd.Parse (argc, argv);

  // Basic configuration validation
  NS_ABORT_MSG_IF (
      m_nNodes < 2,
      "nodes must be at least 2");

  NS_ABORT_MSG_IF (
      m_nSinks < 0 || (m_nSinks * 2) > m_nNodes,
      "flows must be >= 1 and 2*flows <= nodes");

  NS_ABORT_MSG_IF (
      m_totalTime <= 0.0,
      "simTime must be greater than 0");

  NS_ABORT_MSG_IF (
      m_nodeSpeed < 0.0,
      "speed cannot be negative");

  NS_ABORT_MSG_IF (
      m_txp <= 0.0,
      "txPower must be greater than 0");

  // Fix random seed/run before simulation objects are created.
  RngSeedManager::SetSeed (m_rngSeed);
  RngSeedManager::SetRun (m_rngRun);

  return m_CSVfileName;
}


int main (int argc, char *argv[])
{
  RoutingExperiment experiment;

  std::string CSVfileName =
      experiment.CommandSetup (argc, argv);

  // Blank out the legacy per-second CSV file
  // and write its headers.
  std::ofstream out (CSVfileName.c_str ());

  out << "SimulationSecond,"
      << "ReceiveRate,"
      << "PacketsReceived,"
      << "NumberOfSinks,"
      << "RoutingProtocol,"
      << "TransmissionPower"
      << std::endl;

  out.close ();

  experiment.Run (CSVfileName);
}

void
RoutingExperiment::Run (std::string CSVfileName)
{
  Packet::EnablePrinting ();
  m_CSVfileName = CSVfileName;

  uint32_t nWifis = m_nNodes;

double TotalTime = m_totalTime;
std::string rate (m_dataRate);
std::string phyMode ("DsssRate11Mbps");
std::string tr_name ("manet-routing-compare");

double nodeSpeed = m_nodeSpeed;
double nodePause = m_nodePause;

Config::SetDefault(
    "ns3::OnOffApplication::PacketSize",
    UintegerValue(m_packetSize));

Config::SetDefault(
    "ns3::OnOffApplication::DataRate",
    StringValue(rate));

  //Set Non-unicastMode rate to unicast mode
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode",StringValue (phyMode));

  NodeContainer adhocNodes;
  adhocNodes.Create (nWifis);

  // ===============================
// Energy Source Configuration
// ===============================

BasicEnergySourceHelper basicSourceHelper;
basicSourceHelper.Set("BasicEnergySourceInitialEnergyJ",
                      DoubleValue(10000.0));

ns3::energy::EnergySourceContainer sources =
    basicSourceHelper.Install(adhocNodes);

  // setting up wifi phy and channel using helpers
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay(
    "ns3::ConstantSpeedPropagationDelayModel");

wifiChannel.AddPropagationLoss(
    "ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a mac and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));

  wifiPhy.Set ("TxPowerStart", DoubleValue (m_txp));
wifiPhy.Set ("TxPowerEnd", DoubleValue (m_txp));

  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer adhocDevices = wifi.Install (wifiPhy, wifiMac, adhocNodes);

  // ===============================
// WiFi Radio Energy Model
// ===============================

WifiRadioEnergyModelHelper radioEnergyHelper;
    radioEnergyHelper.Install(adhocDevices, sources);
  MobilityHelper mobilityAdhoc;
  [[maybe_unused]] int64_t streamIndex = 0; // used to get consistent mobility across scenarios

  ObjectFactory pos;
  pos.SetTypeId(
    "ns3::RandomRectanglePositionAllocator");

pos.Set(
    "X",
    StringValue(
        "ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"));

pos.Set(
    "Y",
    StringValue(
        "ns3::UniformRandomVariable[Min=0.0|Max=1000.0]"));

  Ptr<PositionAllocator> taPositionAlloc = pos.Create ()->GetObject<PositionAllocator> ();
  streamIndex += taPositionAlloc->AssignStreams (streamIndex);

  std::stringstream ssSpeed;
ssSpeed << "ns3::UniformRandomVariable[Min=1.0|Max="
        << nodeSpeed
        << "]";

std::stringstream ssPause;
ssPause << "ns3::ConstantRandomVariable[Constant="
        << nodePause
        << "]";

mobilityAdhoc.SetMobilityModel(
    "ns3::RandomWaypointMobilityModel",
    "Speed", StringValue(ssSpeed.str()),
    "Pause", StringValue(ssPause.str()),
    "PositionAllocator", PointerValue(taPositionAlloc));
  mobilityAdhoc.SetPositionAllocator (taPositionAlloc);
  mobilityAdhoc.Install (adhocNodes);
  streamIndex += mobilityAdhoc.AssignStreams (adhocNodes, streamIndex);

  AodvHelper aodv;
  OlsrHelper olsr;
  DsdvHelper dsdv;
  DsrHelper dsr;
  DsrMainHelper dsrMain;
  Ipv4ListRoutingHelper list;
  InternetStackHelper internet;

  switch (m_protocol)
    {
    case 1:
      list.Add (olsr, 100);
      m_protocolName = "OLSR";
      break;
    case 2:
      list.Add (aodv, 100);
      m_protocolName = "AODV";
      break;
    case 3:
      list.Add (dsdv, 100);
      m_protocolName = "DSDV";
      break;
    case 4:
      m_protocolName = "DSR";
      break;
    default:
      NS_FATAL_ERROR ("No such protocol:" << m_protocol);
    }

  if (m_protocol < 4)
    {
      internet.SetRoutingHelper (list);
      internet.Install (adhocNodes);
    }
  else if (m_protocol == 4)
    {
      internet.Install (adhocNodes);
      dsrMain.Install (dsr, adhocNodes);
    }

  NS_LOG_INFO ("assigning ip address");

  Ipv4AddressHelper addressAdhoc;
  addressAdhoc.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer adhocInterfaces;
  adhocInterfaces = addressAdhoc.Assign (adhocDevices);

  OnOffHelper onoff1 ("ns3::UdpSocketFactory",Address ());
  onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));

  for (uint32_t i = 0; i < m_nSinks; ++i)
    {
      Ptr<Socket> sink = SetupPacketReceive (adhocInterfaces.GetAddress (i), adhocNodes.Get (i));

      AddressValue remoteAddress (InetSocketAddress (adhocInterfaces.GetAddress (i), port));
      onoff1.SetAttribute ("Remote", remoteAddress);

      Ptr<UniformRandomVariable> var = CreateObject<UniformRandomVariable> ();
      ApplicationContainer temp = onoff1.Install (adhocNodes.Get (i + m_nSinks));
      temp.Start (
    Seconds (
        var->GetValue (m_appStartMin,
                       m_appStartMin + 1.0)));
      temp.Stop (Seconds (TotalTime));
    }

  std::stringstream ss;
  ss << nWifis;
  std::string nodes = ss.str ();

  std::stringstream ss2;
  ss2 << nodeSpeed;
  std::string sNodeSpeed = ss2.str ();

  std::stringstream ss3;
  ss3 << nodePause;
  std::string sNodePause = ss3.str ();

  std::stringstream ss4;
  ss4 << rate;
  std::string sRate = ss4.str ();

  //NS_LOG_INFO ("Configure Tracing.");
  //tr_name = tr_name + "_" + m_protocolName +"_" + nodes + "nodes_" + sNodeSpeed + "speed_" + sNodePause + "pause_" + sRate + "rate";

  //AsciiTraceHelper ascii;
  //Ptr<OutputStreamWrapper> osw = ascii.CreateFileStream ( (tr_name + ".tr").c_str());
  //wifiPhy.EnableAsciiAll (osw);
  AsciiTraceHelper ascii;
  MobilityHelper::EnableAsciiAll (ascii.CreateFileStream (tr_name + ".mob"));

  m_flowMonitor = m_flowHelper.InstallAll();


  NS_LOG_INFO ("Run Simulation.");

  CheckThroughput ();


Simulator::Stop(Seconds(TotalTime));

Simulator::Run();
m_flowMonitor->CheckForLostPackets();
Ptr<Ipv4FlowClassifier> classifier =
    DynamicCast<Ipv4FlowClassifier>(m_flowHelper.GetClassifier());
std::map<FlowId, FlowMonitor::FlowStats> stats =
    m_flowMonitor->GetFlowStats();
std::cout << "\n==============================\n";
std::cout << "FLOW SUMMARY\n";
std::cout << "==============================\n";

uint64_t totalTxPackets = 0;
uint64_t totalRxPackets = 0;
uint64_t totalLostPackets = 0;

double totalDelay = 0.0;
double totalJitter = 0.0;
uint64_t totalRxBytes = 0;

// ===============================
// Energy Statistics
// ===============================

double totalRemainingEnergy = 0.0;
double totalConsumedEnergy = 0.0;

for (ns3::energy::EnergySourceContainer::Iterator it = sources.Begin();
     it != sources.End();
     ++it)
{
    Ptr<ns3::energy::BasicEnergySource> source =
    DynamicCast<ns3::energy::BasicEnergySource>(*it);

    double remaining = source->GetRemainingEnergy();

    totalRemainingEnergy += remaining;
    totalConsumedEnergy += (10000.0 - remaining);
}

double averageRemainingEnergy =
    totalRemainingEnergy / nWifis;

double averageConsumedEnergy =
    totalConsumedEnergy / nWifis;

for (const auto &flow : stats)
{
  totalTxPackets += flow.second.txPackets;
  totalRxPackets += flow.second.rxPackets;
  totalLostPackets += flow.second.lostPackets;

  totalDelay += flow.second.delaySum.GetSeconds();
  totalJitter += flow.second.jitterSum.GetSeconds();

  totalRxBytes += flow.second.rxBytes;

    std::cout << "Flow ID : " << flow.first << std::endl;

    std::cout << "Tx Packets : "
              << flow.second.txPackets << std::endl;

    std::cout << "Rx Packets : "
              << flow.second.rxPackets << std::endl;

    std::cout << "Lost Packets : "
              << flow.second.lostPackets << std::endl;

    std::cout << "Delay Sum : "
              << flow.second.delaySum.GetSeconds()
              << " s" << std::endl;

    std::cout << "Jitter Sum : "
              << flow.second.jitterSum.GetSeconds()
              << " s" << std::endl;

    std::cout << "------------------------------\n";
}

double pdr = 0.0;
double packetLoss = 0.0;
double avgDelay = 0.0;
double avgJitter = 0.0;
double throughput = 0.0;

if (totalTxPackets > 0)
{
    pdr = (double)totalRxPackets * 100.0 / totalTxPackets;
    packetLoss = (double)totalLostPackets * 100.0 / totalTxPackets;
}

if (totalRxPackets > 0)
{
    avgDelay = totalDelay / totalRxPackets;
    avgJitter = totalJitter / totalRxPackets;
}

// Replace 100.0 with your simulation time variable later if needed
throughput =(totalRxBytes * 8.0) /(TotalTime * 1000000.0);

std::cout << "\n==============================\n";
std::cout << "NETWORK SUMMARY\n";
std::cout << "==============================\n";

std::cout << "Total Tx Packets : " << totalTxPackets << std::endl;
std::cout << "Total Rx Packets : " << totalRxPackets << std::endl;
std::cout << "Total Lost Packets : " << totalLostPackets << std::endl;

std::cout << "PDR : " << pdr << " %" << std::endl;
std::cout << "Packet Loss : " << packetLoss << " %" << std::endl;

std::cout << "Average Delay : "
          << avgDelay
          << " s" << std::endl;

std::cout << "Average Jitter : "
          << avgJitter
          << " s" << std::endl;

std::cout << "Throughput : "
          << throughput
          << " Mbps" << std::endl;

std::cout << "Total Remaining Energy : "
          << totalRemainingEnergy
          << " J" << std::endl;

std::cout << "Total Consumed Energy : "
          << totalConsumedEnergy
          << " J" << std::endl;

std::cout << "Average Remaining Energy : "
          << averageRemainingEnergy
          << " J" << std::endl;

std::cout << "Average Consumed Energy : "
          << averageConsumedEnergy
          << " J" << std::endl;

const std::string resultsDir =
    "/mnt/d/projects/PHOENIX-MANET/results/";

const std::string summaryFile =
    resultsDir + "results_summary.csv";

const std::string flowFile =
    resultsDir + "flow_statistics.csv";

std::ofstream flowCsv(flowFile);
if (!flowCsv.is_open())
{
    std::cerr << "ERROR: Could not create flow_statistics.csv" << std::endl;
    return;
}

flowCsv
    << "FlowID,"
    << "TxPackets,"
    << "RxPackets,"
    << "LostPackets,"
    << "Delay,"
    << "Jitter\n";

for (const auto &flow : stats)
{
    flowCsv
        << flow.first << ","
        << flow.second.txPackets << ","
        << flow.second.rxPackets << ","
        << flow.second.lostPackets << ","
        << flow.second.delaySum.GetSeconds() << ","
        << flow.second.jitterSum.GetSeconds()
        << "\n";
}

flowCsv.close();



std::ifstream checkFile(summaryFile);
bool writeHeader =
    !checkFile.good() ||
    checkFile.peek() == std::ifstream::traits_type::eof();
checkFile.close();

// ---------- Open summary file ----------
std::ofstream summaryCsv(summaryFile, std::ios::app);

if (!summaryCsv.is_open())
{
    std::cerr << "ERROR: Could not create results_summary.csv" << std::endl;
    return;
}

if (writeHeader)
{
    summaryCsv
    << "Protocol,"
    << "Nodes,"
    << "SimulationTime,"
    << "Speed,"
    << "TxPackets,"
    << "RxPackets,"
    << "LostPackets,"
    << "PDR,"
    << "PacketLoss,"
    << "AverageDelay,"
    << "AverageJitter,"
    << "Throughput,"
    << "TotalRemainingEnergy,"
    << "TotalConsumedEnergy,"
    << "AverageRemainingEnergy,"
    << "AverageConsumedEnergy\n";
}

summaryCsv
    << "AODV,"
    << nWifis << ","
    << TotalTime << ","
    << nodeSpeed << ","
    << totalTxPackets << ","
    << totalRxPackets << ","
    << totalLostPackets << ","
    << pdr << ","
    << packetLoss << ","
    << avgDelay << ","
    << avgJitter << ","
    << throughput << ","
    << totalRemainingEnergy << ","
    << totalConsumedEnergy << ","
    << averageRemainingEnergy << ","
    << averageConsumedEnergy
    << "\n";

summaryCsv.close();

std::cout << "Network summary saved to results_summary.csv\n";

std::cout << "\nFlow statistics saved to flow_statistics.csv\n";

Simulator::Destroy();
}