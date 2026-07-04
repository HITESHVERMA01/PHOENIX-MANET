#include "../src/phoenix/phoenix-core.h"

using namespace ns3;

int main()
{
    PhoenixLogger::OpenLogFile("phoenix.log");

    PhoenixLogger::Info("TEST", "PHOENIX Logger Started");

    PhoenixLogger::Warning("TEST", "This is a warning");

    PhoenixLogger::Error("TEST", "This is an error");

    PhoenixLogger::CloseLogFile();

    return 0;
}