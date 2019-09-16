#include "constants.h"
class WaterSensor {
    public:
    volatile int pulseCount;
    double flowVal;
    unsigned int flowMlVal;
    unsigned long totalMlVal,oldTime;

    //constructor
    WaterSensor();
    
    //methods
    void pulseCounter();
    double flowRate(volatile int &pluseCount);
    unsigned int flowML(double &flowVal);
    double totalFlow(unsigned int &flowMlVal);
    void printFunc();
    private:

};
