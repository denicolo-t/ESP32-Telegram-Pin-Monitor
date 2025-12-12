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

// --- Stato del pin (0/1) → stringa associata ---
#define PIN_STATE_0     "chiuso"
#define PIN_STATE_1     "aperto"

// --- Comando Telegram per interrogare lo stato ---
#define CMD_STATO       "stato"

#endif
