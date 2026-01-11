#pragma once
#include "AngajatiManager.h"
#include "ElectrocasniceManager.h"
#include "CerereManager.h"

class ServiceManager {
private:
    ServiceManager() = default; 

    AngajatiManager angajatiManager;
    ElectrocasniceManager electrocasniceManager;
    CerereManager cerereManager;

    int timpCurent = 0;

public:
    static ServiceManager& getInstance() {
        static ServiceManager instance;
        return instance;
    }

    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;


    void incarcaDate(const std::string& fAng, const std::string& fEl, const std::string& fCer);
    
    // Simulare
    void runSimulare();

    // Getteri
    AngajatiManager& getAngajatiManager() { return angajatiManager; }
    ElectrocasniceManager& getElectrocasniceManager() { return electrocasniceManager; }
    CerereManager& getCerereManager() { return cerereManager; }

private:
    void proceseazaTic();
    void incearcaAlocareCereri();
};