# ESP32 Telegram Pin Monitor

Questo progetto consente di monitorare lo stato di un pin digitale su un ESP32 e di comunicare le informazioni tramite un bot Telegram. Include sia l’invio automatico di messaggi al cambio di stato sia la ricezione di comandi da Telegram.

---

## **Funzionalità principali**

1. **Connessione Wi-Fi**
   - L’ESP32 si connette automaticamente alla rete Wi-Fi configurata in `config.h`.  

2. **Invio messaggi Telegram**
   - All’avvio del dispositivo, invia un messaggio di conferma (“ESP32 avviato e connesso alla rete”).
   - Ogni volta che lo stato del pin digitale cambia, invia un messaggio con lo stato aggiornato.  
   - La mappatura dello stato digitale 0/1 è personalizzabile tramite `config.h` (es: 0 → "chiuso", 1 → "aperto").

3. **Ricezione messaggi Telegram**
   - L’ESP32 legge periodicamente i messaggi ricevuti dal bot Telegram tramite l’API `getUpdates`.
   - Tutti i messaggi ricevuti vengono stampati sulla seriale per il debug.
   - Se riceve il comando configurato (`stato`) in qualsiasi forma di maiuscole/minuscole, risponde con lo stato attuale del pin.  

4. **Configurazione centralizzata**
   - Tutte le variabili configurabili (SSID, password, token bot, chat ID, pin monitorato, mappa stato, comando) sono contenute in `config.h`.
   - Un file `config.h.sample` è fornito come esempio dei valori da inserire.

5. **Case-insensitive**
   - Il comando Telegram per interrogare lo stato funziona indipendentemente dalla maiuscola/minuscola (`stato`, `STATO`, `Stato`…).

---

## **Struttura del progetto**

```
/ESP32-Telegram-PinMonitor/
│
├─ main.ino            # Sketch principale
├─ config.h            # Configurazione personale (da creare a partire dal sample)
├─ config.h.sample     # Esempio di configurazione
└─ README.md           # Documentazione del progetto
```

---

## **Requisiti**

- ESP32 WROOM o compatibile
- IDE Arduino aggiornato
- Librerie:
  - `WiFi.h` (inclusa con il core ESP32)
  - `HTTPClient.h` (inclusa con il core ESP32)
  - `ArduinoJson` (da installare tramite Library Manager)

---

## **Configurazione**

1. Copiare `config.h.sample` come `config.h`.  
2. Inserire le informazioni reali:
   - SSID e password della rete Wi-Fi  
   - Token del bot Telegram  
   - Chat ID destinatario dei messaggi  
   - Pin digitale da monitorare  
   - Mappe degli stati 0/1 (es: chiuso/aperto)  
   - Comando Telegram per interrogare lo stato (es: "stato")  

---

## **Uso**

1. Caricare il progetto sull’ESP32 tramite Arduino IDE.  
2. Aprire il monitor seriale per visualizzare debug e messaggi ricevuti.  
3. Interagire con il bot Telegram:
   - All’avvio, il bot riceverà un messaggio di conferma.  
   - Ogni cambio stato del pin digitale invierà un messaggio automatico.  
   - Inviare il comando configurato (`stato`) per ricevere lo stato attuale del pin.

---

## **Note tecniche**

- Polling Telegram ogni 500 ms; frequenza modificabile nel `loop()`.  
- Possibile estendere con interrupt hardware per rilevare i cambi di stato in tempo reale.  
- Facile da integrare in progetti più complessi con più pin o più comandi.  

---

## **Autore**
Progetto sviluppato come esempio per l’uso di ESP32 e Telegram Bot API.