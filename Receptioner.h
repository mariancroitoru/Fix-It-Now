#pragma once

#include "Angajat.h"
#include <string>
#include <vector>

class Receptioner: public Angajat{

    std::vector<int> idCereriInregistrate;

    public:
        Receptioner()=default;
        Receptioner(const std::string&, const std::string&, const std::string&, const Data& ,const std::string&);
        void adaugaCerere(int );
        const std::vector<int>& getCereri() const;
        double calculeazaSalariu() const override;
        void afisare(std::ostream&) const override;
        std::string getTipAngajat() const override;
        
};