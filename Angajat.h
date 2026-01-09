#pragma once
#include <string>
#include <iostream>
#include "Data.h"

class Angajat{

protected:

    //TODO : gandeste te la o denumire mai buna pentru id urile de la angajat si cerere
    static int nextID;
    const int id;
    std::string nume, prenume, cnp;
    Data dataAngajarii;
    std::string orasDomiciliu;

    static constexpr double SALARIU_BAZA=4000.0;
    static constexpr double BONUS_DE_FIDELITARE_PROCENT=5.0;
    static constexpr int ANI_FIDELITATE=3;
    static constexpr double PRIMA_TRANS=400.0;

public:

    Angajat():id(0){}
    Angajat(const std::string& ,const std::string&, const std::string& , const Data& , const std::string& ); 

    virtual ~Angajat()=default;


    // getteri

    int getId() const;
    std::string getNume() const;
    std::string getPrenume() const;
    std::string getCNP() const;
    Data getDataAngajarii() const;
    std::string getOrasDomiciliu() const;


    //setteri

    void setNume(const std::string&);
    void setPrenume(const std::string& );


    //validare

    void validareNume(const std::string& , const std::string& ) const;


    //calcul salariu-- VIRTUAL PURE
    virtual double calculeazaSalariu() const=0;
    virtual void afisare(std::ostream& ) const;

        //tip angajat-- VIRTUAL PURE
    virtual std::string getTipAngajat() const=0;

    // auxiliare
    double calculeazaBonusFidelitate() const;
    bool primestePrimaTransport() const;

};