#ifndef ESPNowMaster_h
#define ESPNowMaster_h

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure to hold data
struct SensorData {
    int raw_pitch;
    int yaw;
};

#define NUM_SLAVES 5

class ESPNowMaster {
  public:
    ESPNowMaster();
    ~ESPNowMaster();
    void init();
    static void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    SensorData getSensorData(int slaveID);

  private:
    esp_now_peer_info_t peerInfos[NUM_SLAVES]; // Define peerInfos array with a fixed size
    static SensorData sensorData[NUM_SLAVES]; // Private member to hold received data
    static void printMacAddress(const uint8_t *mac);
    
};

#endif // ESPNowMaster_h
