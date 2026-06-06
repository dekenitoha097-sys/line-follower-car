#include "cmd.h"
#include "config.h"

void treatCmd(JSONVar msg)
{
    if (!msg.hasOwnProperty("commande"))
        return;
    String cmd = (const char *)msg["commande"];
    if (cmd == "change_mode")
    {
        if (!msg.hasOwnProperty("mode"))
            return;
        String mode = (const char *)msg["mode"];
        if (mode == "manuel")
        {
            modeActuel = MODE_MANUEL;
            setMoteurs(0, 0); // ← arrêt immédiat
        }
        else if (mode == "autonome")
            modeActuel = MODE_AUTONOME;
        else if (mode == "blockly")
            modeActuel = BLOCKLY;
    }

    if (cmd == "manuel_control" && modeActuel == MODE_MANUEL)
    {
        if (!msg.hasOwnProperty("direction"))
            return;
        String dir = (const char *)msg["direction"];

        if (dir == "avant")
            setMoteurs(VITESSE_BASE, VITESSE_BASE);
        else if (dir == "arriere")
            setMoteurs(-VITESSE_BASE, -VITESSE_BASE);
        else if (dir == "gauche")
            setMoteurs(-VITESSE_VIRAGE, VITESSE_VIRAGE);
        else if (dir == "droite")
            setMoteurs(VITESSE_VIRAGE, -VITESSE_VIRAGE);
        else if (dir == "stop")
            setMoteurs(0, 0);
    }
}