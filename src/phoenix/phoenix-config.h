#ifndef PHOENIX_CONFIG_H
#define PHOENIX_CONFIG_H

namespace ns3
{

class PhoenixConfig
{
public:

    // ------------------------------
    // General
    // ------------------------------

    static constexpr double Version = 0.1;

    // ------------------------------
    // HELLO packets
    // ------------------------------

    static constexpr double HelloInterval = 1.0;

    // ------------------------------
    // Prediction
    // ------------------------------

    static constexpr double PredictionInterval = 0.5;

    // ------------------------------
    // Decision Engine
    // ------------------------------

    static constexpr double DecisionInterval = 1.0;

    // ------------------------------
    // Network Health
    // ------------------------------

    static constexpr double MaxQueueLength = 100.0;

    static constexpr double MaxRSSI = -20.0;

    static constexpr double MinRSSI = -100.0;

    // ------------------------------
    // Logging
    // ------------------------------

    static constexpr bool EnableLogging = true;

};

}

#endif
