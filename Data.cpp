#include "Data.h"
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <vector>
#include <chrono>



using namespace std;

// --- CONSTRUCTORI ---

Data::Data(int z,int l,int a):zi(z),luna(l), an(a){
    if (!esteValida()) {
        throw invalid_argument("Data invalida: " + to_string(z) + "." + to_string(l) + "." + to_string(a));
    }
}

// Constructor din string: "12.03.2021"
Data::Data(const string& dataStr) {
    // Validare format de baza (lungime si puncte)
    if (dataStr.length() != 10 || dataStr[2] != '.' || dataStr[5] != '.') {
        throw invalid_argument("Format invalid! Trebuie sa fie DD.MM.YYYY");
    }

    try {
        zi = stoi(dataStr.substr(0, 2));
        luna = stoi(dataStr.substr(3, 2));
        an = stoi(dataStr.substr(6, 4));
    } catch (...) {
        throw invalid_argument("String-ul contine caractere care nu sunt cifre.");
    }

    if (!esteValida()) {
        throw out_of_range("Data extrasa nu exista in calendar: " + dataStr);
    }
}

// --- VALIDARE ---

bool Data::esteValida() const {

    if (an < 1 || luna < 1 || luna > 12 || zi < 1) 
            return false;

    int zilePeLuna[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    //verific anu bisect si pun 29 de zile pt februarie 
    if ((an % 4 == 0 && an % 100 != 0) || (an % 400 == 0)) {
        zilePeLuna[2] = 29;
        }
        
        return zi <= zilePeLuna[luna];
}

// --- GETTERI ---

int Data::getZi() const { 
    return zi; 
}
int Data::getLuna() const {
     return luna; 
}
int Data::getAn() const { 
    return an; 
}

int Data::getVarsta() const {

    Data azi = Data::dataCurenta();
    int varsta = azi.an - this->an;

    // Daca inca nu e ziua in anu respectiv!
    if (azi.luna < this->luna || (azi.luna == this->luna && azi.zi < this->zi)) {
        varsta--;
    }
    return (varsta < 0) ? 0 : varsta;
}

// --- OPERATORI ---

bool Data::operator==(const Data& d) {
    return (an == d.an && luna == d.luna && zi == d.zi);
}

bool Data::operator<(const Data& d) {
    if (an != d.an) return an < d.an;
    if (luna != d.luna) return luna < d.luna;
    return zi < d.zi;
}

bool Data::operator>(const Data& d) {
    // Daca nu e mai mica si nici egala, inseamna ca e mai mare
    return !(*this < d || *this == d);
}

// Operatorul de afisare (cu <iomanip> pentru zerouri)
ostream& operator<<(ostream& dev, const Data& d) {
    dev << setfill('0') << setw(2) << d.zi << "."
       << setfill('0') << setw(2) << d.luna << "."
       << d.an;
    return dev;
}

// --- STATIC (Data Curenta) ---

Data Data::dataCurenta() {
    auto azi = chrono::system_clock::now();
    time_t timp_c = chrono::system_clock::to_time_t(azi);
    
    struct tm* t = localtime(&timp_c);

    return Data(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}