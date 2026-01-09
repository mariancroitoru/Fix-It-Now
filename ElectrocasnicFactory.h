#pragma once
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "Electrocasnic.h"

class ElectrocasnicFactory {
public:
    
    static std::unique_ptr<Electrocasnic> creeazaElectrocasnic(const std::string& tip,const std::string& marca,const std::string& model,int anFabricatie,double pretCatalog,const std::map<std::string, std::string>& dateSpecifice);

    static std::map<std::string, std::string> parseazaDateSpecifice(const std::string& linie);
};