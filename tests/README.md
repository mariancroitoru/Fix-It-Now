# Fișiere de Test - FixItNow Service

## Structura Folderului

```
tests/
├── README.md                          (acest fișier)
├── angajati_validi.csv               (angajați corecți)
├── angajati_invalizi.csv             (angajați cu erori)
├── catalog_electrocasnice.csv        (modele reparabile)
├── cereri_valide.csv                 (cereri pentru aparate din catalog)
├── cereri_invalide.csv               (cereri pentru aparate nereparabile)
├── scenario_incarcare_echilibrata.csv (test pentru alocarea echilibrată)
└── scenario_cazuri_limita.csv        (test cazuri extreme)
```

## Descrierea Fișierelor

### 1. `angajati_validi.csv`
**Scop**: Testează adăugarea corectă a angajaților cu date valide.

**Format**: `Tip,Nume,Prenume,CNP,DataAngajarii,OrasDomiciliu,[Specializari]`

**Conținut**:
- Minim 3 tehnicieni cu specializări diverse
- Minim 1 recepționer
- Minim 1 supervizor
- Vârste diferite (16+)
- Orașe diferite (București și alte orașe pentru testarea primei de transport)

**Exemplu linie**:
```
Tehnician,Popescu,Ion,1850203123456,15.03.2020,Cluj,TV:Samsung;TV:LG;Frigider:Samsung
```

**Cazuri testate**:
- ✓ CNP valid românesc (cu cifră de control corectă)
- ✓ Vârstă >= 16 ani la data angajării
- ✓ Nume și prenume între 3-30 caractere
- ✓ Date calendaristice valide
- ✓ Specializări multiple pentru tehnicieni

---

### 2. `angajati_invalizi.csv`
**Scop**: Testează validarea și raportarea erorilor pentru date incorecte.

**Cazuri testate**:
- ✗ CNP invalid (lungime greșită, cifră de control eronată)
- ✗ Vârstă < 16 ani la angajare
- ✗ Nume/prenume prea scurt (< 3 caractere)
- ✗ Nume/prenume prea lung (> 30 caractere)
- ✗ Date invalide (ex: 30.02.2020, 32.01.2020)
- ✗ Format CSV incorect (lipsă câmpuri)

**Așteptat**: Programul continuă execuția și raportează fiecare eroare cu:
```
Eroare la citire: Angajat invalid pe linia X, cauza: [descriere eroare]
```

---

### 3. `catalog_electrocasnice.csv`
**Scop**: Populează catalogul cu modele reparabile.

**Format**: `Tip,Marca,Model,AnFabricatie,PretCatalog,DateSpecifice`

**Date specifice per tip**:
- **Frigider**: `congelator:da` sau `congelator:nu`
- **TV**: `diagonala:55;unitateDiagonala:inch`
- **MasinaDeSpalat**: `capacitate:7.5`

**Exemplu linii**:
```
Frigider,Samsung,Frost200,2020,2500.0,congelator:da
TV,LG,OLED55,2021,3500.0,diagonala:55;unitateDiagonala:inch
MasinaDeSpalat,Whirlpool,WM7,2019,1800.0,capacitate:7
```

**Conținut**: Minim 10-15 modele diferite, acoperind:
- Toate tipurile (Frigider, TV, Mașină de spălat)
- Mărci diverse (Samsung, LG, Bosch, Whirlpool, Arctic, etc.)
- Ani de fabricație variați (2015-2024)
- Specificații diverse

---

### 4. `cereri_valide.csv`
**Scop**: Testează procesarea cererilor pentru aparate din catalog.

**Format**: `Tip,Marca,Model,AnFabricatie,PretCatalog,DateSpec,Timestamp,Complexitate`

**Timestamp**: Format `DD.MM.YYYY HH:MM:SS`

**Complexitate**: 1-5 (unde durata = vechime * complexitate)

**Exemplu linie**:
```
Frigider,Samsung,Frost200,2020,2500.0,congelator:da,09.01.2026 10:30:00,3
```

**Cazuri testate**:
- Cereri cu timestampuri diferite (pentru testarea sortării)
- Complexități variate (1-5)
- Aparate cu vechimi diferite (pentru testarea calculului duratei)
- Distribuție pe tipuri și mărci (pentru testarea alocării la tehnicieni specializați)

**Așteptat**:
- Toate cererile sunt marcate ca VALIDE
- Sunt alocate tehnicienilor potriviți
- Sortarea se face după timestamp

---

### 5. `cereri_invalide.csv`
**Scop**: Testează detectarea și raportarea cererilor pentru aparate nereparabile.

**Conținut**:
- Aparate cu tipuri necunoscute (ex: "Aspirator")
- Mărci care nu există în catalog pentru tipul respectiv
- Modele care nu există în catalog
- Complexitate 0 (aparat declarat ireparabil)

**Exemplu linii**:
```
TV,NoName,Model123,2018,1500.0,diagonala:40;unitateDiagonala:inch,09.01.2026 11:00:00,3
Frigider,Samsung,ModelInexistent,2020,2000.0,congelator:da,09.01.2026 11:15:00,2
```

**Așteptat**:
- Cererile sunt marcate ca INVALIDE
- Sunt adăugate la statistici (aparateNereparabile)
- NU sunt procesate în simulare
- Raportare conține număr de apariții pentru fiecare model

---

### 6. `scenario_incarcare_echilibrata.csv`
**Scop**: Testează criteriul 3 - alocarea echilibrată a tehnicienilor.

**Setup**:
1. Încarcă 3 tehnicieni cu specializări identice (ex: toți pot repara TV Samsung)
2. Încarcă 10+ cereri pentru TV Samsung cu durate similare

**Așteptat**:
- Primele 3 cereri se alocă câte una fiecărui tehnician
- Următoarele cereri se alocă tehnicianului cu cea mai mică durată totală acumulată
- La final, toți tehnicienii au durate aproximativ egale (± 1-2 unități)

**Verificare în output**:
```
[Timp 0] Tehnician 1 primeste cererea 1
         Tehnician 2 primeste cererea 2
         Tehnician 3 primeste cererea 3
[Timp 1] Tehnician 1 primeste cererea 4  (fiindcă are durata minimă)
```

---

### 7. `scenario_cazuri_limita.csv`
**Scop**: Testează situații extreme și edge cases.

**Cazuri incluse**:

a) **Limita 3 cereri active per tehnician**:
   - Tehnician cu 3 cereri active nu primește a 4-a
   - Cererea a 4-a intră în așteptare
   - După finalizarea unei cereri, tehnician primește cerere din așteptare

b) **Tehnician fără specializare potrivită**:
   - Cerere pentru TV LG când toți tehnicienii sunt specializați doar pe Samsung
   - Cererea rămâne în așteptare pe toată durata simulării

c) **Cereri simultane (același timestamp)**:
   - Multiple cereri cu același timestamp
   - Verifică sortarea stabilă

d) **Vechime 0 ani** (aparat fabricat în anul curent):
   - Vechime = 1 (pentru a evita durată 0)
   - Durată = 1 * complexitate

e) **Complexitate maximă (5)**:
   - Aparat vechi (10 ani) + complexitate 5
   - Durată = 50 unități
   - Test pentru reparații de lungă durată

**Format exemplu**:
```
# Test: 3 cereri active + 1 in asteptare
TV,Samsung,OLED55,2021,3500,diagonala:55;unitateDiagonala:inch,09.01.2026 10:00:00,1
TV,Samsung,OLED55,2021,3500,diagonala:55;unitateDiagonala:inch,09.01.2026 10:00:01,1
TV,Samsung,OLED55,2021,3500,diagonala:55;unitateDiagonala:inch,09.01.2026 10:00:02,1
TV,Samsung,OLED55,2021,3500,diagonala:55;unitateDiagonala:inch,09.01.2026 10:00:03,1
```

---

## Rularea Testelor

### Mod manual (din meniu):
1. Start aplicație
2. Meniu 1 → Opțiune 2 → Încarcă `angajati_validi.csv`
3. Meniu 2 → Opțiune 2 → Încarcă `catalog_electrocasnice.csv`
4. Meniu 3 → Opțiune 1 → Încarcă `cereri_valide.csv`
5. Meniu 3 → Opțiune 2 → Start simulare

### Mod automat (sugestie pentru testare rapidă):
Creați un script `test_runner.sh` sau `test_runner.bat`:
```bash
#!/bin/bash
echo "Rulare test complet..."
./FixItNow <<EOF
1
2
tests/angajati_validi.csv
0
2
2
tests/catalog_electrocasnice.csv
0
3
1
tests/cereri_valide.csv
2
0
0
EOF
```

---

## Verificarea Rezultatelor

### 1. Verificare console output:
- ✓ Toate erorile sunt raportate clar
- ✓ Programul continuă după erori
- ✓ Simularea afișează status la fiecare tic
- ✓ Mesajele sunt clare și informative

### 2. Verificare rapoarte CSV:
```bash
cat raport_top3_salarii.csv
cat raport_tehnician_maxim.csv
cat raport_cereri_asteptare.csv
```

### 3. Verificare alocări:
- Tehnicieni primesc doar cereri pentru specializările lor
- Niciun tehnician nu depășește 3 cereri active
- Încărcarea este aproximativ egală între tehnicieni

### 4. Verificare calcule:
- Salariu = BAZĂ + Fidelitate + Transport + Bonus (pentru tehnicieni)
- Durată = Vechime * Complexitate (minim 1)
- Preț = PretCatalog * Durată

---

## Criterii de Succes

✅ **Toate testele trebuie să:**
- Proceseze datele valide corect
- Detecteze și raporteze toate erorile
- Ruleze fără crash-uri
- Producă output corect și consistent
- Respecte toate regulile de business

✅ **Simularea trebuie să:**
- Aloce cererile corect după cei 3 criterii
- Proceseze cererile în timp real
- Finalizeze toate cererile valide
- Mențină cererile invalide în așteptare

✅ **Rapoartele trebuie să:**
- Conțină date corecte
- Fie formatate corect (CSV valid)
- Respecte sortările cerute

---

## Note Importante

- Toate fișierele CSV folosesc virgulă (`,`) ca delimitator
- Liniile care încep cu `#` sunt tratate ca comentarii
- Liniile goale sunt ignorate
- Encoding: UTF-8
- Line endings: LF (Unix) sau CRLF (Windows) - ambele acceptate

---

## Extensii Posibile (Bonus)

- Test pentru concurență (multiple cereri în paralel)
- Test pentru persistență (salvare/încărcare stare)
- Test pentru undo/redo operații
- Benchmark pentru performanță (1000+ cereri)