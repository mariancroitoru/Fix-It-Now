#include "CerereReparatie.h"
#include <iomanip>
#include <cstring>
#include <stdexcept> 

using namespace std;

int CerereReparatie::nextID = 0;


CerereReparatie::CerereReparatie(unique_ptr<Electrocasnic> _aparat, const time_t _timestamp, int _nivelComplexitate):id(nextID++),aparat(std::move(_aparat)),timestamp(_timestamp),nivelComplexitate(_nivelComplexitate),stare(StareCerere::IN_ASTEPTARE),idTehnician(-1){
    
    if (!aparat) {
        throw std::invalid_argument("Eroare Cerere: Aparatul nu este creat!");
    }

    if (nivelComplexitate < 0 || nivelComplexitate > 5) {
        throw std::out_of_range("Eroare Cerere: Complexitatea trebuie sa fie intre 0 si 5!");
    }


    if (nivelComplexitate == 0) {
        durataEstimata = 0;
        pretReparatie = 0;
        durataRamasa = 0;
        stare = StareCerere::INVALIDA;
    } else {
        int vechime = aparat->getVechime();
        if (vechime == 0) 
            vechime = 1; 
        durataEstimata = vechime * nivelComplexitate;
        
        pretReparatie = aparat->getPretCatalog() * durataEstimata;
        
        durataRamasa = durataEstimata;
    }
}

CerereReparatie::CerereReparatie(const CerereReparatie& other): id(other.id), timestamp(other.timestamp),nivelComplexitate(other.nivelComplexitate),durataEstimata(other.durataEstimata),durataRamasa(other.durataRamasa),pretReparatie(other.pretReparatie), stare(other.stare), idTehnician(other.idTehnician){
    if (other.aparat) {
        aparat = other.aparat->cloneaza();
    } else {
        aparat = nullptr;
    }
}

CerereReparatie& CerereReparatie::operator=(const CerereReparatie& other) {
    if (this == &other) 
        return *this;

    id = other.id;
    
    timestamp = other.timestamp;
    nivelComplexitate = other.nivelComplexitate;
    durataEstimata = other.durataEstimata;
    durataRamasa = other.durataRamasa;
    pretReparatie = other.pretReparatie;
    stare = other.stare;
    idTehnician = other.idTehnician;

    if (other.aparat) {
        aparat = other.aparat->cloneaza();
    } else {
        aparat.reset();
    }

    return *this;
}


void CerereReparatie::proceseaza() {
    if (durataRamasa > 0) {
        durataRamasa--;
    }
    
    if (durataRamasa == 0 && stare == StareCerere::IN_LUCRU) {
        stare = StareCerere::FINALIZATA;
    }
}

bool CerereReparatie::esteFinalizata() const {
    return stare == StareCerere::FINALIZATA;
}

bool CerereReparatie::esteValida() const {
    return (aparat != nullptr) && (stare != StareCerere::INVALIDA);
}


string CerereReparatie::getTimeStampString() const {
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    
    if (timeinfo) {
        strftime(buffer, 80, "%d.%m.%Y %H:%M:%S", timeinfo);
    } else {
        return "Data Invalida";
    }
    return string(buffer);
}

void CerereReparatie::afisare(std::ostream& dev) const {
    dev << "Cerere #" << id << " | Data: " << getTimeStampString() << endl;
    
    string s;
    switch (stare) {
        case StareCerere::IN_ASTEPTARE: s = "IN ASTEPTARE"; break;
        case StareCerere::REPARTIZATA: s = "REPARTIZATA"; break;
        case StareCerere::IN_LUCRU: s = "IN LUCRU"; break;
        case StareCerere::FINALIZATA: s = "FINALIZATA"; break;
        case StareCerere::INVALIDA: s = "INVALIDA"; break;
    }
    dev << "Status: " << s << endl;

    if (aparat) {
        aparat->afisare(dev);
    }

    if (stare != StareCerere::INVALIDA) {
        dev << "Complexitate: " << nivelComplexitate << "/5" << endl;
        dev << "Estimare: " << durataEstimata << "h | Pret: " << fixed << setprecision(2) << pretReparatie << " RON" << endl;
        
        if (stare == StareCerere::IN_LUCRU || stare == StareCerere::FINALIZATA) {
            dev << "Tehnician ID: " << idTehnician << " | Timp Ramas: " << durataRamasa << endl;
        }
    } else {
        dev << "MENTIUNE: Aparat ireparabil." << endl;
    }
    dev << "-----------------------------------" << endl;
}

bool CerereReparatie::operator<(const CerereReparatie& other) const {
    return this->timestamp < other.timestamp;
}

// --- GETTERI ---
int CerereReparatie::getId() const {
    return id; 
}
int CerereReparatie::getNivelComplexitate() const { 
    return nivelComplexitate; 
}
const Electrocasnic* CerereReparatie::getAparat() const {
     return aparat.get(); 
    }
time_t CerereReparatie::getTimeStamp() const { 
    return timestamp; 
}
int CerereReparatie::getDurataEstimata() const { 
    return durataEstimata; 
}
int CerereReparatie::getDurataRamasa() const { 
    return durataRamasa; 
}
double CerereReparatie::getPretReparatie() const {
     return pretReparatie; 
    }
StareCerere CerereReparatie::getStare() const {
     return stare; 
    }
int CerereReparatie::getIdTehnician() const {
     return idTehnician; 
    }

// --- SETTERI ---
void CerereReparatie::setStare(const StareCerere s) { 
    stare = s; 
}
void CerereReparatie::setIdTehnician(int id) { 
    idTehnician = id; 
}