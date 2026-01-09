#include "Supervizor.h"
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>



using namespace std;


Supervizor::Supervizor(const string& _nume, const string& _prenume, const string& _cnp, const Data& _dataAngajarii, const string& _orasDomiciliu ): Angajat(_nume,_prenume,_cnp,_dataAngajarii, _orasDomiciliu){}

double Supervizor::calculeazaSalariu() const{
    double salariu= SALARIU_BAZA + SALARIU_BAZA*(0.20);

     salariu += calculeazaBonusFidelitate();

    //  prima de transport de 400 RON daca nu e din Bucuresti 
    if (primestePrimaTransport()) {
        salariu += PRIMA_TRANS;
    }

    return salariu;

}

string Supervizor::getTipAngajat() const{
    return "Supervizor";
}

void Supervizor::afisare(ostream& dev) const{

    Angajat::afisare(dev);
    dev<< "|  Rol: "<<getTipAngajat()
       <<"| Salariu curent: "<<std::fixed<<setprecision(2)<<calculeazaSalariu()<<"RON"<<endl;
}