#pragma once
#include <string>
#include <memory>
#include <vector>
#include "CerereReparatie.h"

class CerereFactory {
public:
    static std::unique_ptr<CerereReparatie> creeazaDinCSV(const std::string& linie);

private:
    static std::vector<std::string> splitLine(const std::string& linie, char delimitator);
};

