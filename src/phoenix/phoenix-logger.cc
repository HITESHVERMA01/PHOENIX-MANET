#include "phoenix-logger.h"

namespace ns3
{

std::ofstream PhoenixLogger::m_logFile;

void PhoenixLogger::OpenLogFile(const std::string& filename)
{
    m_logFile.open(filename);
}

void PhoenixLogger::CloseLogFile()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void PhoenixLogger::Info(const std::string& module,
                         const std::string& message)
{
    std::cout << "[INFO][" << module << "] "
              << message << std::endl;

    if (m_logFile.is_open())
    {
        m_logFile << "[INFO][" << module << "] "
                  << message << std::endl;
    }
}

void PhoenixLogger::Warning(const std::string& module,
                            const std::string& message)
{
    std::cout << "[WARNING][" << module << "] "
              << message << std::endl;

    if (m_logFile.is_open())
    {
        m_logFile << "[WARNING][" << module << "] "
                  << message << std::endl;
    }
}

void PhoenixLogger::Error(const std::string& module,
                          const std::string& message)
{
    std::cout << "[ERROR][" << module << "] "
              << message << std::endl;

    if (m_logFile.is_open())
    {
        m_logFile << "[ERROR][" << module << "] "
                  << message << std::endl;
    }
}

}