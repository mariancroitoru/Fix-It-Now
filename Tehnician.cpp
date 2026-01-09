#include "Tehnician.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;



Tehnician::Tehnician(const string& _nume, const string& _prenume, const string& _cnp, const Data& _dataAngajarii, const string& _orasDomiciliu): Angajat(_nume, _prenume, _cnp, _dataAngajarii, _orasDomiciliu),valoareaTotalaReparatii(0.0), durataTotalaLucrata(0.0) {}

void Tehnician::adaugaSpecializare(const string& tip, const string& marca) {
    specializari[tip].insert(marca); // Folosim set pentru a evita duplicatele plus ca avem in ordine alfabetica!
}

bool Tehnician::areSpecializare(const string& tip, const string& marca) const {
    auto it = specializari.find(tip);
    if (it != specializari.end()) {
        return it->second.count(marca) > 0;
    }
    return false;
}

bool Tehnician::poatePrimiCerere() const {
    return idCereriActive.size() < MAX_CERERI_ACTIVE;
}

void Tehnician::adaugaIdCerereActiva(int id) {
    if (poatePrimiCerere()) {
        idCereriActive.push_back(id);
    }
}

void Tehnician::finalizareCerere(int idCerere, double valoareReparatie, int durataEfectiva) {
    auto it = find(idCereriActive.begin(), idCereriActive.end(), idCerere);
    if (it != idCereriActive.end()) {
        idCereriActive.erase(it);
        valoareaTotalaReparatii += valoareReparatie; // Pentru bonusul de 2% 
        durataTotalaLucrata += durataEfectiva;      // Pentru criteriul de incarcare echilibrata 
    }
}

double Tehnician::getDurataTotalaLucrata() const {
    return durataTotalaLucrata;
}

int Tehnician::getNrCereriActive() const {
    return static_cast<int>(idCereriActive.size());
}

double Tehnician::calculeazaSalariu() const {
    //  (4000) + fidelitatea (+5% la 3 ani) 
    double salariu = SALARIU_BAZA + calculeazaBonusFidelitate();

    //  prima de transport de 400 RON daca nu e din Bucuresti 
    if (primestePrimaTransport()) {
        salariu += PRIMA_TRANS;
    }

    // Tehnicienii primesc bonus 2% din valoarea reparatiilor efectuate
    salariu += (BONUS_REPARATII_PROCENT / 100.0) * valoareaTotalaReparatii;

    return salariu;
}

string Tehnician::getTipAngajat() const {
    return "Tehnician";
}

void Tehnician::afisare(ostream& dev) const {
   
    Angajat::afisare(dev); 
    dev << " | Rol: " << getTipAngajat() 
        << " | Experienta: " << durataTotalaLucrata << " unitati de timp"  // adica cat timp a lucrat
        << " | Cereri active: " << idCereriActive.size()
        << " | Salariu curent: " << std::fixed<<setprecision(2) << calculeazaSalariu() << " RON" << endl;
}
