#ifndef CONFIG_H
#define CONFIG_H

// --- Wi-Fi ---
#define WIFI_SSID       "TUO_SSID"
#define WIFI_PASSWORD   "TUA_PASSWORD"

// --- Telegram ---
#define BOT_TOKEN       "INSERISCI_IL_TUO_TOKEN"
#define CHAT_ID         "INSERISCI_IL_TUO_CHAT_ID"

// --- Pin digitale da monitorare ---
#define PIN_INPUT       4

// --- Stati del pin (0/1) e messaggi associati ---
#define PIN_STATE_0_MSG "🔒 Pin chiuso"
#define PIN_STATE_1_MSG "🔓 Pin aperto"

// --- Comando Telegram per interrogare lo stato ---
#define CMD_STATO       "stato"

// --- Messaggi generali ---
#define MSG_START       "🚀 *ESP32 avviato* e connesso alla rete Wi-Fi!"
#define MSG_WIFI_RECON  "✅ *ESP32 riconnesso* alla rete Wi-Fi!"
#define MSG_WIFI_LOST   "⚠️ Wi-Fi persa! Tentativo di riconnessione..."
#define MSG_PIN_CHANGE_PREFIX "Pin aggiornato: "
#define MSG_PIN_STATUS  "📌 Stato pin attuale: "
#define MSG_ERROR_WIFI  "❌ Impossibile inviare messaggio, Wi-Fi non connessa"

#endif