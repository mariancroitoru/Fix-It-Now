

#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "Angajat.h"

class AngajatFactory {
public:
    static std::unique_ptr<Angajat> creeazaAngajat(const std::string& tip, const std::string& nume, const std::string& prenume, const std::string& cnp, const Data& dataAngajarii, const std::string& orasDomiciliu, const std::map<std::string, std::set<std::string>>& specializari = {});
    static std::unique_ptr<Angajat> creeazaDinCSV(const std::string& linie);

private:
    static std::vector<std::string> splitLine(const std::string& linie, char delimitator);    
};