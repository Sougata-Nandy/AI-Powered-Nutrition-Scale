#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_now.h>
#include "HX711.h"

// --- SETTINGS ---
const char* ssid = "Your_Hotspot_Name";
const char* password = "Your_Password";
const char* serverUrl = "http://10.27.202.141:5000/upload"; // Update your IP!
uint8_t broadcastAddress[] = {0x28, 0x05, 0xA5, 0x66, 0x0F, 0xF8}; // Update MAC!

typedef struct struct_message {
    char food[32];
    float calories;
    float weight;
    float cal100;
} struct_message;

struct_message myData;
HX711 scale;

void setup() {
  Serial.begin(115200);
  // (Camera & HX711 Init Logic here...)
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  
  esp_now_init();
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0; peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  // Trigger on Button Press
  if (digitalRead(12) == LOW) {
    float weight = scale.get_units(10);
    camera_fb_t * fb = esp_camera_fb_get();
    
    HTTPClient http;
    http.begin(serverUrl);
    http.setTimeout(15000); // 15-second timeout protection
    http.addHeader("x-weight", String(weight));
    
    int httpCode = http.POST(fb->buf, fb->len);
    if (httpCode == 200) {
       // (Parse JSON response into myData...)
       esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
    esp_camera_fb_return(fb);
  }
}