#include "Meniu.h"
#include "AngajatFactory.h"
#include "ElectrocasnicFactory.h"
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <fstream>

using namespace std;

Meniu::Meniu() {
    service = &ServiceManager::getInstance();
}

void Meniu::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Meniu::pause() {
    cout << "\nApasati ENTER pentru a continua...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void Meniu::afiseazaTitlu() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║        FIXITNOW - SERVICE MENU         ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
}

void Meniu::ruleazaAplicatie() {
    while (true) {
        clearScreen();
        afiseazaTitlu();
        cout << "1. Gestiune Angajati" << endl;
        cout << "2. Gestiune Electrocasnice" << endl;
        cout << "3. Procesare Cereri (Simulare)" << endl;
        cout << "4. Raportari" << endl;
        cout << "5. Statistici Generale" << endl;
        cout << "0. Iesire" << endl;
        cout << "\nOptiune: ";

        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        switch (opt) {
            case 1: meniuGestiuneAngajati(); break;
            case 2: meniuGestiuneElectrocasnice(); break;
            case 3: meniuProcesareCereri(); break;
            case 4: meniuRaportari(); break;
            case 5: 
                service->getCerereManager().afiseazaStatisticiCurente(); 
                pause();
                break;
            case 0:
                cout << "La revedere!" << endl;
                return;
            default:
                cout << "Optiune invalida!" << endl;
                pause();
        }
    }
}
void Meniu::meniuGestiuneAngajati() {
    while (true) {
        clearScreen();
        cout << "=== GESTIUNE ANGAJATI ===" << endl;
        cout << "1. Incarca din CSV" << endl;
        cout << "2. Afiseaza toti angajatii" << endl;
        cout << "3. Adauga Angajat Manual" << endl;      
        cout << "4. Modifica Nume Angajat" << endl;        
        cout << "5. Cauta dupa CNP (Afisare + Salariu)" << endl; 
        cout << "6. Sterge angajat" << endl;               
        cout << "7. Verifica personal minim" << endl;      
        cout << "0. Inapoi" << endl;
        cout << "Optiune: ";
        
        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(); 

        try {
            switch(opt) {
                case 1: {
                    cout << "Fisier: "; string f; getline(cin, f);
                    service->getAngajatiManager().incarcaDinCSV(f);
                    pause(); break;
                }
                case 2:
                    service->getAngajatiManager().afiseazaToti();
                    pause(); break;
                case 3: { 
                    cout << "Tip (Tehnician/Receptioner/Supervizor): "; string tip; getline(cin, tip);
                    cout << "Nume: "; string n; getline(cin, n);
                    cout << "Prenume: "; string p; getline(cin, p);
                    cout << "CNP: "; string cnp; getline(cin, cnp);
                    cout << "Data Angajarii (ZZ.LL.AAAA): "; string d; getline(cin, d);
                    cout << "Oras: "; string oras; getline(cin, oras);
                    
                    if(service->getAngajatiManager().adaugaAngajatManual(tip, n, p, cnp, d, oras))
                        cout << "Angajat adaugat cu succes!" << endl;
                    else 
                        cout << "Eroare la adaugare (tip invalid sau format gresit)." << endl;
                    pause(); break;
                }
                case 4: { 
                    cout << "CNP angajat de modificat: "; string cnp; getline(cin, cnp);
                    cout << "Nume nou: "; string n; getline(cin, n);
                    cout << "Prenume nou: "; string p; getline(cin, p);
                    
                    if(service->getAngajatiManager().modificaNume(cnp, n, p))
                         cout << "Date actualizate cu succes." << endl;
                    else cout << "Angajatul nu a fost gasit." << endl;
                    pause(); break;
                }
                case 5: { 
                    cout << "CNP: "; string cnp; getline(cin, cnp);
                    Angajat* a = service->getAngajatiManager().cautaDupaCNP(cnp);
                    if(a) {
                        a->afisare(cout);
                        cout << "Salariu curent : " << a->calculeazaSalariu() << " RON" << endl;
                    } else cout << "Nu exista un angajat cu acest CNP." << endl;
                    pause(); break;
                }
                case 6: { 
                    cout << "CNP: "; string cnp; getline(cin, cnp);
                    if(service->getAngajatiManager().stergeAngajat(cnp)) cout << "Angajat sters din sistem.";
                    else cout << "Nu a fost gasit.";
                    pause(); break;
                }
                case 7: { 
                    if(service->getAngajatiManager().verificaPersonalMinim()) 
                        cout << "Personal OK (Minim 3 Tehnicieni, 1 Receptioner, 1 Supervizor)." << endl;
                    else cout << "Personal INSUFICIENT pentru functionare service!" << endl;
                    pause(); break;
                }
                case 0: return;
                default: cout << "Optiune invalida." << endl; pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl; pause();
        }
    }
}
void Meniu::meniuGestiuneElectrocasnice() {
    while (true) {
        clearScreen();
        cout << "=== GESTIUNE ELECTROCASNICE ===" << endl;
        cout << "1. Incarca Catalog CSV" << endl;
        cout << "2. Afiseaza Catalog" << endl;
        cout << "3. Adauga Model Manual" << endl;       
        cout << "4. Sterge Model" << endl;             
        cout << "5. Verifica Model (Reparabil?)" << endl;
        cout << "6. Statistici Respinse (Sortat)" << endl;
        cout << "7. Istoric Aparate Reparate" << endl;  
        cout << "0. Inapoi" << endl;
        cout << "Optiune: ";

        int opt; 
        if (!(cin >> opt)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        try {
            switch(opt) {
                case 1: {
                    cout << "Fisier: "; string f; getline(cin, f);
                    service->getElectrocasniceManager().incarcaDinCSV(f);
                    pause(); break;
                }
                case 2:
                    service->getElectrocasniceManager().afiseazaCatalog();
                    pause(); break;
                case 3: { 
                    string t, m, mod; int an; double pret;
                    cout << "Tip (TV/Frigider/MasinaDeSpalat): "; getline(cin, t);
                    cout << "Marca: "; getline(cin, m);
                    cout << "Model: "; getline(cin, mod);
                    cout << "An: "; cin >> an;
                    cout << "Pret reparatie: "; cin >> pret; cin.ignore();
                    
                    service->getElectrocasniceManager().adaugaModelManual(t, m, mod, an, pret);
                    pause(); break;
                }
                case 4: { 
                    string m, mod;
                    cout << "Marca: "; getline(cin, m);
                    cout << "Model: "; getline(cin, mod);
                    if(service->getElectrocasniceManager().stergeModel(m, mod))
                        cout << "Model sters din catalog." << endl;
                    else cout << "Modelul nu a fost gasit." << endl;
                    pause(); break;
                }
                case 5: {
                    string t, m, mod;
                    cout << "Tip: "; getline(cin, t);
                    cout << "Marca: "; getline(cin, m);
                    cout << "Model: "; getline(cin, mod);
                    if(service->getElectrocasniceManager().existaModel(t, m, mod))
                        cout << "DA, il putem repara." << endl;
                    else cout << "NU, model necunoscut." << endl;
                    pause(); break;
                }
                case 6:
                    service->getElectrocasniceManager().afiseazaAparateNereparabile();
                    pause(); break;
                case 7: // === ISTORIC ===
                    service->getCerereManager().afiseazaIstoricReparatii();
                    pause(); break;
                case 0: return;
                default: cout << "Optiune invalida!" << endl; pause();
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl; pause();
        }
    }
}

void Meniu::meniuProcesareCereri() {
    while(true) {
        clearScreen();
        cout << "=== SIMULARE CERERI ===" << endl;
        cout << "1. Incarca Cereri CSV" << endl;
        cout << "2. Porneste Simulare" << endl;
        cout << "0. Inapoi" << endl;
        cout << "Optiune: ";

        int opt; cin >> opt; cin.ignore();

        try {
            switch(opt) {
                case 1: {
                    cout << "Fisier Cereri: "; string f; getline(cin, f);
                    service->getCerereManager().incarcaDinCSV(f, service->getElectrocasniceManager());
                    pause(); break;
                }
                case 2:
                    service->runSimulare();
                    pause(); break;
                case 0: return;
            }
        } catch (const exception& e) {
            cout << "Eroare: " << e.what() << endl; pause();
        }
    }
}

void Meniu::meniuRaportari() {
    clearScreen();
    cout << "=== RAPORTARI CSV ===" << endl;
    cout << "1. Top 3 Angajati (Salarii)" << endl;
    cout << "2. Tehnician - Cea mai lunga reparatie" << endl;
    cout << "3. Cereri in asteptare (grupate)" << endl;
    cout << "4. Genereaza toate rapoartele" << endl;
    cout << "0. Inapoi" << endl;
    cout << "Optiune: ";
    
    int opt; cin >> opt; cin.ignore();
    
    switch(opt) {
        case 1:
            service->getAngajatiManager().genereazaRaportTop3Salarii("raport_top3_salarii.csv");
            pause();
            break;
        case 2: {
            auto* t = service->getAngajatiManager().getTehnicianCeaMaiLungaReparatie();
            if (t) {
                ofstream out("raport_tehnician_top.csv");
                out << "ID,Nume,Prenume,DurataTotala" << endl;
                out << t->getId() << "," << t->getNume() << "," 
                    << t->getPrenume() << "," << t->getDurataTotalaLucrata() << endl;
                out.close();
                cout << "[OK] Raport generat: raport_tehnician_top.csv" << endl;
            } else {
                cout << "[!] Nu exista tehnicieni in sistem." << endl;
            }
            pause();
            break;
        }
        case 3:
            service->getCerereManager().genereazaRaportCereriInAsteptare("raport_cereri_asteptare.csv");
            pause();
            break;
        case 4: {
            service->getAngajatiManager().genereazaRaportTop3Salarii("raport_top3_salarii.csv");
            service->getCerereManager().genereazaRaportCereriInAsteptare("raport_cereri_asteptare.csv");
            auto* t = service->getAngajatiManager().getTehnicianCeaMaiLungaReparatie();
            if (t) {
                ofstream out("raport_tehnician_top.csv");
                out << "ID,Nume,Prenume,DurataTotala" << endl;
                out << t->getId() << "," << t->getNume() << "," 
                    << t->getPrenume() << "," << t->getDurataTotalaLucrata() << endl;
                out.close();
            }
            cout << "[OK] Toate rapoartele au fost generate!" << endl;
            pause();
            break;
        }
        case 0:
            return;
    }
}