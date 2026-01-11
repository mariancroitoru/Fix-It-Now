#pragma once
#include "ServiceManager.h"

class Meniu {
private:
    ServiceManager* service; 

    void clearScreen();
    void pause();
    
    void afiseazaTitlu();
    void meniuGestiuneAngajati();
    void meniuGestiuneElectrocasnice();
    void meniuProcesareCereri();
    void meniuRaportari();

public:
    Meniu();
    void ruleazaAplicatie();
};