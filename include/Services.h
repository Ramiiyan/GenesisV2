#include "constants.h"

class Services{
    public:
    //constuctor
    //Services();

    //methods
    void initBoard(); //setup the board.
    void initSensor(); // setup the SensorSS
    boolean findNetwork();
    void gprsConnection();
    boolean connectToMqtt();
    boolean isMqttConnected();
    String getCpsi();
    void sendToPortal(double& r1, double& r2);
    //void callBack(char* t, byte* payload, unsigned int l);
    

};