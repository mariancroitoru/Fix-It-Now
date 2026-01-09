#include "ServiceManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <algorithm> // Pentru sort, min_element, find_if

using namespace std;

// Singleton instance
ServiceManager* ServiceManager::instance = nullptr;

// Constructor
ServiceManager::ServiceManager() : timpCurent(0), esteInSimulare(false) {}

// Destructor
ServiceManager::~ServiceManager() {
    // unique_ptr se eliberează automat
    // Curățare memorie pentru instance se face în deleteInstance
}

// Singleton: Get Instance
ServiceManager* ServiceManager::getInstance() {
    if (instance == nullptr) {
        instance = new ServiceManager();
    }
    return instance;
}

// Singleton: Delete Instance
void ServiceManager::deleteInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

// === GESTIUNE ANGAJAȚI ===

bool ServiceManager::arePersonalMinim() const {
    return (getNumarTehnicieni() >= 3 && 
            getNumarReceptioneri() >= 1 && 
            getNumarSupervizori() >= 1);
}

int ServiceManager::getNumarTehnicieni() const {
    return count_if(angajati.begin(), angajati.end(), 
        [](const unique_ptr<Angajat>& a) { return a->getTipAngajat() == "Tehnician"; });
}

int ServiceManager::getNumarReceptioneri() const {
    return count_if(angajati.begin(), angajati.end(), 
        [](const unique_ptr<Angajat>& a) { return a->getTipAngajat() == "Receptioner"; });
}

int ServiceManager::getNumarSupervizori() const {
    return count_if(angajati.begin(), angajati.end(), 
        [](const unique_ptr<Angajat>& a) { return a->getTipAngajat() == "Supervizor"; });
}

void ServiceManager::adaugaAngajat(unique_ptr<Angajat> angajat) {
    if (!angajat) {
        throw invalid_argument("Angajat null!");
    }
    angajati.push_back(move(angajat));
    cout << "Angajat adaugat cu succes! ID: " << angajati.back()->getId() << endl;
}

bool ServiceManager::stergeAngajat(const string& cnp) {
    auto it = find_if(angajati.begin(), angajati.end(),
        [&cnp](const unique_ptr<Angajat>& a) { return a->getCNP() == cnp; });
    
    if (it != angajati.end()) {
        cout << "Angajat sters: " << (*it)->getNume() << " " << (*it)->getPrenume() << endl;
        angajati.erase(it);
        return true;
    }
    return false;
}

bool ServiceManager::modificaNumeAngajat(const string& cnp, const string& numeNou, const string& prenumeNou) {
    Angajat* ang = cautaAngajatDupaCNP(cnp);
    if (ang) {
        ang->setNume(numeNou);
        ang->setPrenume(prenumeNou);
        cout << "Nume modificat cu succes!" << endl;
        return true;
    }
    return false;
}

Angajat* ServiceManager::cautaAngajatDupaCNP(const string& cnp) {
    auto it = find_if(angajati.begin(), angajati.end(),
        [&cnp](const unique_ptr<Angajat>& a) { return a->getCNP() == cnp; });
    
    return (it != angajati.end()) ? it->get() : nullptr;
}

Angajat* ServiceManager::cautaAngajatDupaID(int id) {
    auto it = find_if(angajati.begin(), angajati.end(),
        [id](const unique_ptr<Angajat>& a) { return a->getId() == id; });
    
    return (it != angajati.end()) ? it->get() : nullptr;
}

void ServiceManager::afiseazaAngajat(const string& cnp) const {
    auto it = find_if(angajati.begin(), angajati.end(),
        [&cnp](const unique_ptr<Angajat>& a) { return a->getCNP() == cnp; });
    
    if (it != angajati.end()) {
        (*it)->afisare(cout);
    } else {
        cout << "Angajat cu CNP " << cnp << " nu a fost gasit!" << endl;
    }
}

void ServiceManager::afiseazaTotiAngajatii() const {
    if (angajati.empty()) {
        cout << "Nu exista angajati inregistrati!" << endl;
        return;
    }
    
    cout << "\n=== LISTA ANGAJATI ===" << endl;
    for (const auto& ang : angajati) {
        ang->afisare(cout);
        cout << "-----------------------------------" << endl;
    }
    cout << "Total: " << angajati.size() << " angajati" << endl;
}

void ServiceManager::incarcaAngajatiDinCSV(const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw runtime_error("Nu pot deschide fisierul: " + numeFisier);
    }
    
    string linie;
    int numarLinie = 0;
    int adaugati = 0, esuati = 0;
    
    cout << "\n=== INCARCARE ANGAJATI DIN " << numeFisier << " ===" << endl;
    
    while (getline(fin, linie)) {
        numarLinie++;
        
        if (linie.empty() || linie[0] == '#') continue; // skip comentarii
        
        try {
            auto angajat = AngajatFactory::creeazaDinCSV(linie);
            adaugaAngajat(move(angajat));
            adaugati++;
        } catch (const exception& e) {
            cout << "Eroare la citire: Angajat invalid pe linia " << numarLinie 
                 << ", cauza: " << e.what() << endl;
            esuati++;
        }
    }
    
    fin.close();
    cout << "Incarcati: " << adaugati << " | Esuati: " << esuati << endl;
}

// === GESTIUNE ELECTROCASNICE ===

void ServiceManager::adaugaModelInCatalog(unique_ptr<Electrocasnic> electrocasnic) {
    if (!electrocasnic) {
        throw invalid_argument("Electrocasnic null!");
    }
    
    string tip = electrocasnic->getTip();
    string marca = electrocasnic->getMarca();
    string model = electrocasnic->getModel();
    
    catalogElectrocasnice[tip][marca][model] = move(electrocasnic);
    cout << "Model adaugat in catalog: " << tip << " " << marca << " " << model << endl;
}

bool ServiceManager::stergeModelDinCatalog(const string& tip, const string& marca, const string& model) {
    auto itTip = catalogElectrocasnice.find(tip);
    if (itTip == catalogElectrocasnice.end()) return false;
    
    auto itMarca = itTip->second.find(marca);
    if (itMarca == itTip->second.end()) return false;
    
    auto itModel = itMarca->second.find(model);
    if (itModel == itMarca->second.end()) return false;
    
    itMarca->second.erase(itModel);
    
    // Curăță structura dacă e goală
    if (itMarca->second.empty()) {
        itTip->second.erase(itMarca);
        if (itTip->second.empty()) {
            catalogElectrocasnice.erase(itTip);
        }
    }
    
    cout << "Model sters din catalog: " << tip << " " << marca << " " << model << endl;
    return true;
}

bool ServiceManager::poateFiReparat(const string& tip, const string& marca, const string& model) const {
    auto itTip = catalogElectrocasnice.find(tip);
    if (itTip == catalogElectrocasnice.end()) return false;
    
    auto itMarca = itTip->second.find(marca);
    if (itMarca == itTip->second.end()) return false;
    
    return itMarca->second.find(model) != itMarca->second.end();
}

const Electrocasnic* ServiceManager::getDetaliiModel(const string& tip, const string& marca, const string& model) const {
    auto itTip = catalogElectrocasnice.find(tip);
    if (itTip == catalogElectrocasnice.end()) return nullptr;
    
    auto itMarca = itTip->second.find(marca);
    if (itMarca == itTip->second.end()) return nullptr;
    
    auto itModel = itMarca->second.find(model);
    if (itModel == itMarca->second.end()) return nullptr;
    
    return itModel->second.get();
}

void ServiceManager::afiseazaCatalogComplet() const {
    if (catalogElectrocasnice.empty()) {
        cout << "Catalogul este gol!" << endl;
        return;
    }
    
    cout << "\n=== CATALOG ELECTROCASNICE REPARABILE ===" << endl;
    for (const auto& [tip, marci] : catalogElectrocasnice) {
        cout << "\n" << tip << ":" << endl;
        for (const auto& [marca, modele] : marci) {
            cout << "  " << marca << ": ";
            for (const auto& [model, detalii] : modele) {
                cout << model << " ";
            }
            cout << endl;
        }
    }
}

void ServiceManager::afiseazaAparateNereparabile() const {
    if (aparateNereparabile.empty()) {
        cout << "Nu exista aparate nereparabile inregistrate." << endl;
        return;
    }
    
    // Sortare descrescătoare după număr apariții
    vector<pair<string, int>> sorted(aparateNereparabile.begin(), aparateNereparabile.end());
    sort(sorted.begin(), sorted.end(), 
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });
    
    cout << "\n=== APARATE NEREPARABILE (sortate dupa aparitii) ===" << endl;
    for (const auto& [aparat, count] : sorted) {
        cout << aparat << ": " << count << " aparitii" << endl;
    }
}

void ServiceManager::incarcaCatalogDinCSV(const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw runtime_error("Nu pot deschide fisierul: " + numeFisier);
    }
    
    string linie;
    int numarLinie = 0;
    int adaugati = 0, esuati = 0;
    
    cout << "\n=== INCARCARE CATALOG DIN " << numeFisier << " ===" << endl;
    
    while (getline(fin, linie)) {
        numarLinie++;
        
        if (linie.empty() || linie[0] == '#') continue;
        
        try {
            // Format: tip,marca,model,anFabricatie,pretCatalog,dateSpecifice
            // Exemplu: Frigider,Samsung,Frost200,2020,2500.0,congelator:da
            
            stringstream ss(linie);
            string tip, marca, model, anStr, pretStr, dateSpec;
            
            getline(ss, tip, ',');
            getline(ss, marca, ',');
            getline(ss, model, ',');
            getline(ss, anStr, ',');
            getline(ss, pretStr, ',');
            getline(ss, dateSpec);
            
            int anFabricatie = stoi(anStr);
            double pretCatalog = stod(pretStr);
            
            auto dateSpecifice = ElectrocasnicFactory::parseazaDateSpecifice(dateSpec);
            auto electrocasnic = ElectrocasnicFactory::creeazaElectrocasnic(
                tip, marca, model, anFabricatie, pretCatalog, dateSpecifice);
            
            adaugaModelInCatalog(move(electrocasnic));
            adaugati++;
            
        } catch (const exception& e) {
            cout << "Eroare la citire: Model invalid pe linia " << numarLinie 
                 << ", cauza: " << e.what() << endl;
            esuati++;
        }
    }
    
    fin.close();
    cout << "Incarcati: " << adaugati << " | Esuati: " << esuati << endl;
}

// === GESTIUNE CERERI ===

void ServiceManager::primesteCerere(unique_ptr<CerereReparatie> cerere) {
    if (!cerere) {
        throw invalid_argument("Cerere null!");
    }
    
    const Electrocasnic* aparat = cerere->getAparat();
    if (!aparat) {
        cout << "Cerere #" << cerere->getId() << " respinsa: aparat invalid!" << endl;
        return;
    }
    
    // Verificare dacă poate fi reparat (exista in catalog)
    if (!poateFiReparat(aparat->getTip(), aparat->getMarca(), aparat->getModel())) {
        // Adaugă la statistici nereparabile
        string cheie = aparat->getTip() + "_" + aparat->getMarca() + "_" + aparat->getModel();
        aparateNereparabile[cheie]++;
        
        cerere->setStare(StareCerere::INVALIDA);
        cout << "Cerere #" << cerere->getId() << " INVALIDA: " 
             << aparat->getTip() << " " << aparat->getMarca() << " " 
             << aparat->getModel() << " nu poate fi reparat (nu e in catalog)!" << endl;
        
        // Nu o adăugăm la cereri active sau de procesat, dar o păstrăm în istoric dacă e nevoie
        // Aici aleg să o păstrez în vectorul toateCererile marcată INVALIDA
    }
    
    toateCererile.push_back(move(cerere));
}

void ServiceManager::incarcaCereriDinCSV(const string& numeFisier) {
    ifstream fin(numeFisier);
    if (!fin.is_open()) {
        throw runtime_error("Nu pot deschide fisierul: " + numeFisier);
    }
    
    string linie;
    int numarLinie = 0;
    int adaugate = 0, esuate = 0;
    
    cout << "\n=== INCARCARE CERERI DIN " << numeFisier << " ===" << endl;
    
    while (getline(fin, linie)) {
        numarLinie++;
        
        if (linie.empty() || linie[0] == '#') continue;
        
        try {
            // Format: tip,marca,model,anFabricatie,pretCatalog,dateSpec,timestamp,complexitate
            // Exemplu: Frigider,Samsung,Frost200,2020,2500.0,congelator:da,01.01.2026 10:30:00,3
            
            stringstream ss(linie);
            string tip, marca, model, anStr, pretStr, dateSpec, timestampStr, complStr;
            
            getline(ss, tip, ',');
            getline(ss, marca, ',');
            getline(ss, model, ',');
            getline(ss, anStr, ',');
            getline(ss, pretStr, ',');
            getline(ss, dateSpec, ',');
            getline(ss, timestampStr, ',');
            getline(ss, complStr);
            
            int anFabricatie = stoi(anStr);
            double pretCatalog = stod(pretStr);
            int complexitate = stoi(complStr);
            
            // Parse timestamp: "DD.MM.YYYY HH:MM:SS" sau simplu time_t
            // Simplificare: presupunem timestamp unix sau parsare manuală
            // Aici voi folosi o metodă simplificată sau fictivă pentru exemplu
             struct tm tm_time = {};
             stringstream ts_ss(timestampStr);
             ts_ss >> get_time(&tm_time, "%d.%m.%Y %H:%M:%S");
             time_t timestamp = mktime(&tm_time);
             if (timestamp == -1) timestamp = time(0); // Fallback la timpul curent
            
            // Creează electrocasnic
            auto dateSpecifice = ElectrocasnicFactory::parseazaDateSpecifice(dateSpec);
            auto electrocasnic = ElectrocasnicFactory::creeazaElectrocasnic(
                tip, marca, model, anFabricatie, pretCatalog, dateSpecifice);
            
            // Creează cerere
            auto cerere = make_unique<CerereReparatie>(move(electrocasnic), timestamp, complexitate);
            primesteCerere(move(cerere));
            adaugate++;
            
        } catch (const exception& e) {
            cout << "Eroare la citire: Cerere invalida pe linia " << numarLinie 
                 << ", cauza: " << e.what() << endl;
            esuate++;
        }
    }
    
    fin.close();
    cout << "Incarcate: " << adaugate << " | Esuate: " << esuate << endl;
}

bool ServiceManager::esteCerereValida(const CerereReparatie& cerere) {
    return cerere.esteValida();
}

// === SIMULARE ===

Tehnician* ServiceManager::gasesteTehnicianPotrivit(const CerereReparatie& cerere) {
    const Electrocasnic* aparat = cerere.getAparat();
    if (!aparat) return nullptr;
    
    vector<Tehnician*> candidati;
    
    // Filtrare tehnicieni eligibili
    for (auto& ang : angajati) {
        if (ang->getTipAngajat() == "Tehnician") {
            Tehnician* teh = dynamic_cast<Tehnician*>(ang.get());
            
            // Criteriu 1: are specializarea
            if (!teh->areSpecializare(aparat->getTip(), aparat->getMarca())) {
                continue;
            }
            
            // Criteriu 2: are sub 3 cereri active
            if (!teh->poatePrimiCerere()) {
                continue;
            }
            
            candidati.push_back(teh);
        }
    }
    
    if (candidati.empty()) return nullptr;
    
    // Criteriu 3: încărcare echilibrată (cel mai mic durata totală)
    return *min_element(candidati.begin(), candidati.end(),
        [](Tehnician* a, Tehnician* b) {
            return a->getDurataTotalaLucrata() < b->getDurataTotalaLucrata();
        });
}

void ServiceManager::alocaCerere(CerereReparatie* cerere, Tehnician* tehnician) {
    if (!cerere || !tehnician) return;
    
    cerere->setStare(StareCerere::REPARTIZATA);
    cerere->setIdTehnician(tehnician->getId());
    tehnician->adaugaIdCerereActiva(cerere->getId());
    
    cereriActive.push_back(cerere);
    
    cout << "  Tehnician " << tehnician->getNume() << " primeste cererea " 
         << cerere->getId() << endl;
}

void ServiceManager::procesareCereriInAsteptare() {
    // Folosim un vector temporar pentru a re-insera cererile nealocate
    // ca să păstrăm ordinea, dar să permitem scoaterea celor alocate
    vector<CerereReparatie*> nealocate;
    bool sAlocatCeva = false;

    while (!cereriInAsteptare.empty()) {
        CerereReparatie* cerere = cereriInAsteptare.front();
        cereriInAsteptare.pop();
        
        Tehnician* teh = gasesteTehnicianPotrivit(*cerere);
        if (teh) {
            alocaCerere(cerere, teh);
            cerere->setStare(StareCerere::IN_LUCRU);
            sAlocatCeva = true;
        } else {
            nealocate.push_back(cerere);
        }
    }
    
    // Punem înapoi cererile care nu au putut fi alocate
    for (auto* c : nealocate) {
        cereriInAsteptare.push(c);
    }
}

void ServiceManager::startSimulare() {
    if (!arePersonalMinim()) {
        cout << "\n!!! SERVICE-UL NU POATE FUNCTIONA !!!" << endl;
        cout << "Este nevoie de minim: 3 tehnicieni, 1 receptioner, 1 supervizor" << endl;
        cout << "Curent: " << getNumarTehnicieni() << " tehnicieni, " 
             << getNumarReceptioneri() << " receptioneri, "
             << getNumarSupervizori() << " supervizori" << endl;
        return;
    }
    
    timpCurent = 0;
    esteInSimulare = true;
    
    cout << "\n========================================" << endl;
    cout << "===     PORNIRE SIMULARE SERVICE     ===" << endl;
    cout << "========================================\n" << endl;
    
    // Sortează toate cererile după timestamp
    sort(toateCererile.begin(), toateCererile.end(),
        [](const unique_ptr<CerereReparatie>& a, const unique_ptr<CerereReparatie>& b) {
            return a->getTimeStamp() < b->getTimeStamp();
        });
    
    // Pune cererile valide în așteptare
    for (auto& cerere : toateCererile) {
        if (cerere->esteValida() && cerere->getStare() != StareCerere::INVALIDA) {
            cereriInAsteptare.push(cerere.get());
        }
    }
    
    cout << "Total cereri valide de procesat: " << cereriInAsteptare.size() << "\n" << endl;
    
    // Loop principal simulare
    while (esteInSimulare) {
        executeazaTic();
        timpCurent++;
        
        // Oprire când nu mai sunt cereri active sau în așteptare
        if (cereriActive.empty() && cereriInAsteptare.empty()) {
            break;
        }
        
        this_thread::sleep_for(chrono::seconds(1)); // 1 tic = 1 secundă
    }
    
    cout << "\n========================================" << endl;
    cout << "===   SIMULARE FINALIZATA CU SUCCES  ===" << endl;
    cout << "========================================" << endl;
    cout << "Timp total: " << timpCurent << " unitati" << endl;
    cout << "Cereri finalizate: " << cereriFinalzate.size() << endl;
}

void ServiceManager::executeazaTic() {
    cout << "\n[Timp " << timpCurent << "]" << endl;
    
    // 1. Procesează cereri active
    vector<CerereReparatie*> deFinalizat;
    
    for (auto* cerere : cereriActive) {
        if (cerere->getStare() == StareCerere::IN_LUCRU) {
            cerere->proceseaza();
            
            if (cerere->esteFinalizata()) {
                deFinalizat.push_back(cerere);
            } else {
                cout << "  Tehnician " << cerere->getIdTehnician() // Ar putea fi getNume() dacă ai acces
                     << " proceseaza cererea " << cerere->getId()
                     << " (raman " << cerere->getDurataRamasa() << " unitati)" << endl;
            }
        }
    }
    
    // 2. Finalizează cererile terminate
    for (auto* cerere : deFinalizat) {
        finalizeazaCerere(cerere);
    }
    
    // 3. Încearcă să aloce cereri din așteptare
    procesareCereriInAsteptare();
    
    // 4. Schimbă starea cererilor alocate în IN_LUCRU
    for (auto* cerere : cereriActive) {
        if (cerere->getStare() == StareCerere::REPARTIZATA) {
            cerere->setStare(StareCerere::IN_LUCRU);
        }
    }
    
    // 5. Afișează cereri în așteptare
    if (!cereriInAsteptare.empty()) {
        cout << "  Cereri in asteptare: ";
        queue<CerereReparatie*> temp = cereriInAsteptare;
        while (!temp.empty()) {
            cout << temp.front()->getId();
            temp.pop();
            if (!temp.empty()) cout << ", ";
        }
        cout << "." << endl;
    }
}

void ServiceManager::afiseazaStatusTic() const {
    cout << "\n--- Status la timpul " << timpCurent << " ---" << endl;
    cout << "Cereri active: " << cereriActive.size() << endl;
    cout << "Cereri in asteptare: " << cereriInAsteptare.size() << endl;
    cout << "Cereri finalizate: " << cereriFinalzate.size() << endl;
}

void ServiceManager::stopSimulare() {
    esteInSimulare = false;
    cout << "\nSimulare oprita manual!" << endl;
}

void ServiceManager::finalizeazaCerere(CerereReparatie* cerere) {
    if (!cerere) return;
    
    Angajat* ang = cautaAngajatDupaID(cerere->getIdTehnician());
    if (ang && ang->getTipAngajat() == "Tehnician") {
        Tehnician* teh = dynamic_cast<Tehnician*>(ang);
        teh->finalizareCerere(cerere->getId(), cerere->getPretReparatie(), 
                              cerere->getDurataEstimata());
    }
    
    // Mută din active în finalizate
    auto it = find(cereriActive.begin(), cereriActive.end(), cerere);
    if (it != cereriActive.end()) {
        cereriActive.erase(it);
    }
    cereriFinalzate.push_back(cerere);
    cerere->setStare(StareCerere::FINALIZATA);
    
    cout << "  Tehnician " << cerere->getIdTehnician() 
         << " finalizeaza cererea " << cerere->getId() << endl;
}

// === RAPORTĂRI ===

void ServiceManager::genereazaRaportTop3Salarii(const string& numeFisier) const {
    ofstream fout(numeFisier);
    if (!fout.is_open()) {
        throw runtime_error("Nu pot crea fisierul: " + numeFisier);
    }
    
    // Sortare după salariu descrescător, apoi după nume
    vector<Angajat*> sorted;
    for (const auto& ang : angajati) {
        sorted.push_back(ang.get());
    }
    
    sort(sorted.begin(), sorted.end(), [](Angajat* a, Angajat* b) {
        double salA = a->calculeazaSalariu();
        double salB = b->calculeazaSalariu();
        if (abs(salA - salB) > 0.001) return salA > salB; // Folosim toleranta pentru float
        
        string numeA = a->getNume() + " " + a->getPrenume();
        string numeB = b->getNume() + " " + b->getPrenume();
        return numeA < numeB;
    });
    
    fout << "ID,Nume,Prenume,Tip,Salariu" << endl;
    
    int count = min(3, static_cast<int>(sorted.size()));
    for (int i = 0; i < count; i++) {
        fout << sorted[i]->getId() << ","
             << sorted[i]->getNume() << ","
             << sorted[i]->getPrenume() << ","
             << sorted[i]->getTipAngajat() << ","
             << fixed << setprecision(2) << sorted[i]->calculeazaSalariu() << endl;
    }
    
    fout.close();
    cout << "Raport generat: " << numeFisier << endl;
}

void ServiceManager::genereazaRaportCeaMaiLungaReparatie(const string& numeFisier) const {
    ofstream fout(numeFisier);
    if (!fout.is_open()) {
        throw runtime_error("Nu pot crea fisierul: " + numeFisier);
    }
    
    Tehnician* maxTeh = nullptr;
    double maxDurata = -1;
    
    for (const auto& ang : angajati) {
        if (ang->getTipAngajat() == "Tehnician") {
            Tehnician* teh = dynamic_cast<Tehnician*>(ang.get());
            double durata = teh->getDurataTotalaLucrata();
            if (durata > maxDurata) {
                maxDurata = durata;
                maxTeh = teh;
            }
        }
    }
    
    fout << "ID,Nume,Prenume,CNP,DataAngajarii,OrasDomiciliu,DurataTotalaLucrata,Salariu" << endl;
    
    if (maxTeh) {
        fout << maxTeh->getId() << ","
             << maxTeh->getNume() << ","
             << maxTeh->getPrenume() << ","
             << maxTeh->getCNP() << ","
             << maxTeh->getDataAngajarii() << "," // Presupune operator<< overload pentru Data
             << maxTeh->getOrasDomiciliu() << ","
             << fixed << setprecision(2) << maxTeh->getDurataTotalaLucrata() << ","
             << maxTeh->calculeazaSalariu() << endl;
    }
    
    fout.close();
    cout << "Raport generat: " << numeFisier << endl;
}

void ServiceManager::genereazaRaportCereriInAsteptare(const string& numeFisier) const {
    ofstream fout(numeFisier);
    if (!fout.is_open()) {
        throw runtime_error("Nu pot crea fisierul: " + numeFisier);
    }
    
    // Grupare pe tip, marcă, model
    map<string, map<string, map<string, int>>> grupate;
    
    // Folosim o copie a cozii pentru a itera
    queue<CerereReparatie*> temp = cereriInAsteptare;
    while (!temp.empty()) {
        CerereReparatie* c = temp.front();
        temp.pop();
        
        const Electrocasnic* ap = c->getAparat();
        if (ap) {
            grupate[ap->getTip()][ap->getMarca()][ap->getModel()]++;
        }
    }
    
    fout << "Tip,Marca,Model,Numar_Cereri" << endl;
    
    // Sortare alfabetică (map face asta automat)
    for (const auto& [tip, marci] : grupate) {
        for (const auto& [marca, modele] : marci) {
            for (const auto& [model, count] : modele) {
                fout << tip << "," << marca << "," << model << "," << count << endl;
            }
        }
    }
    
    fout.close();
    cout << "Raport generat: " << numeFisier << endl;
}

// === UTILITĂȚI ===

void ServiceManager::afiseazaStatistici() const {
    cout << "\n=== STATISTICI SERVICE ===" << endl;
    cout << "Angajati: " << angajati.size() << endl;
    cout << "  - Tehnicieni: " << getNumarTehnicieni() << endl;
    cout << "  - Receptioneri: " << getNumarReceptioneri() << endl;
    cout << "  - Supervizori: " << getNumarSupervizori() << endl;
    cout << "\nModele in catalog: ";
    int total = 0;
    for (const auto& [tip, marci] : catalogElectrocasnice) {
        for (const auto& [marca, modele] : marci) {
            total += modele.size();
        }
    }
    cout << total << endl;
    cout << "\nCereri totale: " << toateCererile.size() << endl;
    cout << "  - Valide: " << (toateCererile.size() - aparateNereparabile.size()) << endl;
    cout << "  - Invalide: " << aparateNereparabile.size() << endl;
    cout << "  - Active: " << cereriActive.size() << endl;
    cout << "  - In asteptare: " << cereriInAsteptare.size() << endl;
    cout << "  - Finalizate: " << cereriFinalzate.size() << endl;
}

void ServiceManager::reset() {
    angajati.clear();
    catalogElectrocasnice.clear();
    toateCererile.clear();
    cereriActive.clear();
    // Golește coada
    queue<CerereReparatie*> empty;
    swap(cereriInAsteptare, empty);
    
    cereriFinalzate.clear();
    aparateNereparabile.clear();
    timpCurent = 0;
    esteInSimulare = false;
    cout << "ServiceManager resetat!" << endl;
}