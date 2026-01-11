#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Angajat.h"
#include "Tehnician.h"

class AngajatiManager {
private:
    std::vector<std::unique_ptr<Angajat>> angajati;

public:
    AngajatiManager() = default;

    void incarcaDinCSV(const std::string& numeFisier);

    Tehnician* gasesteTehnicianDisponibil(const std::string& tipAparat, const std::string& marcaAparat);



    void adaugaAngajat(std::unique_ptr<Angajat> a);
    bool stergeAngajat(const std::string& cnp);
    bool modificaNume(const std::string& cnp, const std::string& numeNou, const std::string& prenumeNou);
    
    Angajat* cautaDupaCNP(const std::string& cnp);
    Angajat* cautaDupaID(int id);




    bool verificaPersonalMinim() const; // 3 Tech, 1 Rec, 1 Sup

    bool adaugaAngajatManual(std::string tip, std::string nume, std::string prenume, std::string cnp, std::string data, std::string oras);
    
    void afiseazaToti() const;
    

    std::vector<Angajat*> getTop3Salarii() const;

    void genereazaRaportTop3Salarii(const std::string& numeFisier) const;
    Tehnician* getTehnicianCeaMaiLungaReparatie() const;


};