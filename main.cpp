#include <iostream>
#include <string>
#include <limits>
#include "ServiceManager.h"

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nApasati ENTER pentru a continua...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void afiseazaMeniuPrincipal() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║        FIXITNOW - SERVICE MENU         ║" << endl;
    cout << "╠════════════════════════════════════════╣" << endl;
    cout << "║ 1. Gestiune Angajati                  ║" << endl;
    cout << "║ 2. Gestiune Electrocasnice            ║" << endl;
    cout << "║ 3. Procesare Cereri                   ║" << endl;
    cout << "║ 4. Raportari                          ║" << endl;
    cout << "║ 5. Statistici                         ║" << endl;
    cout << "║ 0. Iesire                             ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "Optiune: ";
}

void meniuGestiuneAngajati(ServiceManager* sm) {
    while (true) {
        clearScreen();
        cout << "\n=== GESTIUNE ANGAJATI ===" << endl;
        cout << "1. Adauga angajat manual" << endl;
        cout << "2. Incarca angajati din CSV" << endl;
        cout << "3. Cauta angajat (dupa CNP)" << endl;
        cout << "4. Modifica nume angajat" << endl;
        cout << "5. Sterge angajat" << endl;
        cout << "6. Afiseaza toti angajatii" << endl;
        cout << "7. Verifica personal minim" << endl;
        cout << "0. Inapoi" << endl;
        cout << "\nOptiune: ";
        
        int opt;
        cin >> opt;
        cin.ignore();
        
        try {
            switch (opt) {
                case 1: {
                    cout << "\nTip angajat (Receptioner/Tehnician/Supervizor): ";
                    string tip;
                    getline(cin, tip);
                    
                    cout << "Nume: ";
                    string nume;
                    getline(cin, nume);
                    
                    cout << "Prenume: ";
                    string prenume;
                    getline(cin, prenume);
                    
                    cout << "CNP: ";
                    string cnp;
                    getline(cin, cnp);
                    
                    cout << "Data angajarii (DD.MM.YYYY): ";
                    string dataStr;
                    getline(cin, dataStr);
                    Data dataAngajarii(dataStr);
                    
                    cout << "Oras domiciliu: ";
                    string oras;
                    getline(cin, oras);
                    
                    map<string, set<string>> specializari;
                    
                    if (tip == "Tehnician") {
                        cout << "\nAdauga specializari (tip:marca, gol pentru a termina):" << endl;
                        while (true) {
                            cout << "Specializare (ex: TV:Samsung): ";
                            string spec;
                            getline(cin, spec);
                            if (spec.empty()) break;
                            
                            size_t pos = spec.find(':');
                            if (pos != string::npos) {
                                string tipElec = spec.substr(0, pos);
                                string marca = spec.substr(pos + 1);
                                specializari[tipElec].insert(marca);
                            }
                        }
                    }
                    
                    auto angajat = AngajatFactory::creeazaAngajat(tip, nume, prenume, cnp, 
                                                                  dataAngajarii, oras, specializari);
                    sm->adaugaAngajat(move(angajat));
                    pause();
                    break;
                }
                
                case 2: {
                    cout << "\nNumele fisierului CSV: ";
                    string numeFisier;
                    getline(cin, numeFisier);
                    sm->incarcaAngajatiDinCSV(numeFisier);
                    pause();
                    break;
                }
                
                case 3: {
                    cout << "\nCNP: ";
                    string cnp;
                    getline(cin, cnp);
                    sm->afiseazaAngajat(cnp);
                    pause();
                    break;
                }
                
                case 4: {
                    cout << "\nCNP angajat: ";
                    string cnp;
                    getline(cin, cnp);
                    
                    cout << "Nume nou: ";
                    string nume;
                    getline(cin, nume);
                    
                    cout << "Prenume nou: ";
                    string prenume;
                    getline(cin, prenume);
                    
                    if (sm->modificaNumeAngajat(cnp, nume, prenume)) {
                        cout << "Modificare reusita!" << endl;
                    } else {
                        cout << "Angajat nu a fost gasit!" << endl;
                    }
                    pause();
                    break;
                }
                
                case 5: {
                    cout << "\nCNP angajat de sters: ";
                    string cnp;
                    getline(cin, cnp);
                    
                    if (sm->stergeAngajat(cnp)) {
                        cout << "Angajat sters cu succes!" << endl;
                    } else {
                        cout << "Angajat nu a fost gasit!" << endl;
                    }
                    pause();
                    break;
                }
                
                case 6: {
                    sm->afiseazaTotiAngajatii();
                    pause();
                    break;
                }
                
                case 7: {
                    if (sm->arePersonalMinim()) {
                        cout << "\n✓ Service-ul ARE personalul minim necesar!" << endl;
                        cout << "Tehnicieni: " << sm->getNumarTehnicieni() << " (minim 3)" << endl;
                        cout << "Receptioneri: " << sm->getNumarReceptioneri() << " (minim 1)" << endl;
                        cout << "Supervizori: " << sm->getNumarSupervizori() << " (minim 1)" << endl;
                    } else {
                        cout << "\n✗ Service-ul NU ARE personalul minim necesar!" << endl;
                        cout << "Tehnicieni: " << sm->getNumarTehnicieni() << " (minim 3)" << endl;
                        cout << "Receptioneri: " << sm->getNumarReceptioneri() << " (minim 1)" << endl;
                        cout << "Supervizori: " << sm->getNumarSupervizori() << " (minim 1)" << endl;
                    }
                    pause();
                    break;
                }
                
                case 0:
                    return;
                    
                default:
                    cout << "Optiune invalida!" << endl;
                    pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl;
            pause();
        }
    }
}

void meniuGestiuneElectrocasnice(ServiceManager* sm) {
    while (true) {
        clearScreen();
        cout << "\n=== GESTIUNE ELECTROCASNICE ===" << endl;
        cout << "1. Adauga model in catalog" << endl;
        cout << "2. Incarca catalog din CSV" << endl;
        cout << "3. Sterge model din catalog" << endl;
        cout << "4. Afiseaza catalog complet" << endl;
        cout << "5. Verifica daca un model poate fi reparat" << endl;
        cout << "6. Afiseaza aparate nereparabile" << endl;
        cout << "0. Inapoi" << endl;
        cout << "\nOptiune: ";
        
        int opt;
        cin >> opt;
        cin.ignore();
        
        try {
            switch (opt) {
                case 1: {
                    cout << "\nTip (Frigider/TV/MasinaDeSpalat): ";
                    string tip;
                    getline(cin, tip);
                    
                    cout << "Marca: ";
                    string marca;
                    getline(cin, marca);
                    
                    cout << "Model: ";
                    string model;
                    getline(cin, model);
                    
                    cout << "An fabricatie: ";
                    int an;
                    cin >> an;
                    
                    cout << "Pret catalog: ";
                    double pret;
                    cin >> pret;
                    cin.ignore();
                    
                    map<string, string> dateSpec;
                    
                    if (tip == "Frigider") {
                        cout << "Are congelator? (da/nu): ";
                        string cong;
                        getline(cin, cong);
                        dateSpec["congelator"] = cong;
                    } else if (tip == "TV") {
                        cout << "Diagonala: ";
                        string diag;
                        getline(cin, diag);
                        dateSpec["diagonala"] = diag;
                        
                        cout << "Unitate (cm/inch): ";
                        string unit;
                        getline(cin, unit);
                        dateSpec["unitateDiagonala"] = unit;
                    } else if (tip == "MasinaDeSpalat") {
                        cout << "Capacitate (kg): ";
                        string cap;
                        getline(cin, cap);
                        dateSpec["capacitate"] = cap;
                    }
                    
                    auto electrocasnic = ElectrocasnicFactory::creeazaElectrocasnic(
                        tip, marca, model, an, pret, dateSpec);
                    sm->adaugaModelInCatalog(move(electrocasnic));
                    pause();
                    break;
                }
                
                case 2: {
                    cout << "\nNumele fisierului CSV: ";
                    string numeFisier;
                    getline(cin, numeFisier);
                    sm->incarcaCatalogDinCSV(numeFisier);
                    pause();
                    break;
                }
                
                case 3: {
                    cout << "\nTip: ";
                    string tip;
                    getline(cin, tip);
                    
                    cout << "Marca: ";
                    string marca;
                    getline(cin, marca);
                    
                    cout << "Model: ";
                    string model;
                    getline(cin, model);
                    
                    if (sm->stergeModelDinCatalog(tip, marca, model)) {
                        cout << "Model sters cu succes!" << endl;
                    } else {
                        cout << "Model nu a fost gasit!" << endl;
                    }
                    pause();
                    break;
                }
                
                case 4: {
                    sm->afiseazaCatalogComplet();
                    pause();
                    break;
                }
                
                case 5: {
                    cout << "\nTip: ";
                    string tip;
                    getline(cin, tip);
                    
                    cout << "Marca: ";
                    string marca;
                    getline(cin, marca);
                    
                    cout << "Model: ";
                    string model;
                    getline(cin, model);
                    
                    if (sm->poateFiReparat(tip, marca, model)) {
                        cout << "\n✓ Modelul POATE fi reparat de service!" << endl;
                    } else {
                        cout << "\n✗ Modelul NU POATE fi reparat de service!" << endl;
                    }
                    pause();
                    break;
                }
                
                case 6: {
                    sm->afiseazaAparateNereparabile();
                    pause();
                    break;
                }
                
                case 0:
                    return;
                    
                default:
                    cout << "Optiune invalida!" << endl;
                    pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl;
            pause();
        }
    }
}

void meniuProcesareCereri(ServiceManager* sm) {
    while (true) {
        clearScreen();
        cout << "\n=== PROCESARE CERERI ===" << endl;
        cout << "1. Incarca cereri din CSV" << endl;
        cout << "2. Start simulare" << endl;
        cout << "3. Stop simulare" << endl;
        cout << "0. Inapoi" << endl;
        cout << "\nOptiune: ";
        
        int opt;
        cin >> opt;
        cin.ignore();
        
        try {
            switch (opt) {
                case 1: {
                    cout << "\nNumele fisierului CSV: ";
                    string numeFisier;
                    getline(cin, numeFisier);
                    sm->incarcaCereriDinCSV(numeFisier);
                    pause();
                    break;
                }
                
                case 2: {
                    cout << "\nPornire simulare..." << endl;
                    sm->startSimulare();
                    pause();
                    break;
                }
                
                case 3: {
                    sm->stopSimulare();
                    pause();
                    break;
                }
                
                case 0:
                    return;
                    
                default:
                    cout << "Optiune invalida!" << endl;
                    pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl;
            pause();
        }
    }
}

void meniuRaportari(ServiceManager* sm) {
    while (true) {
        clearScreen();
        cout << "\n=== RAPORTARI ===" << endl;
        cout << "1. Top 3 angajati cu cel mai mare salariu" << endl;
        cout << "2. Tehnicianul cu cea mai lunga durata de lucru" << endl;
        cout << "3. Cereri in asteptare (grupate)" << endl;
        cout << "0. Inapoi" << endl;
        cout << "\nOptiune: ";
        
        int opt;
        cin >> opt;
        cin.ignore();
        
        try {
            switch (opt) {
                case 1: {
                    string numeFisier = "raport_top3_salarii.csv";
                    sm->genereazaRaportTop3Salarii(numeFisier);
                    pause();
                    break;
                }
                
                case 2: {
                    string numeFisier = "raport_tehnician_maxim.csv";
                    sm->genereazaRaportCeaMaiLungaReparatie(numeFisier);
                    pause();
                    break;
                }
                
                case 3: {
                    string numeFisier = "raport_cereri_asteptare.csv";
                    sm->genereazaRaportCereriInAsteptare(numeFisier);
                    pause();
                    break;
                }
                
                case 0:
                    return;
                    
                default:
                    cout << "Optiune invalida!" << endl;
                    pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl;
            pause();
        }
    }
}

int main() {
    ServiceManager* sm = ServiceManager::getInstance();
    
    cout << "╔════════════════════════════════════════╗" << endl;
    cout << "║     Bine ati venit la FixItNow!       ║" << endl;
    cout << "║   Service autorizat electrocasnice     ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    
    while (true) {
        clearScreen();
        afiseazaMeniuPrincipal();
        
        int optiune;
        cin >> optiune;
        cin.ignore();
        
        switch (optiune) {
            case 1:
                meniuGestiuneAngajati(sm);
                break;
                
            case 2:
                meniuGestiuneElectrocasnice(sm);
                break;
                
            case 3:
                meniuProcesareCereri(sm);
                break;
                
            case 4:
                meniuRaportari(sm);
                break;
                
            case 5:
                sm->afiseazaStatistici();
                pause();
                break;
                
            case 0:
                cout << "\nLa revedere!" << endl;
                ServiceManager::deleteInstance();
                return 0;
                
            default:
                cout << "Optiune invalida!" << endl;
                pause();
        }
    }
    
    return 0;
}