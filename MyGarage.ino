#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

int lastState = -1;
long lastUpdateId = 0;
bool wifiConnected = false;

// ------------------------------------------------------
// Invio messaggi Telegram con emoji e Markdown
// ------------------------------------------------------
void sendTelegramMessage(const String& message) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println(MSG_ERROR_WIFI);
        return;
    }

    Serial.print("Invio messaggio Telegram: ");
    Serial.println(message);

    HTTPClient http;
    String url = "https://api.telegram.org/bot" +
                 String(BOT_TOKEN) +
                 "/sendMessage?chat_id=" +
                 String(CHAT_ID) +
                 "&text=" + message +
                 "&parse_mode=Markdown";
    Serial.print("URL richiesta: ");
    Serial.println(url);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.print("Telegram inviato con successo, codice HTTP: ");
        Serial.println(httpCode);
    } else {
        Serial.print("Errore invio Telegram, codice HTTP: ");
        Serial.println(httpCode);
    }
    http.end();
}

// ------------------------------------------------------
// Converti stringa in minuscolo (case-insensitive)
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

                if (toLowerCase(String(text)) == toLowerCase(String(CMD_STATO))) {
                    int pinState = digitalRead(PIN_INPUT);
                    String stateStr = (pinState == 0) ? PIN_STATE_0_MSG : PIN_STATE_1_MSG;
                    sendTelegramMessage(MSG_PIN_STATUS + stateStr);
                }
            }
        }
    }

    http.end();
}

// ------------------------------------------------------
// Gestione connessione Wi-Fi
// ------------------------------------------------------
void manageWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        if (wifiConnected) {
            Serial.println(MSG_WIFI_LOST);
            wifiConnected = false;
        }

        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            wifiConnected = true;
            Serial.println("\nWi-Fi riconnessa!");
            Serial.print("IP: "); Serial.println(WiFi.localIP());
            sendTelegramMessage(MSG_WIFI_RECON);

            // Invia subito lo stato corrente del pin
            int pinState = digitalRead(PIN_INPUT);
            String stateStr = (pinState == 0) ? PIN_STATE_0_MSG : PIN_STATE_1_MSG;
            sendTelegramMessage(MSG_PIN_STATUS + stateStr);
        } else {
            Serial.println("\nRiconnessione fallita, riprovo più tardi.");
        }
    } else {
        if (!wifiConnected) wifiConnected = true;
    }
}

// ------------------------------------------------------
// Setup
// ------------------------------------------------------
void setup() {
    Serial.begin(115200);
    pinMode(PIN_INPUT, INPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Connessione alla rete Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    wifiConnected = true;
    Serial.println("\nConnesso.");
    Serial.print("IP: "); Serial.println(WiFi.localIP());

    sendTelegramMessage(MSG_START);
}

// ------------------------------------------------------
// Loop principale
// ------------------------------------------------------
void loop() {
    // Gestione Wi-Fi
    manageWiFi();

    // Controllo pin
    int currentState = digitalRead(PIN_INPUT);
    if (currentState != lastState) {
        String stateStr = (currentState == 0) ? PIN_STATE_0_MSG : PIN_STATE_1_MSG;
        sendTelegramMessage(MSG_PIN_CHANGE_PREFIX + stateStr);
        lastState = currentState;
    }

    // Lettura messaggi Telegram
    checkTelegramUpdates();

    delay(500);
}
