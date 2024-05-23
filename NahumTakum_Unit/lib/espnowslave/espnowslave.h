#ifndef ESPNOWSLAVE_H
#define ESPNOWSLAVE_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

class ESPNowSlave {
public:
    ESPNowSlave();
    ~ESPNowSlave();
    void init();
    void sendData(int data1, int data2);
    static void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
private:
    esp_now_peer_info_t peerInfo;
};

#endif // ESPNOWSLAVE_H
