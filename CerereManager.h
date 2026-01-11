#pragma once
#include <vector>
#include <queue>
#include <memory>
#include <string>
#include "CerereReparatie.h"
#include "ElectrocasniceManager.h"

class CerereManager {
private:
    std::vector<std::unique_ptr<CerereReparatie>> toateCererile;

    std::queue<CerereReparatie*> coadaAsteptare;
    std::vector<CerereReparatie*> cereriInLucru;
    std::vector<CerereReparatie*> cereriFinalizate;

public:
    CerereManager() = default;

    void incarcaDinCSV(const std::string& fisier, ElectrocasniceManager& catalog);
    void adaugaCerere(std::unique_ptr<CerereReparatie> c);

    CerereReparatie* getUrmatoareaInAsteptare();
    void popAsteptare();
    void puneInapoiInAsteptare(CerereReparatie* c);
    bool areCereriInAsteptare() const;

    void repartizeazaCerere(CerereReparatie* c, int idTehnician);
    void mutaInFinalizate(CerereReparatie* c);

    std::vector<CerereReparatie*>& getCereriInLucru();
    bool areCereriActive() const;
    void afiseazaStatisticiCurente() const;



    std::vector<int> getIdsInAsteptare() const;

    void genereazaRaportCereriInAsteptare(const std::string& numeFisier) const;


    void afiseazaIstoricReparatii() const;


};