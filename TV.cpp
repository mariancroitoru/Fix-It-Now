#include "TV.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;


TV::TV(const string& _marca, const string& _model, int _anFab, double _pretCat, double _diagonala, bool _esteInCm): Electrocasnic("TV",_marca,_model,_anFab,_pretCat), diagonala(_diagonala), esteInCm(_esteInCm){}

double TV::getDiagonala() const{
    return diagonala;
}

bool TV::getEsteInCm() const{
    return esteInCm;
}

void TV::afisare(ostream& dev) const{
    dev << "Tip: " << tip << "|"
        << "Marca: " << marca << " | "
        << "Model: " << model << " | "
        << "An: " << anFabricatie << " | "
        << "Pret: " << pretCatalog << " RON | "
        <<"Diagonala: "<<diagonala<<(esteInCm ? " cm": " inch")<<endl;
}


unique_ptr<Electrocasnic> TV::cloneaza() const{
     return make_unique<TV>(*this);
}

