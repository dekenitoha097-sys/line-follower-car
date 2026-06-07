#include "sendData.h"
#include "config.h"
#include <Arduino_JSON.h>
#include <WebSocketsServer.h>
#include "position.h"

int valG = 0;
int valD = 0;

extern WebSocketsServer ws;

void sendData(bool ligneG, bool ligneD)
{
    updatePosition(ligneG, ligneD);
    valG = analogRead(capteurGauche);
    valD = analogRead(capteurDroite);

    JSONVar data;
    data["valG"] = valG;
    data["valD"] = valD;
    data["ligneG"] = ligneG;
    data["ligneD"] = ligneD;
    data["mode"] = modeActuel;
    data["ip"] = WiFi.localIP().toString();
    data["rssi"] = WiFi.RSSI();
    data["uptime"] = millis() / 1000; // secondes depuis démarrage
    data["heap"] = ESP.getFreeHeap(); // mémoire libre en bytes
    data["vitesseBase"]   = VITESSE_BASE;
    data["x"] = posX;
    data["y"] = posY;
    data["angle"] = angle;

    String payload = JSON.stringify(data);
    ws.broadcastTXT(payload);
}