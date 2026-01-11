#include "CerereManager.h"
#include "CerereFactory.h" 
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

void CerereManager::incarcaDinCSV(const string& fisier, ElectrocasniceManager& catalog) {
    ifstream file(fisier);
    if (!file.is_open()) {
        throw runtime_error("Nu pot deschide fisierul cereri: " + fisier);
    }

    string linie;
    int linieNr = 0;
    while(getline(file, linie)) {
        linieNr++;
        try {
            auto cerere = CerereFactory::creeazaDinCSV(linie);
            
            if (cerere) {
                const auto* ap = cerere->getAparat();
                if (catalog.existaModel(ap->getTip(), ap->getMarca(), ap->getModel())) {
                    adaugaCerere(move(cerere));
                } else {
                    catalog.inregistreazaCerereInvalida(ap->getTip(), ap->getMarca(), ap->getModel());
                }
            }
        } catch (const exception& e) {
            cerr << "Eroare linia:" << linieNr << ": " << e.what() << endl;
        }
    }
}

void CerereManager::adaugaCerere(unique_ptr<CerereReparatie> c) {
    CerereReparatie* ptr = c.get();
    toateCererile.push_back(move(c)); 
    coadaAsteptare.push(ptr);         
}

CerereReparatie* CerereManager::getUrmatoareaInAsteptare() {
    if (coadaAsteptare.empty()) return nullptr;
    return coadaAsteptare.front();
}

void CerereManager::popAsteptare() {
    if (!coadaAsteptare.empty()) coadaAsteptare.pop();
}

void CerereManager::puneInapoiInAsteptare(CerereReparatie* c) {
    coadaAsteptare.push(c);
}

bool CerereManager::areCereriInAsteptare() const {
    return !coadaAsteptare.empty();
}

void CerereManager::repartizeazaCerere(CerereReparatie* c, int idTehnician) {
    if (!c) return;

    c->setIdTehnician(idTehnician);

    c->setStare(StareCerere::REPARTIZATA);

    cereriInLucru.push_back(c);
}

void CerereManager::mutaInFinalizate(CerereReparatie* c) {
    if (!c) return;
    c->setStare(StareCerere::FINALIZATA);
    cereriFinalizate.push_back(c);
}

vector<CerereReparatie*>& CerereManager::getCereriInLucru() {
    return cereriInLucru;
}

bool CerereManager::areCereriActive() const {
    return !coadaAsteptare.empty() || !cereriInLucru.empty();
}

void CerereManager::afiseazaStatisticiCurente() const {
    cout << "Statistici Momentane: " 
         << coadaAsteptare.size() << " In Asteptare | " 
         << cereriInLucru.size() << " In Lucru | " 
         << cereriFinalizate.size() << " Finalizate" << endl;
}



vector<int> CerereManager::getIdsInAsteptare() const {
    vector<int> ids;
    queue<CerereReparatie*> copie = coadaAsteptare;
    while (!copie.empty()) {
        ids.push_back(copie.front()->getId());
        copie.pop();
    }
    return ids;
}


void CerereManager::genereazaRaportCereriInAsteptare(const string& numeFisier) const {
    ofstream out(numeFisier);
    if (!out.is_open()) {
        cerr << "Eroare: Nu pot crea fisierul " << numeFisier << endl;
        return;
    }

    out << "Tip,Marca,Model,NumarCereri" << endl;
    
    map<string, map<string, map<string, int>>> grupate;
    
    queue<CerereReparatie*> copie = coadaAsteptare;
    while (!copie.empty()) {
        const auto* ap = copie.front()->getAparat();
        grupate[ap->getTip()][ap->getMarca()][ap->getModel()]++;
        copie.pop();
    }
    
    for (const auto& tip : grupate) {
        for (const auto& marca : tip.second) {
            for (const auto& model : marca.second) {
                out << tip.first << ","
                    << marca.first << ","
                    << model.first << ","
                    << model.second << endl;
            }
        }
    }
    
    out.close();
    cout << "[OK] Raport generat: " << numeFisier << endl;
}


void CerereManager::afiseazaIstoricReparatii() const {
    if (cereriFinalizate.empty()) {
        cout << "Nu exista reparatii finalizate." << endl;
        return;
    }

    cout << "\n=== ISTORIC REPARATII ===" << endl;
    for (const auto* c : cereriFinalizate) {
        const auto* ap = c->getAparat();
        cout << "[ID Cerere: " << c->getId() << "] - ";
        cout << ap->getTip() << " " << ap->getMarca() << " " << ap->getModel();
        cout << " (Tehnician ID: " << c->getIdTehnician() << ")" << endl;
    }
}