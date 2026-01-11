#include "ServiceManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;


void ServiceManager::incarcaDate(const string& fAng, const string& fEl, const string& fCer) {
    try {
        cout << "Incarcare date..." << endl;
        electrocasniceManager.incarcaDinCSV(fEl);
        angajatiManager.incarcaDinCSV(fAng);
        
        if (!angajatiManager.verificaPersonalMinim()) {
            cerr << " [ATENTIE] Personal insuficient!" << endl;
        }

        cerereManager.incarcaDinCSV(fCer, electrocasniceManager);
        cout << "Sistem initializat cu succes.\n" << endl;
    } catch (const exception& e) {
        cerr << "EROARE FATALA: " << e.what() << endl;
    }
}

void ServiceManager::runSimulare() {
    cout << "=== START SIMULARE SERVICE ===" << endl;

    int tickFaraActivitate = 0;

    while (cerereManager.areCereriActive()) {
        timpCurent++;
        
        cout << "\n[Timp " << timpCurent << "]" << endl;
        
        proceseazaTic();
        
        vector<int> idsAsteptare = cerereManager.getIdsInAsteptare();
        if (!idsAsteptare.empty()) {
            cout << " Cereri in asteptare: ";
            for (size_t i = 0; i < idsAsteptare.size(); ++i) {
                cout << idsAsteptare[i] << (i < idsAsteptare.size() - 1 ? ", " : "");
            }
            cout << "." << endl;
        } else {
            cout << " Cereri in asteptare: -" << endl;
        }

        size_t inLucru = cerereManager.getCereriInLucru().size();
        bool areAsteptare = cerereManager.areCereriInAsteptare();

        if (inLucru == 0 && areAsteptare) {
            tickFaraActivitate++;
            if (tickFaraActivitate >= 3) {
                // Iesim din bucla fara sa urlam erori
                break;
            }
        } else {
            tickFaraActivitate = 0;
        }

        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "\n==================================================" << endl;
    cout << "           SIMULARE FINALIZATA" << endl;
    cout << "==================================================" << endl;
    
    if (cerereManager.areCereriInAsteptare()) {
        cout << "[!] Atentie: S-a incheiat programul de lucru." << endl;
        cout << "    Au ramas " << cerereManager.getIdsInAsteptare().size() << " cereri nerezolvate " << endl;
        cout << "    (Posibil lipsa tehnicieni specializati sau piese)." << endl;
    } else {
        cout << " [OK] Toate cererile au fost procesate cu succes!" << endl;
        cout << "      Nicio cerere ramasa in asteptare." << endl;
    }
    cout << "==================================================" << endl;
}

void ServiceManager::proceseazaTic() {
    auto& lista = cerereManager.getCereriInLucru();
    auto it = lista.begin();
    
    while (it != lista.end()) {
        CerereReparatie* c = *it;

        if (c->getStare() == StareCerere::REPARTIZATA) {
            c->setStare(StareCerere::IN_LUCRU);
        }
        
        if (c->getStare() == StareCerere::IN_LUCRU) {
            c->proceseaza(); 
            cout << " Tehnician " << c->getIdTehnician() 
                 << " proceseaza cererea cu id " << c->getId() 
                 << " (raman " << c->getDurataRamasa() << " unitati de timp)" << endl;
        }

        if (c->esteFinalizata()) {
            cout << " Tehnician " << c->getIdTehnician() 
                 << " finalizeaza cererea " << c->getId() << endl;
            
            if (auto* t = dynamic_cast<Tehnician*>(angajatiManager.cautaDupaID(c->getIdTehnician()))) {
                t->finalizareCerere(c->getId(), c->getPretReparatie(), c->getDurataEstimata());
            }

            cerereManager.mutaInFinalizate(c);
            it = lista.erase(it);
        } else {
            ++it;
        }
    }

    incearcaAlocareCereri();
}

void ServiceManager::incearcaAlocareCereri() {
    vector<int> ids = cerereManager.getIdsInAsteptare();
    int limita = ids.size(); 
    int incercari = 0;

    while (cerereManager.areCereriInAsteptare() && incercari < limita) {
        CerereReparatie* c = cerereManager.getUrmatoareaInAsteptare();
        cerereManager.popAsteptare(); 
        
        const auto* ap = c->getAparat();
        Tehnician* tech = angajatiManager.gasesteTehnicianDisponibil(ap->getTip(), ap->getMarca());

        if (tech) {
            cout << " Tehnician " << tech->getId() 
                 << " primeste cererea cu id " << c->getId() << endl;
            
            tech->adaugaIdCerereActiva(c->getId());
            cerereManager.repartizeazaCerere(c, tech->getId());
        } else {
            cerereManager.puneInapoiInAsteptare(c);
        }
        incercari++;
    }
}