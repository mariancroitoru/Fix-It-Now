# FixItNow - Date de Testare

### 1. Cataloage
* **catalog_electrocasnice.csv**: Lista oficială a modelelor acceptate în service.
    * *Coloane:* Tip, Marca, Model, An, Pret, Spec1, Spec2 (opțional).

### 2. Angajați
* **angajati_validi.csv**: Conține personalul minim necesar (3 Tehnicieni, 1 Recepționer, 1 Supervizor) cu **CNP-uri valide** verificate algoritmic.
* **angajati_invalizi.csv**: Conține înregistrări eronate (CNP invalid, dată lipsă, rol necunoscut) pentru a testa robustețea sistemului de încărcare.

### 3. Cereri de Reparație
* **cereri_valide.csv**: Cereri pentru aparate existente în catalog. Acestea ar trebui să fie alocate automat tehnicienilor.
* **cereri_invalide.csv**: Cereri pentru aparate care nu există în catalog (ex: Sony, Indesit). Acestea vor fi respinse și vor apărea în raportul de "Aparate Nereparabile".

## Instrucțiuni
Pentru a rula o simulare completă, încărcați fișierele în următoarea ordine din meniul aplicației:
1.  Catalog: `tests/catalog_electrocasnice.csv`
2.  Angajați: `tests/angajati_validi.csv`
3.  Cereri: `tests/cereri_valide.csv`