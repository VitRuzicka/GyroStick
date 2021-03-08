
#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    byte yaw;
    byte pitch;
    byte roll;
    byte throttle;
    byte arm;
    byte setting;
    byte aux1;
    byte aux2;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Yaw: ");
  Serial.println(myData.yaw);
  Serial.print("Pitch: ");
  Serial.println(myData.pitch);
  Serial.print("Roll: ");
  Serial.println(myData.roll);
  Serial.print("Throttle: ");
  Serial.println(myData.throttle);
  Serial.print("Arm: ");
  Serial.println(myData.arm);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}

