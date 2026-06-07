#include "position.h"
#include "config.h"
#include <math.h>

float posX  = 0.0;
float posY  = 0.0;
float angle = 0.0; // 0° = tout droit, + = droite, - = gauche

// Distance avancée par tick (à calibrer selon ta voiture)
#define DIST_TICK       0.5   // cm par appel
#define ANGLE_VIRAGE    15.0  // degrés par tick en virage

void updatePosition(bool ligneG, bool ligneD) {
    float deltaAngle = 0.0;
    float deltaDist  = 0.0;

    if (ligneG && ligneD) {
        // Tout droit
        deltaDist  = DIST_TICK;
        deltaAngle = 0.0;
    }
    else if (ligneG && !ligneD) {
        // Virage gauche
        deltaDist  = DIST_TICK * 0.5;
        deltaAngle = -ANGLE_VIRAGE;
    }
    else if (!ligneG && ligneD) {
        // Virage droite
        deltaDist  = DIST_TICK * 0.5;
        deltaAngle = +ANGLE_VIRAGE;
    }
    else {
        // Ligne perdue → continue tout droit
        deltaDist  = DIST_TICK;
        deltaAngle = 0.0;
    }

    angle += deltaAngle;

    // Conversion polaire → cartésien
    float rad = angle * M_PI / 180.0;
    posX += deltaDist * cos(rad);
    posY += deltaDist * sin(rad);
}

void resetPosition() {
    posX  = 0.0;
    posY  = 0.0;
    angle = 0.0;
}