#include <Arduino.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>
#include <Arduino_JSON.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include "cmd.h"
#include "config.h"

// ================= CAPTEURS =================
#define capteurGauche 35
#define capteurDroite 34
// ================= MOTEURS =================
#define IN1 27
#define IN2 26
#define ENA 14
#define IN3 25
#define IN4 33
#define ENB 12
// ================= PWM =================
#define freqPWM 20000
#define resolutionPWM 8
#define canalENA 0
#define canalENB 1
// ================= PARAMS =================

#define SEUIL_LIGNE 2000


Mode modeActuel = MODE_AUTONOME;

WebSocketsServer ws = WebSocketsServer(81);
DNSServer dnsServer;

// Gestion des événements WebSocket
void onWsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
    Serial.printf("[WS] Client %d connecté\n", num);
    ws.sendTXT(num, "{\"status\":\"connecte\",\"ip\":\"" + WiFi.localIP().toString() + "\"}");
    break;

  case WStype_DISCONNECTED:
    Serial.printf("[WS] Client %d déconnecté\n", num);
    break;

  case WStype_TEXT:
  {
    JSONVar msg = JSON.parse((char *)payload);
    if (JSON.typeof(msg) == "undefined")
      break;
    treatCmd(msg);
    break;
  }
  }
}

// Envoie des données au client WebSocket toutes les 150ms
void sendData()
{
  int valeur = 42; // exemple de valeur à envoyer

  String json = "{\"valeur\":" + String(42) + ", \"mode\":" + String(modeActuel) + "}";
  
  ws.broadcastTXT(json);
}

void demarrerWebSocket()
{
  ws.begin();
  ws.onEvent(onWsEvent);
  Serial.println("WebSocket démarré → ws://" + WiFi.localIP().toString() + ":81");
}

// ================= WIFI =================
void lancerConfigWifi()
{
  WiFiManager wm;
  // wm.resetSettings(); // décommenter pour forcer le portail à chaque fois
  // wm.resetSettings(); // force le portail à CHAQUE démarrage

  bool connecte = wm.autoConnect("RobotConfig");

  if (connecte)
  {
    Serial.println("WiFi connecté ! IP : " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println("Échec configuration, redémarrage...");
    delay(3000);
    ESP.restart();
  }
}

// ================= PWM INIT =================
void pwmSetup()
{
  ledcSetup(canalENA, freqPWM, resolutionPWM);
  ledcSetup(canalENB, freqPWM, resolutionPWM);
  ledcAttachPin(ENA, canalENA);
  ledcAttachPin(ENB, canalENB);
}

// ================= MOTEURS =================
void setMoteurs(int vG, int vD)
{
  if (vG >= 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    vG = -vG;
  }

  if (vD >= 0)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    vD = -vD;
  }

  ledcWrite(canalENA, constrain(vG, 0, 255));
  ledcWrite(canalENB, constrain(vD, 0, 255));
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200); // EN PREMIER toujours
  delay(1000);          // attendre que Serial soit prêt

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pwmSetup();

  lancerConfigWifi();  // après Serial
  demarrerWebSocket(); // après WiFi
  // Dans setup(), après WiFi.softAP()
  dnsServer.start(53, "robotPiste", WiFi.localIP());
  MDNS.begin("robotPiste");
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}

// ================= LOOP =================
void loop()
{
  dnsServer.processNextRequest();
  ws.loop();

  sendData();
  int valG = analogRead(capteurGauche);
  int valD = analogRead(capteurDroite);

  bool ligneG = valG > SEUIL_LIGNE;
  bool ligneD = valD > SEUIL_LIGNE;

  Serial.printf("G:%4d D:%4d | ligneG:%d ligneD:%d\n", valG, valD, ligneG, ligneD);

  if (modeActuel == MODE_MANUEL)
  {

  }

  if (modeActuel == MODE_AUTONOME)
  {
    if (ligneG && ligneD)
      setMoteurs(VITESSE_BASE, VITESSE_BASE);
    else if (ligneG && !ligneD)
      setMoteurs(-VITESSE_VIRAGE, VITESSE_VIRAGE);
    else if (!ligneG && ligneD)
      setMoteurs(VITESSE_VIRAGE, -VITESSE_VIRAGE);
    else
      setMoteurs(VITESSE_BASE, VITESSE_BASE);
  }

  if(modeActuel == BLOCKLY)
  {
    // à compléter avec les commandes Blockly
  }

  delay(10);
}