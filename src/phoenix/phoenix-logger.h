#ifndef PHOENIX_LOGGER_H
#define PHOENIX_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

namespace ns3
{

class PhoenixLogger
{
public:

    static void Info(const std::string& module,
                     const std::string& message);

    static void Warning(const std::string& module,
                        const std::string& message);

    static void Error(const std::string& module,
                      const std::string& message);

    static void OpenLogFile(const std::string& filename);

    static void CloseLogFile();

private:

    static std::ofstream m_logFile;

};

}

#endif