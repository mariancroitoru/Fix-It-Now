#include "Receptioner.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;

Receptioner::Receptioner(const string& _nume, const string& _prenume, const string& _cnp, const Data& _dataAngajarii, const string& _orasDomiciliu):Angajat(_nume,_prenume,_cnp,_dataAngajarii,_orasDomiciliu){}


void Receptioner::adaugaCerere(int id) {
    idCereriInregistrate.push_back(id); 
}

const vector<int>& Receptioner::getCereri() const {
    return idCereriInregistrate;
}

double Receptioner::calculeazaSalariu() const {

    double salariuTotal = SALARIU_BAZA;

    salariuTotal += calculeazaBonusFidelitate();

    if (primestePrimaTransport()) {
        salariuTotal += PRIMA_TRANS;
    }

    return salariuTotal;
}

string Receptioner::getTipAngajat() const {
    return "Receptioner";
}

void Receptioner::afisare(ostream& dev) const {
    
    Angajat::afisare(dev);
    dev << " | Rol: " << getTipAngajat()
        << " | Cereri inregistrate: " << idCereriInregistrate.size()
        << " | Salariu curent: " <<std::fixed<<setprecision(2)<< calculeazaSalariu() << " RON" << endl;
}