#pragma once

#include "Angajat.h"
#include <string>
#include <vector>


class Supervizor: public Angajat{

    static constexpr double SPOR_CONDUCERE_PROCENT=20.0;

    public:

        Supervizor()=default;
        Supervizor(const std::string&, const std::string&, const std::string&, const Data&, const std::string&);
        double calculeazaSalariu() const override;
        void afisare(std::ostream& ) const override;
        std::string getTipAngajat() const override;


        



};