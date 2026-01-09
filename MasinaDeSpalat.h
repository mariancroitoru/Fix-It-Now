#pragma once
#include "Electrocasnic.h"

#include  <string>
#include <memory>

class MasinaDeSpalat: public Electrocasnic {

    double capacitate; // in kg

public:
    MasinaDeSpalat()=default;
    MasinaDeSpalat(const std::string& , const std::string&, int,double,double );
    double getCapacitate() const;
    void afisare(std::ostream&) const override;
    std::unique_ptr<Electrocasnic> cloneaza() const override;

};