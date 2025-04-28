#include "dataPush.h"


const char* ssid = "MoilapWiFi"; 
const char* password = "saokhongdung4g";

WiFiManager wm;

void internetInit(){
    // WiFi.mode(WIFI_STA);
    // wm.setConfigPortalBlocking(false);
    // wm.setConfigPortalTimeout(60);
    // bool res;
    // res = wm.autoConnect("Gateway1", "mesh1234");
    // if (!res){
    //     Serial.println("Failed to connect!");
    // }
    // else {
    //     Serial.println("Connected!");
    // }
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
}

void dataPusher(const String &sensorId, float temp, float humi, float voltage = 0){
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        String serverUrl = "http://192.168.100.234:3000/stream_data"; // Thay bằng IP của server
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        // Tạo JSON string
        String jsonPayload = "{";
        jsonPayload += "\"sensor_id\":\"" + sensorId + "\",";
        jsonPayload += "\"temperature\":" + String(temp) + ",";
        jsonPayload += "\"humidity\":" + String(humi);
        if (voltage > 0)
        {
            jsonPayload += ",\"voltage\":" + String(voltage);
        }
        jsonPayload += "}";

        // Gửi POST request
        Serial.println(jsonPayload);
        int httpResponseCode = http.POST(jsonPayload);

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode == 200)
        {
            Serial.println("✅ Gửi dữ liệu thành công\n");
        }
        else
        {
            Serial.println("❌ Gửi dữ liệu thất bại\n");
        }

        http.end();
    }
    else
    {
        Serial.println("⚠️ WiFi chưa kết nối!");
    }
}