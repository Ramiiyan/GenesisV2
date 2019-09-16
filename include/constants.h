#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>

// WaterSensor Parameters
#define SENSOR1             2    
#define SENSOR2             3    
#define INTRUPT1            0       //for digital pin 2
#define INTRUPT2            1       //for digital pin 3
#define CALIFACT            7.5

// topic parameters
#define RES_TOPIC           "/9647632068185842/genesis/waterflow/v2/sub"
#define SUB_TOPIC           "+/9647632068185842/genesis/waterflow/v2/sub"
#define PUB_TOPIC           "genesis/waterflow/v2/common"

//mqtt parameters
#define MQTT_USERNAME       "genesis-waterflow-v2_2727"
#define MQTT_PASSWORD       "1566449253_2727"
#define MQTT_BROKER         "mqtt.iot.ideamart.io"
#define MQTT_PORT           1883

//dev board constants
#define ideaBoard_PWRKEY    13
#define ideaBoard_RX        8
#define ideaBoard_TX        7
#define ideaBoard_RST       11

//apn parameters
#define NET_APN             "dialogbb" //dialogbb or nbiot
#define APN_USER            ""
#define APN_PASS            ""

