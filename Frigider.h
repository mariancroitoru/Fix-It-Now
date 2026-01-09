#include "Electrocasnic.h"
#include <string>
#include <memory>


class Frigider: public Electrocasnic{

    bool areCongelator;

public:
    Frigider()=default;
    Frigider(const std::string&, const std::string&, int, double, bool);
    bool getAreCongelator() const;
    void afisare(std::ostream &) const override;
    std::unique_ptr<Electrocasnic> cloneaza() const override;

};