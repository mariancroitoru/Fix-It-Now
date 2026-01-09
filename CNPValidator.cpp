#include "CNPValidator.h"
#include "Data.h"
#include <array>
#include <string>


using namespace std;


bool CNPValidator::esteValid(const string& cnp) {
    if (cnp.length() != 13) return false;

    for (char c : cnp) {
        if (!isdigit(c)) return false;
    }

    try {
            Data d = getDataNasterii(cnp);
        } catch (...) {
            return false;
        }

    int jj = stoi(cnp.substr(7, 2));
    if (!((jj >= 1 && jj <= 46) || jj == 51 || jj == 52)) 
        return false;

        return verificaCifreControl(cnp);
}

Data CNPValidator::getDataNasterii(const string& cnp) {

    int s = cnp[0] - '0';
    int aa = stoi(cnp.substr(1, 2));
    int ll = stoi(cnp.substr(3, 2));
    int zz = stoi(cnp.substr(5, 2));

    int secol = 0;
    switch (s) {
        case 1: case 2: secol = 1900; break;
        case 3: case 4: secol = 1800; break;
        case 5: case 6: secol = 2000; break;
        case 7: case 8: case 9: secol = 1900; break;
        default: throw invalid_argument("Secol invalid! PACIENT DECEDAT");
    }

    return Data(zz, ll, secol + aa);

}

char CNPValidator::getSex(const string& cnp) {
    int s = cnp[0] - '0';
    return (s % 2 == 1) ? 'M' : 'F';
}


bool CNPValidator::verificaCifreControl(const string& cnp) {

    const array<int, 12> cheie = {2, 7, 9, 1, 4, 6, 3, 5, 8, 2, 7, 9};
    int suma = 0;

    for (int i = 0; i < 12; i++) {
        suma += (cnp[i] - '0') * cheie[i];
    }

    int rest = suma % 11;
    int c_calc = (rest == 10) ? 1 : rest;

    return c_calc == (cnp[12] - '0');
}

bool CNPValidator::verificaData(int an, int luna, int zi) {
    try {
            Data d(zi, luna, an);
            return true;
       } catch (...) {
            return false;
    }

}