#include "AngajatiManager.h"
#include "AngajatFactory.h"
#include "Receptioner.h"
#include "Supervizor.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <iomanip>

using namespace std;

void AngajatiManager::incarcaDinCSV(const string& numeFisier) {
    ifstream file(numeFisier);
    
    if (!file.is_open()) {
        throw runtime_error("Eroare CRITICA: Nu pot deschide fisierul de angajati: " + numeFisier);
    }

    string linie;
    int linieNr = 0;
    while(getline(file, linie)) {
        linieNr++;
        try {
            auto ang = AngajatFactory::creeazaDinCSV(linie);
            if(ang) {
                adaugaAngajat(move(ang));
            }
        } catch(const exception& e) {
            cerr << "Eroare linia " << linieNr << ": " << e.what() << endl;
        }
    }
    file.close();
}

Tehnician* AngajatiManager::gasesteTehnicianDisponibil(const string& tipAparat, const string& marcaAparat) {
    Tehnician* ales = nullptr;
    double incarcareMinima = numeric_limits<double>::max(); 

    for (const auto& ang : angajati) {
        Tehnician* t = dynamic_cast<Tehnician*>(ang.get());
        
        if (t) {
            if (t->areSpecializare(tipAparat, marcaAparat)) {
                
                if (t->poatePrimiCerere()) {
                    
                    if (t->getDurataTotalaLucrata() < incarcareMinima) {
                        incarcareMinima = t->getDurataTotalaLucrata();
                        ales = t;
                    }
                }
            }
        }
    }
    return ales; 
}

void AngajatiManager::adaugaAngajat(unique_ptr<Angajat> a) {
    if (a) angajati.push_back(move(a));
}

bool AngajatiManager::stergeAngajat(const string& cnp) {
    auto it = remove_if(angajati.begin(), angajati.end(), 
        [&cnp](const unique_ptr<Angajat>& a) {
            return a->getCNP() == cnp;
        });
    
    if (it != angajati.end()) {
        angajati.erase(it, angajati.end());
        return true;
    }
    return false;
}

bool AngajatiManager::modificaNume(const string& cnp, const string& numeNou, const string& prenumeNou) {
    Angajat* a = cautaDupaCNP(cnp);
    if (a) {
        try {
            a->setNume(numeNou);
            a->setPrenume(prenumeNou);
            return true;
        } catch (...) { 
            return false; 
            }
    }
    return false;
}

Angajat* AngajatiManager::cautaDupaCNP(const string& cnp) {
    for (const auto& a : angajati) {
        if (a->getCNP() == cnp) return a.get();
    }
    return nullptr;
}

Angajat* AngajatiManager::cautaDupaID(int id) {
    for (const auto& a : angajati) {
        if (a->getId() == id) return a.get();
    }
    return nullptr;
}

bool AngajatiManager::verificaPersonalMinim() const {
    int t = 0, r = 0, s = 0;
    for (const auto& a : angajati) {
        if (dynamic_cast<Tehnician*>(a.get())) t++;
        else if (dynamic_cast<Receptioner*>(a.get())) r++;
        else if (dynamic_cast<Supervizor*>(a.get())) s++;
    }
    return (t >= 3 && r >= 1 && s >= 1);
}

void AngajatiManager::afiseazaToti() const {
    if (angajati.empty()) {
        cout << "Nu exista angajati." << endl;
        return;
    }
    cout << "\n=== ECHIPA SERVICE ===" << endl;
    for (const auto& a : angajati) {
        a->afisare(cout);
        cout << "----------------------" << endl;
    }
}

vector<Angajat*> AngajatiManager::getTop3Salarii() const {
    vector<Angajat*> lista;
    for (const auto& a : angajati) lista.push_back(a.get());

    sort(lista.begin(), lista.end(), [](Angajat* a, Angajat* b) {
        double s1 = a->calculeazaSalariu();
        double s2 = b->calculeazaSalariu();
        if (abs(s1 - s2) > 0.01) return s1 > s2;
        return a->getNume() < b->getNume();
    });

    if (lista.size() > 3) lista.resize(3);
    return lista;
}


void AngajatiManager::genereazaRaportTop3Salarii(const string& numeFisier) const {
    ofstream out(numeFisier);
    if (!out.is_open()) {
        cerr << "Eroare: Nu pot crea fisierul " << numeFisier << endl;
        return;
    }

    out << "Nume,Prenume,Tip,Salariu" << endl;
    
    auto top3 = getTop3Salarii();
    for (const auto* a : top3) {
        out << a->getNume() << ","
            << a->getPrenume() << ","
            << a->getTipAngajat() << ","
            << fixed << setprecision(2) << a->calculeazaSalariu() << endl;
    }
    
    out.close();
    cout << "[OK] Raport generat: " << numeFisier << endl;
}

Tehnician* AngajatiManager::getTehnicianCeaMaiLungaReparatie() const {
    Tehnician* maxTehn = nullptr;
    double maxDurata = 0;
    
    for (const auto& a : angajati) {
        Tehnician* t = dynamic_cast<Tehnician*>(a.get());
        if (t && t->getDurataTotalaLucrata() > maxDurata) {
            maxDurata = t->getDurataTotalaLucrata();
            maxTehn = t;
        }
    }
    return maxTehn;
}

bool AngajatiManager::adaugaAngajatManual(string tip, string nume, string prenume, string cnp, string data, string oras) {
    string linieSimulata = tip + "," + nume + "," + prenume + "," + cnp + "," + data + "," + oras;
    
    try {
        auto ang = AngajatFactory::creeazaDinCSV(linieSimulata);
        if (ang) {
            adaugaAngajat(move(ang));
            return true;
        }
    } catch (const exception& e) {
        cerr << "Eroare adaugare manuala: " << e.what() << endl;
    }
    return false;
}