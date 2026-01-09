#include "Angajat.h"
#include "Data.h"
#include "CNPValidator.h"

#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

int Angajat::nextID=1000;



Angajat::Angajat(const string& _nume, const string& _prenume, const string& _cnp,const Data& _dataAngajarii, const string& _orasDomiciliu): id(nextID++),nume(_nume), prenume(_prenume), cnp(_cnp), dataAngajarii(_dataAngajarii), orasDomiciliu(_orasDomiciliu){

        validareNume(_nume, "Nume");
        validareNume(_prenume, "Prenume");

        if (!CNPValidator::esteValid(_cnp)) {
            throw std::invalid_argument("CNP invalid: " + _cnp);
        }

        // verificare varsta daca are minim 16 ani 
        Data dataNasterii = CNPValidator::getDataNasterii(_cnp);
        
        int varstaLaAngajare = _dataAngajarii.getAn() - dataNasterii.getAn();
        if (_dataAngajarii.getLuna() < dataNasterii.getLuna() || (_dataAngajarii.getLuna() == dataNasterii.getLuna() && _dataAngajarii.getZi() < dataNasterii.getZi())) {
            varstaLaAngajare--;
        }

        if (varstaLaAngajare < 16) {
            throw std::invalid_argument("Persoana trebuie sa aiba minim 16 ani la data angajarii.");
        }
}



//getterii

int Angajat::getId() const{
    return id;
}

string Angajat::getNume() const{
    return nume;
}
 string Angajat::getPrenume() const{
    return prenume;
 }

 string Angajat::getCNP() const{
    return cnp;
 }
 Data Angajat::getDataAngajarii() const{
    return dataAngajarii;
 }

 string Angajat::getOrasDomiciliu() const{
    return orasDomiciliu;
 }


// --- VALIDĂRI ---

void Angajat::validareNume(const std::string& valoare, const std::string& tipNume) const {
    if (valoare.length() < 3 || valoare.length() > 30) {
        throw std::invalid_argument(tipNume + " trebuie sa aiba intre 3 si 30 caractere."); 
    }
}

// --- SETTERI (Permit modificarea numelui, de ex. la căsătorie) 

void Angajat::setNume(const std::string& _nume) {
    validareNume(_nume, "Nume");
    nume = _nume;
}

void Angajat::setPrenume(const std::string& _prenume) {
    validareNume(_prenume, "Prenume");
    prenume = _prenume;
}

// --- AUXILIARE SALARIU ---

double Angajat::calculeazaBonusFidelitate() const {

    Data azi = Data::dataCurenta();
    int aniLucrati = azi.getAn() - dataAngajarii.getAn();
    
    if (azi.getLuna() < dataAngajarii.getLuna() || (azi.getLuna() == dataAngajarii.getLuna() && azi.getZi() < dataAngajarii.getZi())) {
        aniLucrati--;
    }
    // etape-- cat de smecher e omu 
    int etape = aniLucrati / ANI_FIDELITATE;
    return etape * (BONUS_DE_FIDELITARE_PROCENT / 100.0) * SALARIU_BAZA;
}

bool Angajat::primestePrimaTransport() const {
    return orasDomiciliu != "Bucuresti";
}

// --- AFIȘARE ---

void Angajat::afisare(std::ostream& dev) const {
    dev << "ID: " << id << " | "
        << "Nume: " << nume << " " << prenume << " | "
        << "CNP: " << cnp << " | "
        << "Oras: " << orasDomiciliu << " | "
        << "Data Angajare: " << dataAngajarii;
        
}