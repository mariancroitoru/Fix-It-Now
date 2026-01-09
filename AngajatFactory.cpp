#include "AngajatFactory.h"
#include "Receptioner.h"
#include "Tehnician.h"
#include "Supervizor.h"
#include <stdexcept>

using namespace std;

unique_ptr<Angajat> AngajatFactory::creeazaAngajat(const string& tip, const string& nume, const string& prenume, const string& cnp, const Data& dataAngajarii, const string& orasDomiciliu,const map<string, set<string>>& specializari) {
        if (tip == "Receptioner") {
            return make_unique<Receptioner>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
        }
        else if (tip == "Supervizor") {
            return make_unique<Supervizor>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
        }
        else if (tip == "Tehnician") {
            auto tehnician = make_unique<Tehnician>(nume, prenume, cnp, dataAngajarii, orasDomiciliu);
            
            for (const auto& entry : specializari) {
                string tipElectrocasnic = entry.first;
                const set<string>& setMarci = entry.second;

                for (const auto& marca : setMarci) {
                    tehnician->adaugaSpecializare(tipElectrocasnic, marca);
                }
            }
            
            return tehnician;
        }
        else {
            throw invalid_argument("Eroare Factory: Tip angajat necunoscut (" + tip + ")");
        }
}

unique_ptr<Angajat> AngajatFactory::creeazaDinCSV(const string& linie) {
    vector<string> virgule = splitLine(linie, ',');

    if (virgule.size() < 6) {
        throw invalid_argument("Eroare CSV: Linie incompleta -> " + linie);
    }

    string tip = virgule[0];
    string nume = virgule[1];
    string prenume = virgule[2];
    string cnp = virgule[3];
    string dataStr = virgule[4]; // Format "ZZ.LL.AAAA"
    string oras = virgule[5];

    int zi = 0, luna = 0, an = 0;
    size_t p1 = dataStr.find('.');
    size_t p2 = dataStr.find('.', p1 + 1);
    
    if (p1 != string::npos && p2 != string::npos) {
        zi = stoi(dataStr.substr(0, p1));
        luna = stoi(dataStr.substr(p1 + 1, p2 - p1 - 1));
        an = stoi(dataStr.substr(p2 + 1));
    }
    Data dataAngajarii(zi, luna, an);

    map<string, set<string>> specializariMap;
    
    if (tip == "Tehnician" && virgule.size() > 6) {
        specializariMap = parseazaSpecializari(virgule[6]);
    }

    return creeazaAngajat(tip, nume, prenume, cnp, dataAngajarii, oras, specializariMap);
}


vector<string> AngajatFactory::splitLine(const string& linie, char delimitator) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = 0;

    while ((end = linie.find(delimitator, start)) != string::npos) {
        tokens.push_back(linie.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(linie.substr(start));
    
    return tokens;
}

map<string, set<string>> AngajatFactory::parseazaSpecializari(const string& text) {
    map<string, set<string>> rezultat;
    
    vector<string> perechi = splitLine(text, ';');
    
    for (const string& p : perechi) {
        size_t poz = p.find(':');
        if (poz != string::npos) {
            string tip = p.substr(0, poz);
            string marca = p.substr(poz + 1);
            
            rezultat[tip].insert(marca);
        }
    }
    return rezultat;
}