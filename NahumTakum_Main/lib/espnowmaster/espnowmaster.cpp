#include "ESPNowMaster.h"

uint8_t slaveMac1[] = {0xD0, 0xEF, 0x76, 0x18, 0x12, 0x4C};
uint8_t slaveMac2[] = {0xCC, 0x7B, 0x5C, 0xA8, 0xC5, 0x7C};
uint8_t slaveMac3[] = {0xCC, 0x7B, 0x5C, 0xAC, 0x7F, 0x80};
uint8_t slaveMac4[] = {0xCC, 0x7B, 0x5C, 0xAC, 0x7F, 0x78};
uint8_t slaveMac5[] = {0xCC, 0x7B, 0x5C, 0xAC, 0x7F, 0xB0};

//uint8_t slaveMac5[] = {0x80, 0x64, 0x6F, 0xC4, 0x92, 0xD0};//Of esp32 dummy

uint8_t* slaveMacs[] = {slaveMac1, slaveMac2, slaveMac3, slaveMac4, slaveMac5};
int slaveIDs[sizeof(slaveMacs) / sizeof(slaveMacs[0])];

SensorData ESPNowMaster::sensorData[NUM_SLAVES];

void ESPNowMaster::onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    const float smoothingFactor = 0.3;
    int slaveID = -1;
    
    for (int i = 0; i < sizeof(slaveMacs) / sizeof(slaveMacs[0]); i++) {
        bool macMatch = true;
        for (int j = 0; j < 6; j++) { // MAC address has 6 bytes
            if (mac[j] != slaveMacs[i][j]) {
                macMatch = false;
                break; // No need to continue checking if a byte doesn't match
            }
        }
        if (macMatch && len == sizeof(int) * 2) {
            slaveID = slaveIDs[i];
            break;
        }
    }

    if (slaveID != -1) {
        int raw_pitch = *((int*)incomingData);
        int yaw = *((int*)(incomingData + sizeof(int)));

        // Smooth the incoming data
        sensorData[slaveID - 1].raw_pitch = smoothingFactor * raw_pitch + (1 - smoothingFactor) * sensorData[slaveID - 1].raw_pitch;
        //sensorData[slaveID - 1].yaw = smoothingFactor * yaw + (1 - smoothingFactor) * sensorData[slaveID - 1].yaw;
        sensorData[slaveID - 1].yaw = yaw;
    } else {
        //Serial.println("Unknown slave or invalid data length");
    }
}

void ESPNowMaster::printMacAddress(const uint8_t *mac) {
    for (int i = 0; i < 6; ++i) {
        if (mac[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(mac[i], HEX);
        if (i < 5) {
            Serial.print(":");
        }
    }
}

SensorData ESPNowMaster::getSensorData(int slaveID) {

    if (slaveID >= 1 && slaveID <= NUM_SLAVES) {
        return sensorData[slaveID - 1]; // Adjust index for 0-based array
    } else {
        Serial.println("Invalid slave ID");
        return {0, 0}; // Return default values
    }
}

ESPNowMaster::ESPNowMaster() {

}

ESPNowMaster::~ESPNowMaster() {
    esp_now_deinit();
}

void ESPNowMaster::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Data sent to: ");
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Slave successfully");
    } else {
        Serial.println("Failed to send data to slave");
    }
}

void ESPNowMaster::init() {
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Add peers
    for (int i = 0; i < sizeof(slaveMacs) / sizeof(slaveMacs[0]); i++) {
        memcpy(peerInfos[i].peer_addr, slaveMacs[i], 6);
        peerInfos[i].channel = 0;
        peerInfos[i].encrypt = false;
        
        if (esp_now_add_peer(&peerInfos[i]) != ESP_OK) {
            Serial.println("Failed to add peer");
            return;
        }
    }

    esp_now_register_recv_cb(onDataRecv);

    Serial.println("ESP-NOW Initialized master");

    for (int i = 0; i < sizeof(slaveMacs) / sizeof(slaveMacs[0]); i++) {
        slaveIDs[i] = i + 1;
        Serial.print("MAC Address: ");
        printMacAddress(slaveMacs[i]);
        Serial.print(" | Slave ID: ");
        Serial.println(slaveIDs[i]);
    }
}
