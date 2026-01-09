#pragma once
#include <string>
#include <memory>
#include "Data.h"

class Electrocasnic {
protected:
    std::string tip, marca, model;
    int anFabricatie;
    double pretCatalog;

public:
    Electrocasnic() = default;
    Electrocasnic(const std::string& , const std::string& , const std::string& , int , double );

    virtual ~Electrocasnic() = default;
    
    // getteri
    std::string getTip() const;
    std::string getMarca() const;
    std::string getModel() const;
    int getAnFabricatie() const;
    double getPretCatalog() const;
    
    int getVechime() const;

    virtual void afisare(std::ostream& ) const = 0;
    virtual std::unique_ptr<Electrocasnic> cloneaza() const = 0;

    bool operator==(const Electrocasnic& altul) const;

};