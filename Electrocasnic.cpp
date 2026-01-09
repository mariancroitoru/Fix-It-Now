#include "Electrocasnic.h"
#include <iostream>
#include <string>
using namespace std;

Electrocasnic::Electrocasnic(const string& _tip, const std::string& _marca, const std::string& _model, int _anFab, double _pretCat):tip(_tip), marca(_marca), model(_model), anFabricatie(_anFab), pretCatalog(_pretCat) {}

string Electrocasnic::getTip() const { 
    return tip; 
}
string Electrocasnic::getMarca() const { 
    return marca; 
}
string Electrocasnic::getModel() const { 
    return model; 
}
int Electrocasnic::getAnFabricatie() const { 
    return anFabricatie; 
}
double Electrocasnic::getPretCatalog() const {
     return pretCatalog; 
    }

int Electrocasnic::getVechime() const {

    int anulCurent = Data::dataCurenta().getAn();
    int vechime = anulCurent - anFabricatie;
    return (vechime < 0) ? 0 : vechime;
}

bool Electrocasnic::operator==(const Electrocasnic& _electrocasnic) const  {
    return (marca == _electrocasnic.marca && model ==_electrocasnic.model && tip ==_electrocasnic.tip);
}
