#pragma once
#include <iostream>
#include <string>
#include <ostream>
#include <istream>  

class Data {
    int zi, luna, an;

public:
    Data()=default;
    Data(int, int, int);
    Data(const std::string& ); // "DD.MM.YYYY"

    // VALIDARI
    bool esteValida() const;


    //getteri   
    int getVarsta() const;
    int getZi() const;
    int getLuna() const;
    int getAn() const;

    //operatori
    bool operator<(const Data&) const;
    bool operator>(const Data&) const;
    bool operator==(const Data&) const;
    friend std::ostream& operator<<(std::ostream&, const Data&);

    //static
    static Data dataCurenta();

    // string toString() const



};