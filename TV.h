#pragma once
#include "Electrocasnic.h"

#include <string>
#include <memory>
#include <iostream>

class TV: public Electrocasnic{
    
    double diagonala;
    bool esteInCm; // true=cm si false=inciii

public:
    TV()=default;
    TV(const std::string&, const std::string&,int,double,double,bool);

    double getDiagonala() const;
    bool getEsteInCm() const;
    void afisare(std::ostream&) const override;

    std::unique_ptr<Electrocasnic> cloneaza() const override;

};