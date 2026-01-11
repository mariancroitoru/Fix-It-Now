#include "CerereFactory.h"
#include "ElectrocasnicFactory.h"
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <iostream>

using namespace std;

vector<string> CerereFactory::splitLine(const string& linie, char delimitator) {
    vector<string> tokens;
    stringstream ss(linie);
    string token;
    while (getline(ss, token, delimitator)) {
        if (!token.empty() && token.back() == '\r') {
            token.pop_back();
        }
        tokens.push_back(token);
    }
    return tokens;
}

unique_ptr<CerereReparatie> CerereFactory::creeazaDinCSV(const string& linie) {
    if (linie.empty()) return nullptr;

    vector<string> tokens = splitLine(linie, ',');
    
    if (tokens.size() < 6) {
        throw invalid_argument("Linie cerere incompleta (lipsesc date): " + linie);
    }

    int complexitate = 0;
    try {
        complexitate = stoi(tokens.back());
    } catch (...) {
        throw invalid_argument("Complexitatea nu este valida: " + tokens.back());
    }

    string linieElectrocasnic = "";
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        linieElectrocasnic += tokens[i];
        if (i < tokens.size() - 2) linieElectrocasnic += ",";
    }

    auto aparat = ElectrocasnicFactory::creeazaDinCSV(linieElectrocasnic);
    if (!aparat) {
        throw runtime_error("Nu s-a putut crea aparatul din cerere.");
    }

    time_t now = time(nullptr);

    return make_unique<CerereReparatie>(move(aparat), now, complexitate);
}