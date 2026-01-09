#include "Frigider.h"
#include <string>
#include <memory>
#include <iostream>
using namespace std;


Frigider::Frigider(const string & _marca, const string & _model, int _anFab, double _pretCat, bool _areCongelator ):Electrocasnic("Frigider",_marca,_model,_anFab,_pretCat),areCongelator(_areCongelator){}

bool Frigider::getAreCongelator() const{
    return areCongelator;
}

void Frigider::afisare (std::ostream& dev) const{
    dev << "Tip: " << tip << "|"
        << "Marca: " << marca << " | "
        << "Model: " << model << " | "
        << "An: " << anFabricatie << " | "
        << "Pret: " << pretCatalog << " RON | "
        << "Congelator: " << (areCongelator ? "DA" : "NU") <<endl;

}


unique_ptr<Electrocasnic> Frigider::cloneaza() const {
     return make_unique<Frigider>(*this);
}