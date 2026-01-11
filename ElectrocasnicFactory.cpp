#include "ElectrocasnicFactory.h"
#include "Frigider.h"
#include "MasinaDeSpalat.h"
#include "TV.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

vector<string> ElectrocasnicFactory::splitLine(const string& linie, char delimitator) {
    vector<string> tokens;
    stringstream ss(linie);
    string token;
    while (getline(ss, token, delimitator)) {
        if (!token.empty() && token.back() == '\r') {
            token.pop_back();
        }
        tokens.push_back(token);
    }
    return tokens;
}


unique_ptr<Electrocasnic> ElectrocasnicFactory::creeazaDinCSV(const string& linie) {
    if (linie.empty()) 
        return nullptr;

    vector<string> tokens = splitLine(linie, ',');

    if (tokens.size() < 5) {
        throw invalid_argument("Linie incompleta (lipsesc date de baza): " + linie);
    }

    string tip = tokens[0];
    string marca = tokens[1];
    string model = tokens[2];
    
    int an;
    double pret;
    try {
        an = stoi(tokens[3]);
        pret = stod(tokens[4]);
    } catch (...) {
        throw invalid_argument("Eroare conversie numar (An/Pret) in linia: " + linie);
    }

    if (tip == "Frigider") {
        if (tokens.size() < 6) throw invalid_argument("Frigiderul necesita specificarea congelatorului (1/0).");
        
        bool areCongelator = (stoi(tokens[5]) != 0); 
        return make_unique<Frigider>(marca, model, an, pret, areCongelator);
    }
    else if (tip == "MasinaDeSpalat") {
        if (tokens.size() < 6) throw invalid_argument("Masina de spalat necesita capacitatea.");
        
        double capacitate = stod(tokens[5]);
        return make_unique<MasinaDeSpalat>(marca, model, an, pret, capacitate);
    }
    else if (tip == "TV") {
        if (tokens.size() < 7) throw invalid_argument("TV necesita diagonala si unitatea de masura (1=cm/0=inch).");
        
        double diagonala = stod(tokens[5]);
        bool esteInCm = (stoi(tokens[6]) != 0);
        return make_unique<TV>(marca, model, an, pret, diagonala, esteInCm);
    }
    else {
        throw invalid_argument("Tip electrocasnic necunoscut: " + tip);
    }
}