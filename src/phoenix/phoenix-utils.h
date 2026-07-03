#ifndef PHOENIX_UTILS_H
#define PHOENIX_UTILS_H

#include <cmath>

namespace ns3
{

class PhoenixUtils
{
public:

    static double Clamp(double value,double low,double high)
    {
        if(value<low)
            return low;

        if(value>high)
            return high;

        return value;
    }

    static double Distance(double x1,double y1,double x2,double y2)
    {
        return std::sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    }

    static double Normalize(double value,double min,double max)
    {
        if(max==min)
            return 0.0;

        return (value-min)/(max-min);
    }

};

}

#endif
