#ifndef CONFIG_H
#define CONFIG_H

#define VITESSE_BASE   200  // ← ajouter
#define VITESSE_VIRAGE 200  // ← ajouter


enum Mode { MODE_AUTONOME, MODE_MANUEL, BLOCKLY };
extern Mode modeActuel;

void setMoteurs(int vG, int vD); 

#endif