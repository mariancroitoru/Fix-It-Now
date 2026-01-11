#include "ElectrocasniceManager.h"
#include "ElectrocasnicFactory.h"
#include "TV.h"            
#include "Frigider.h"       
#include "MasinaDeSpalat.h"
#include <fstream>
#include <iostream>
#include <stdexcept> 
#include <algorithm>
#include <vector>

using namespace std;

void ElectrocasniceManager::incarcaDinCSV(const string& numeFisier) {
    ifstream file(numeFisier);
    
    if (!file.is_open()) {
        throw runtime_error("Eroare : Nu pot deschide fisierul de electrocasnice: " + numeFisier);
    }

    string linie;
    int linieNr = 0;
    while (getline(file, linie)) {
        linieNr++;
        try {
            auto el = ElectrocasnicFactory::creeazaDinCSV(linie);
            if (el) {
                adaugaInCatalog(move(el)); 
            }
        } catch (const exception& e) {
            cerr << "Eroare linia:  " << linieNr << ": " << e.what() << endl;
        }
    }
    file.close();
}

void ElectrocasniceManager::adaugaInCatalog(unique_ptr<Electrocasnic> e) {
    if (!e) return;
    catalog[e->getTip()][e->getMarca()][e->getModel()] = move(e);
}

bool ElectrocasniceManager::existaModel(const string& tip, const string& marca, const string& model) const {
    if (catalog.count(tip) && catalog.at(tip).count(marca) && catalog.at(tip).at(marca).count(model)) {
        return true;
    }
    return false;
}

const Electrocasnic* ElectrocasniceManager::getDetaliiModel(const string& tip, const string& marca, const string& model) const {
    if (existaModel(tip, marca, model)) {
        return catalog.at(tip).at(marca).at(model).get();
    }
    return nullptr;
}

void ElectrocasniceManager::inregistreazaCerereInvalida(const string& tip, const string& marca, const string& model) {
    string cheie = tip + " " + marca + " " + model;
    aparateNereparabile[cheie]++;
}

void ElectrocasniceManager::afiseazaAparateNereparabile() const {
    if (aparateNereparabile.empty()) {
        cout << "Nu exista cereri respinse." << endl;
        return;
    }

    cout << "\n=== STATISTICA APARATE NEREPARABILE ===" << endl;
    vector<pair<string, int>> lista(aparateNereparabile.begin(), aparateNereparabile.end());
    
    sort(lista.begin(), lista.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; 
    });

    for (const auto& item : lista) {
        cout << " -> " << item.first << ": " << item.second << " cereri respinse." << endl;
    }
}

void ElectrocasniceManager::afiseazaCatalog() const {
    cout << "\n=== CATALOG DISPONIBIL ===" << endl;
    for (const auto& cat : catalog) {
        cout << "[" << cat.first << "]" << endl;
        for (const auto& marca : cat.second) {
            for (const auto& model : marca.second) {
                cout << "  - ";
                model.second->afisare(cout);
            }
        }
    }
}


void ElectrocasniceManager::adaugaModelManual(string tip, string marca, string model, int an, double pret) {
    unique_ptr<Electrocasnic> e = nullptr;
    
    if (tip == "TV") e = make_unique<TV>(marca, model, an, pret, 0, false);
    else if (tip == "Frigider") e = make_unique<Frigider>(marca, model, an, pret, false);
    else if (tip == "MasinaDeSpalat") e = make_unique<MasinaDeSpalat>(marca, model, an, pret, 0);
    
    if (e) {
        adaugaInCatalog(move(e));
    } else {
        cout << "Tip aparat necunoscut: " << tip << endl;
    }
}

bool ElectrocasniceManager::stergeModel(string marca, string model) {
    for (auto& categorie : catalog) { 
        if (categorie.second.count(marca)) {
            auto& modeleMap = categorie.second[marca];
            if (modeleMap.count(model)) {
                modeleMap.erase(model);
                
                if (modeleMap.empty()) categorie.second.erase(marca);
                
                return true; 
            }
        }
    }
    return false; 
}