#pragma once
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>

#include "Angajat.h"
#include "Receptioner.h"
#include "Tehnician.h"
#include "Supervizor.h"
#include "Electrocasnic.h"
#include "CerereReparatie.h"
#include "AngajatFactory.h"
#include "ElectrocasnicFactory.h"

class ServiceManager {
private:
    // Singleton instance
    static ServiceManager* instance;
    
    // Constructor privat pentru Singleton
    ServiceManager();
    
    // Colecții principale
    std::vector<std::unique_ptr<Angajat>> angajati;
    
    // Catalog: map<tip, map<marca, map<model, unique_ptr<Electrocasnic>>>>
    std::map<std::string, std::map<std::string, std::map<std::string, std::unique_ptr<Electrocasnic>>>> catalogElectrocasnice;
    
    // Cereri
    std::vector<std::unique_ptr<CerereReparatie>> toateCererile; // toate cererile primite
    std::vector<CerereReparatie*> cereriActive; // pointeri la cereri in lucru
    std::queue<CerereReparatie*> cereriInAsteptare;
    std::vector<CerereReparatie*> cereriFinalzate;
    
    // Pentru statistici
    std::map<std::string, int> aparateNereparabile; // "tip_marca_model" -> nr_aparitii
    
    // Simulare
    int timpCurent;
    bool esteInSimulare;

public:
    // Singleton
    static ServiceManager* getInstance();
    static void deleteInstance();
    
    // Interzice copierea
    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;
    
    ~ServiceManager();
    
    // === GESTIUNE ANGAJAȚI ===
    
    // Verificări
    bool arePersonalMinim() const;
    int getNumarTehnicieni() const;
    int getNumarReceptioneri() const;
    int getNumarSupervizori() const;
    
    // CRUD
    void adaugaAngajat(std::unique_ptr<Angajat> angajat);
    bool stergeAngajat(const std::string& cnp);
    bool modificaNumeAngajat(const std::string& cnp, const std::string& numeNou, const std::string& prenumeNou);
    Angajat* cautaAngajatDupaCNP(const std::string& cnp);
    Angajat* cautaAngajatDupaID(int id);
    
    // Afișare
    void afiseazaAngajat(const std::string& cnp) const;
    void afiseazaTotiAngajatii() const;
    
    // Încărcare din fișier
    void incarcaAngajatiDinCSV(const std::string& numeFisier);
    
    // === GESTIUNE ELECTROCASNICE ===
    
    // CRUD Catalog
    void adaugaModelInCatalog(std::unique_ptr<Electrocasnic> electrocasnic);
    bool stergeModelDinCatalog(const std::string& tip, const std::string& marca, const std::string& model);
    
    // Verificări
    bool poateFiReparat(const std::string& tip, const std::string& marca, const std::string& model) const;
    const Electrocasnic* getDetaliiModel(const std::string& tip, const std::string& marca, const std::string& model) const;
    
    // Afișare
    void afiseazaCatalogComplet() const;
    void afiseazaAparateNereparabile() const;
    
    // Încărcare
    void incarcaCatalogDinCSV(const std::string& numeFisier);
    
    // === GESTIUNE CERERI ===
    
    // Primire cereri
    void primesteCerere(std::unique_ptr<CerereReparatie> cerere);
    void incarcaCereriDinCSV(const std::string& numeFisier);
    
    // Verificare validitate
    bool esteCerereValida(const CerereReparatie& cerere);
    
    // === SIMULARE ===
    
    // Alocare automată
    Tehnician* gasesteTehnicianPotrivit(const CerereReparatie& cerere);
    void alocaCerere(CerereReparatie* cerere, Tehnician* tehnician);
    void procesareCereriInAsteptare();
    
    // Simulare în timp real
    void startSimulare();
    void executeazaTic();
    void afiseazaStatusTic() const;
    void stopSimulare();
    
    // Finalizare cereri
    void finalizeazaCerere(CerereReparatie* cerere);
    
    // === RAPORTĂRI ===
    
    void genereazaRaportTop3Salarii(const std::string& numeFisier) const;
    void genereazaRaportCeaMaiLungaReparatie(const std::string& numeFisier) const;
    void genereazaRaportCereriInAsteptare(const std::string& numeFisier) const;
    
    // === UTILITĂȚI ===
    
    void afiseazaStatistici() const;
    void reset(); // pentru testare
};