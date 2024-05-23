#include "ESPNowSlave.h"
//CC:7B:5C:AC:7F:64
uint8_t masterMac[] = {0xCC, 0x7B, 0x5C, 0xAC, 0x7F, 0x64};//Final Master PCB

void ESPNowSlave::OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    Serial.print("Data received from: ");
    if (memcmp(mac, masterMac, sizeof(masterMac)) == 0 && len == 1 && incomingData[0] == '1') {
        Serial.println("Master");
    }
}

ESPNowSlave::ESPNowSlave() {

}

ESPNowSlave::~ESPNowSlave() {
    esp_now_deinit();
}

void ESPNowSlave::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("Data sent to: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    //Serial.println("Signal sent to master successfully");
  } else {
    //Serial.println("Failed to send signal to master");
  }
}

void ESPNowSlave::init() {

    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);

    memcpy(peerInfo.peer_addr, masterMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("ESP-NOW Initialized slave");

    //esp_now_register_recv_cb(OnDataRecv);

}

void ESPNowSlave::sendData(int data1, int data2) {

    int data[2] = {data1, data2};

    esp_err_t result = esp_now_send(masterMac, (uint8_t*)&data, sizeof(data));

    if (result == ESP_OK) {
        //Serial.print("Data sent to slave with success : ");
    } else {
        //Serial.println("Error sending data to slave");
    }
}
