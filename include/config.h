#ifndef CONFIG_H
#define CONFIG_H

#include <WiFiManager.h>


// ================= PARAMS =================
extern int VITESSE_BASE;
extern int VITESSE_VIRAGE;
extern int SEUIL_LIGNE;

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
#define freqPWM       20000
#define resolutionPWM 8
#define canalENA      0
#define canalENB      1

extern WiFiManager wm;

enum Mode
{
    NOTHING,
    MODE_AUTONOME,
    MODE_MANUEL,
    BLOCKLY
};
extern Mode modeActuel;

void setMoteurs(int vG, int vD);

#endif