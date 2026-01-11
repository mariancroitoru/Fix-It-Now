#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Electrocasnic.h"

class ElectrocasniceManager {
private:
    std::map<std::string, std::map<std::string, std::map<std::string, std::unique_ptr<Electrocasnic>>>> catalog;
    
    std::map<std::string, int> aparateNereparabile;

public:
    ElectrocasniceManager() = default;

    void incarcaDinCSV(const std::string& numeFisier);
    
    void adaugaInCatalog(std::unique_ptr<Electrocasnic> e);

    bool existaModel(const std::string& tip, const std::string& marca, const std::string& model) const;
    const Electrocasnic* getDetaliiModel(const std::string& tip, const std::string& marca, const std::string& model) const;
    

    

    void adaugaModelManual(std::string tip, std::string marca, std::string model, int an, double pret);
    bool stergeModel(std::string marca, std::string model);


    void inregistreazaCerereInvalida(const std::string& tip, const std::string& marca, const std::string& model);
    void afiseazaAparateNereparabile() const;
    void afiseazaCatalog() const;
};