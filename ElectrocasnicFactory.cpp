#include "ElectrocasnicFactory.h"

// Includem clasele concrete
#include "Frigider.h"
#include "TV.h"
#include "MasinaDeSpalat.h"

#include <stdexcept>
#include <iostream>

using namespace std;

unique_ptr<Electrocasnic> ElectrocasnicFactory::creeazaElectrocasnic(const string& tip, const string& marca,const string& model,int anFabricatie, double pretCatalog,const map<string, string>& dateSpecifice) 
{
    if (tip == "Frigider") {
        if (dateSpecifice.find("congelator") == dateSpecifice.end()) {
             throw invalid_argument("Eroare Factory: Lipseste specificatia 'congelator' pentru Frigider!");
        }

        string val = dateSpecifice.at("congelator");
        bool areCongelator = (val == "da" || val == "DA" || val == "Da");
        
        return make_unique<Frigider>(marca, model, anFabricatie, pretCatalog, areCongelator);
    }
    
    else if (tip == "TV") {
        if (dateSpecifice.find("diagonala") == dateSpecifice.end()) {
            throw invalid_argument("Eroare Factory: Lipseste specificatia 'diagonala' pentru TV!");
        }

        double diagonala = stod(dateSpecifice.at("diagonala")); 

        bool esteInCm = true;
        if (dateSpecifice.find("unitateDiagonala") != dateSpecifice.end()) {
            string unitate = dateSpecifice.at("unitateDiagonala");
            if (unitate == "inch" || unitate == "in") {
                esteInCm = false;
            }
        }
        
        return make_unique<TV>(marca, model, anFabricatie, pretCatalog, diagonala, esteInCm);
    }
    
    else if (tip == "MasinaDeSpalat") {
        if (dateSpecifice.find("capacitate") == dateSpecifice.end()) {
            throw invalid_argument("Eroare Factory: Lipseste specificatia 'capacitate' pentru Masina de Spalat!");
        }

        double capacitate = stod(dateSpecifice.at("capacitate"));
        
        return make_unique<MasinaDeSpalat>(marca, model, anFabricatie, pretCatalog, capacitate);
    }
    
    else {
        throw invalid_argument("Eroare Factory: Tip electrocasnic necunoscut: " + tip);
    }
}

map<string, string> ElectrocasnicFactory::parseazaDateSpecifice(const string& linie) {
    map<string, string> rezultat;
    size_t start = 0;
    size_t end = 0;

    while ((end = linie.find(';', start)) != string::npos) {
        string pereche = linie.substr(start, end - start);
        size_t splitPos = pereche.find(':');
        if (splitPos != string::npos) {
            rezultat[pereche.substr(0, splitPos)] = pereche.substr(splitPos + 1);
        }
        start = end + 1;
    }

    if (start < linie.length()) {
        string pereche = linie.substr(start);
        size_t splitPos = pereche.find(':');
        if (splitPos != string::npos) {
            rezultat[pereche.substr(0, splitPos)] = pereche.substr(splitPos + 1);
        }
    }
    return rezultat;
}