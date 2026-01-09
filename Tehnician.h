#pragma once
#include "Angajat.h"
#include <map>
#include <set>
#include <vector>

class Tehnician : public Angajat {

    std::map<std::string, std::set<std::string>> specializari; 
    
    std::vector<int> idCereriActive; 
    double valoareaTotalaReparatii;
    double durataTotalaLucrata;  

    static constexpr double BONUS_REPARATII_PROCENT = 2.0;
    static constexpr int MAX_CERERI_ACTIVE = 3;

public:
        Tehnician() = default;
        Tehnician(const std::string& _nume, const std::string& _prenume, const std::string& _cnp, const Data& _dataAngajarii, const std::string& _orasDomiciliu);

        // Specializari
        void adaugaSpecializare(const std::string& tip, const std::string& marca);
        bool areSpecializare(const std::string& tip, const std::string& marca) const;

        // Gestiune Cereri Active
        bool poatePrimiCerere() const;
        void adaugaIdCerereActiva(int id);
        void finalizareCerere(int idCerere, double valoareReparatie, int durataEfectiva);

        // Getteri pentru alocare automata 
        double getDurataTotalaLucrata() const;
        int getNrCereriActive() const ;

        // Salariu si Afisare
        double calculeazaSalariu() const override;
        void afisare(std::ostream& ) const override;
        std::string getTipAngajat() const override;
};