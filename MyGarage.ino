#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// Stato precedente del pin
int lastState = -1;
long lastUpdateId = 0; // Per evitare duplicati

// ------------------------------------------------------
// Invio messaggi a Telegram
// ------------------------------------------------------
void sendTelegramMessage(const String& message) {
    if (WiFi.status() != WL_CONNECTED) return;

    HTTPClient http;
    String url = "https://api.telegram.org/bot" +
                 String(BOT_TOKEN) +
                 "/sendMessage?chat_id=" +
                 String(CHAT_ID) +
                 "&text=" + message;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode <= 0) {
        Serial.print("Errore invio Telegram: ");
        Serial.println(httpCode);
    }
    http.end();
}

// ------------------------------------------------------
// Converti stringa in minuscolo (per case-insensitive)
// ------------------------------------------------------
String toLowerCase(String str) {
    str.toLowerCase();
    return str;
}

// ------------------------------------------------------
// Lettura messaggi Telegram
// ------------------------------------------------------
void checkTelegramUpdates() {
    if (WiFi.status() != WL_CONNECTED) return;

    HTTPClient http;
    String url = "https://api.telegram.org/bot" +
                 String(BOT_TOKEN) +
                 "/getUpdates?offset=" +
                 String(lastUpdateId + 1);
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        String payload = http.getString();

        DynamicJsonDocument doc(8192);
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
            JsonArray updates = doc["result"].as<JsonArray>();
            for (JsonObject upd : updates) {
                long updateId = upd["update_id"];
                lastUpdateId = updateId;

                const char* text     = upd["message"]["text"];
                long fromId          = upd["message"]["from"]["id"];
                const char* fromName = upd["message"]["from"]["first_name"];

                Serial.println("Messaggio Telegram ricevuto:");
                Serial.print("  Mittente ID: "); Serial.println(fromId);
                Serial.print("  Nome: "); Serial.println(fromName);
                Serial.print("  Testo: "); Serial.println(text);
                Serial.println("------------------------------");

                // Se il messaggio corrisponde al comando "stato" (case-insensitive)
                if (toLowerCase(String(text)) == toLowerCase(String(CMD_STATO))) {
                    int pinState = digitalRead(PIN_INPUT);
                    String stateStr = (pinState == 0) ? PIN_STATE_0 : PIN_STATE_1;
                    sendTelegramMessage("Stato attuale: " + stateStr);
                }
            }
        }
    }

    http.end();
}

// ------------------------------------------------------
// Setup
// ------------------------------------------------------
void setup() {
    Serial.begin(115200);
    pinMode(PIN_INPUT, INPUT);

    // Connessione Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connessione alla rete Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnesso.");
    Serial.print("IP: "); Serial.println(WiFi.localIP());

    // Messaggio di avvio
    sendTelegramMessage("ESP32 avviato e connesso alla rete.");
}

// ------------------------------------------------------
// Loop principale
// ------------------------------------------------------
void loop() {
    // Controllo pin
    int currentState = digitalRead(PIN_INPUT);
    if (currentState != lastState) {
        String stateStr = (currentState == 0) ? PIN_STATE_0 : PIN_STATE_1;
        sendTelegramMessage("Cambio stato: " + stateStr);
        lastState = currentState;
    }

    // Lettura messaggi Telegram
    checkTelegramUpdates();

    delay(500);  // Riduce carico CPU e rete
}
