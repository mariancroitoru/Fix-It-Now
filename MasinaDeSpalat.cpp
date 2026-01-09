#include "MasinaDeSpalat.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

MasinaDeSpalat::MasinaDeSpalat(const string& _marca, const string& _model, int _anFab, double _pretCat, double _capacitate): Electrocasnic("Masina de spalat",_marca, _model,_anFab, _pretCat),capacitate(_capacitate){}

double MasinaDeSpalat::getCapacitate() const{
    return capacitate;
}

void MasinaDeSpalat::afisare(ostream& dev) const{
    dev << "Tip: " << tip << "|"
        << "Marca: " << marca << " | "
        << "Model: " << model << " | "
        << "An: " << anFabricatie << " | "
        << "Pret: " << pretCatalog << " RON | "
        <<"Capacitate: "<<capacitate<<endl;
}

unique_ptr<Electrocasnic> MasinaDeSpalat::cloneaza() const{
     return make_unique<MasinaDeSpalat>(*this);
}

