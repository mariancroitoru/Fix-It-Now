#include "AngajatFactory.h"
#include "Receptioner.h"
#include "Tehnician.h"
#include "Supervizor.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

vector<string> AngajatFactory::splitLine(const string& linie, char delimitator) {
    vector<string> tokens;
    stringstream ss(linie);
    string token;
    while (getline(ss, token, delimitator)) {
        // Curatam eventualele caractere invizibile (\r) ramase de la Windows
        if (!token.empty() && token.back() == '\r') {
            token.pop_back();
        }
        tokens.push_back(token);
    }
    return tokens;
}

unique_ptr<Angajat> AngajatFactory::creeazaDinCSV(const string& linie) {
    if (linie.empty()) return nullptr;

    vector<string> tokens = splitLine(linie, ',');

    if (tokens.size() < 6) {
        throw std::invalid_argument("Linie incompleta (prea putine coloane): " + linie);
    }

    string tip = tokens[0];
    string nume = tokens[1];
    string prenume = tokens[2];
    string cnp = tokens[3];
    string dataStr = tokens[4];
    string oras = tokens[5];

    Data dataAngajarii(dataStr); 
    map<string, set<string>> specializari;

    // --- LOGICA REPARATA AICI ---
    if (tip == "Tehnician") {
        // Citim toate coloanele de la indexul 6 pana la final
        // Ne asteptam la formatul "Tip:Marca" (ex: "TV:Samsung")
        for (size_t i = 6; i < tokens.size(); ++i) {
            string spec = tokens[i];
            
            // Ignoram token-uri goale
            if (spec.empty()) continue;

            size_t pos = spec.find(':');
            if (pos != string::npos) {
                string tipAparat = spec.substr(0, pos);
                string marcaAparat = spec.substr(pos + 1);
                
                specializari[tipAparat].insert(marcaAparat);
                
                // Debug: Sa vedem ca s-a citit corect
                // cout << "   [DEBUG] Specializare incarcata: " << tipAparat << " - " << marcaAparat << endl;
            }
        }
    }
    // -----------------------------

    return creeazaAngajat(tip, nume, prenume, cnp, dataAngajarii, oras, specializari);
}

unique_ptr<Angajat> AngajatFactory::creeazaAngajat(const string& tip, const string& nume, const string& prenume, const string& cnp, const Data& dataAngajarii, const string& orasDomiciliu, const map<string, set<string>>& specializari) {
    
    if (tip == "Receptioner") {
        return make_unique<Receptioner>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
    } 
    else if (tip == "Supervizor") {
        return make_unique<Supervizor>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
    } 
    else if (tip == "Tehnician") {
        auto tech = make_unique<Tehnician>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
        for (const auto& pereche : specializari) {
            for (const string& marca : pereche.second) {
                tech->adaugaSpecializare(pereche.first, marca);
            }
        }
        return tech;
    } 
    else {
        throw invalid_argument("Tip angajat necunoscut: " + tip);
    }
}