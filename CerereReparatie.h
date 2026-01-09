#pragma once 
#include "Electrocasnic.h"
#include "Tehnician.h"
#include <string>
#include <iostream>
#include <memory>
#include <ctime>

enum class StareCerere{

    IN_ASTEPTARE,
    REPARTIZATA,
    IN_LUCRU,
    FINALIZATA,
    INVALIDA
};


    class CerereReparatie {
        //TODO la fel! alta denumire pentru id uri!!
        static int nextID;
         int id;
        std::unique_ptr<Electrocasnic> aparat;
        time_t timestamp;
        int nivelComplexitate;  // 0-5 (0- nu se poatte repara! REMAT!!)
        int durataEstimata;
        int durataRamasa;
        double pretReparatie;
        StareCerere stare;
        int idTehnician; // -1 daca e nerepartizata

        public:
            CerereReparatie()=default;
            CerereReparatie(std::unique_ptr<Electrocasnic> _aparat, const time_t _timestamp, int _nivelComplexitate);


            // constructor de copiere si operatorul egal pentru unique_ptr ????????

            CerereReparatie(const CerereReparatie&);
            CerereReparatie& operator=(const CerereReparatie& );

            //getteri
            int getId() const;
            int getNivelComplexitate() const;
            const Electrocasnic* getAparat() const;
            time_t getTimeStamp() const;
            int getDurataEstimata() const;
            int getDurataRamasa() const;
            double getPretReparatie() const;
            StareCerere getStare() const;
            int getIdTehnician() const;


            // setters

            void setStare(const StareCerere);
            void setIdTehnician(int);


            // Procesare

            void proceseaza(); // reduce durata ramasa
            bool esteFinalizata() const;
            bool esteValida() const;
            
        void afisare(std::ostream&) const;
        std::string getTimeStampString() const;



        //Comparare pentru sortare???? 
        bool operator<(const CerereReparatie& ) const;

    };

