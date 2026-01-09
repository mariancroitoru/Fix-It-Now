#pragma once
#include "Data.h"
#include <string>

class CNPValidator {
public:
    static bool esteValid(const std::string& cnp);
    static Data getDataNasterii(const std::string& cnp);
    static char getSex(const std::string& cnp);

private:

    static bool verificaCifreControl(const std::string& cnp);
    static bool verificaData(int an, int luna, int zi);
    
    CNPValidator() = delete; 
};