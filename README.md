# AI-Powered Smart Nutrition Scale
An MCA Final Year Project developed at Ramaiah Institute of Technology.

## 🚀 Features
* **Dual-Microcontroller Setup**: ESP32-CAM for sensing and Standard ESP32 for UI.
* **Wireless Bridge**: Uses ESP-NOW protocol for low-latency data transfer.
* **AI Orchestration**: Integrates Clarifai (Visual AI) and USDA (Nutritional Data).
* **Graceful Degradation**: Heuristic color-based detection fallback.

## 🛠️ Hardware Used
* ESP32-CAM, ESP32 Dev Module, HX711 Load Cell.
* 16x2 I2C LCD, 0.96" OLED, R/Y/G LEDs.

## 💻 Tech Stack
* **Language**: C++ (Arduino), Python (Flask).
* **APIs**: Clarifai, USDA FoodData Central.
* **Protocols**: HTTP POST, ESP-NOW, I2C.