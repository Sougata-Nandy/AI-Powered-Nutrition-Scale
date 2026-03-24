#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_SSD1306.h>

#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 14

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

typedef struct struct_message {
    char food[32];
    float calories;
    float weight;
    float cal100;
} struct_message;

struct_message incomingData;
bool newData = false;

void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
    memcpy(&incomingData, data, sizeof(incomingData));
    newData = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_RED, OUTPUT); pinMode(LED_YELLOW, OUTPUT); pinMode(LED_GREEN, OUTPUT);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin("Your_Hotspot_Name", "Your_Password"); // Sync Channel
    
    Wire.begin(21, 22);
    lcd.init(); lcd.backlight();
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    
    esp_now_init();
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
    lcd.print("SYSTEM READY");
}

void loop() {
    if (newData) {
        // (Update LCD, OLED, and LEDs logic...)
        newData = false;
    }
}