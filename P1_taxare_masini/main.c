#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNG_NUMAR 16
#define LUNG_ZONA  16
#define NR_ZONE    4

// Zonele sunt tinute ca numere: 0 = Exterior, 1 = Periferie, 2 = Oras, 3 = Centrala.
// Este mai usor sa le facem managementul daca le tinem drept valori intregi vs siruri
const char *NUME_ZONE[NR_ZONE] = {"Exterior", "Periferie", "Oras", "Centrala"};

typedef struct {
    char numar[LUNG_NUMAR];
    int zona;
} Abonament;

typedef struct {
    int id;
    int zonaIntrare;          // zona in care intra vehiculul
    int zonaIesire;           // zona din care iese
} Camera;

typedef struct {
    char numar[LUNG_NUMAR];
    int idCamera;
    int ora, minut, secunda;
} Eveniment;

//aceasta functie transforma zona di text in numar
int zonaDinText(const char *text) {
    for (int i = 0; i < NR_ZONE; i++)
        if (strcmp(text, NUME_ZONE[i]) == 0)
            return i;
    return 0;
}

//transformarea din ora minut secunda intr-un singur numar
int inSecunde(const Eveniment *e) {
    return e->ora * 3600 + e->minut * 60 + e->secunda;
}

//gaseste zona unui abonament
int zonaAbonament(const Abonament *abon, int nrAbon, const char *numar) {
    for (int i = 0; i < nrAbon; i++)
        if (strcmp(abon[i].numar, numar) == 0)
            return abon[i].zona;
    return 0;
}

// gaseste o camera
Camera *cautaCamera(Camera *camere, int nrCam, int id) {
    for (int i = 0; i < nrCam; i++)
        if (camere[i].id == id)
            return &camere[i];
    return NULL;
}

//###########
// CITIRE / AFISARE ABONAMENTE
//
// atentie: aici am facut functii separate pentru citire si afisare
//          de ce? este vreun avantaj in a le face separate in general (sau in cazul acestei probleme)?
//
// atentie: exista 2 modalitati de a scrie functiile de citire pentru aceasta problema:
//          varianta 1: Abonament *citesteAbonamente(int *n) 
//          varianta 2: void citesteAbonamente(Abonament **abon, int *n)
//          le veti vedea pe amandoua (pentru abonamente avem varianta 1, pentru celelalte 2)
//          vom discuta la curs daca exista sau nu diferente esentiale intre ele
//###########
void citesteAbonamente(Abonament **abon, int *n) {
    char zona[LUNG_ZONA];
    scanf("%d", n);
    *abon = malloc(*n * sizeof(Abonament));
    for (int i = 0; i < *n; i++) {
        scanf("%15s %15s", (*abon)[i].numar, zona);
        (*abon)[i].zona = zonaDinText(zona);
    }
}

void afiseazaAbonamente(const Abonament *abon, int n) {
    for (int i = 0; i < n; i++)
        printf("%s %s\n", abon[i].numar, NUME_ZONE[abon[i].zona]);
}

//###########
// CITIRE / AFISARE CAMERE
//###########
Camera * citesteCamere(int *n) {
    char intrare[LUNG_ZONA], iesire[LUNG_ZONA];
    scanf("%d", n);
    Camera *camere = malloc(*n * sizeof(Camera));
    for (int i = 0; i < *n; i++) {
        scanf("%d %15s %15s", &camere[i].id, intrare, iesire);
        camere[i].zonaIntrare = zonaDinText(intrare);
        camere[i].zonaIesire  = zonaDinText(iesire);
    }
    return camere;
}

void afiseazaCamere(const Camera *camere, int n) {
    for (int i = 0; i < n; i++)
        printf("%d %s %s\n", camere[i].id,
               NUME_ZONE[camere[i].zonaIntrare], NUME_ZONE[camere[i].zonaIesire]);
}

//###########
// CITIRE / AFISARE EVENIMENTE
// atentie: nu stim dinainte cate evenimente avem deci malloc va trebui facut cu un numar oarecare
// similar, realloc dubleaza capacitatea
// vom discuta despre asta cand ajungem la LISTE
// fara LISTE aceasta rezolvare nu este optima
//###########
Eveniment * citesteEvenimente(int *n) {
    int capacitate = 8;
    Eveniment *ev = malloc(capacitate * sizeof(Eveniment));
    Eveniment e;
    *n = 0;
    while (scanf("%15s", e.numar) == 1 && strcmp(e.numar, "0") != 0) {
        scanf("%d %d %d %d", &e.idCamera, &e.ora, &e.minut, &e.secunda);
        if (*n == capacitate) {
            capacitate *= 2;
            ev = realloc(ev, capacitate * sizeof(Eveniment));
        }
        ev[*n] = e;
        (*n)++;
    }
    return ev;
}

void afiseazaEvenimente(const Eveniment *ev, int n) {
    for (int i = 0; i < n; i++)
        printf("%s %d %02d:%02d:%02d\n", ev[i].numar, ev[i].idCamera,
               ev[i].ora, ev[i].minut, ev[i].secunda);
}

//###########
// afisare evenimente de pe camera x
//###########
void afiseazaEvenimenteCamera(const Eveniment *ev, int n, int idCamera) {
    for (int i = 0; i < n; i++)
        if (ev[i].idCamera == idCamera)
            printf("%s %02d:%02d:%02d\n", ev[i].numar,
                   ev[i].ora, ev[i].minut, ev[i].secunda);
}

//###########
// calcul taxe
//###########
int taxaVehicul(const char *numar, int zonaAbon,
                const Eveniment *ev, int nrEv, Camera *camere, int nrCam) {
    int zonaCurenta = 0, timpAnterior = 0, taxa = 0;
    for (int i = 0; i < nrEv; i++) {
        if (strcmp(ev[i].numar, numar) != 0)
            continue;
        int timp = inSecunde(&ev[i]);
        if (zonaCurenta > zonaAbon)
            taxa += (zonaCurenta - zonaAbon) * (timp - timpAnterior);
        Camera *c = cautaCamera(camere, nrCam, ev[i].idCamera);
        if (c != NULL)
            zonaCurenta = c->zonaIntrare;
        timpAnterior = timp;
    }
    return taxa;
}

int aparutInainte(const Eveniment *ev, int poz) {
    for (int i = 0; i < poz; i++)
        if (strcmp(ev[i].numar, ev[poz].numar) == 0)
            return 1;
    return 0;
}

void afiseazaTaxe(const Abonament *abon, int nrAbon,
                  const Eveniment *ev, int nrEv, Camera *camere, int nrCam) {
    for (int i = 0; i < nrEv; i++) {
        if (aparutInainte(ev, i))
            continue;                       // fiecare masina se trateaza o singura data (adica prima data)
        int zonaAbon = zonaAbonament(abon, nrAbon, ev[i].numar);
        int taxa = taxaVehicul(ev[i].numar, zonaAbon, ev, nrEv, camere, nrCam);
        if (taxa > 0)
            printf("%s %.1f\n", ev[i].numar, taxa / 10.0);
    }
}


int main(void) {
    int test, idCamera;
    int nrAbon = 0, nrCam = 0, nrEv = 0;
    Abonament *abon = NULL;
    Camera *camere = NULL;
    Eveniment *ev = NULL;

    scanf("%d", &test);

    switch (test) {
        case 1:  
            citesteAbonamente(&abon, &nrAbon);
            afiseazaAbonamente(abon, nrAbon);
            break;
        case 2:
            camere = citesteCamere(&nrCam);
            afiseazaCamere(camere, nrCam); 
            break;
        case 3: 
            citesteAbonamente(&abon, &nrAbon);
            camere = citesteCamere(&nrCam);
            ev = citesteEvenimente(&nrEv);
            afiseazaEvenimente(ev, nrEv); 
            break;
        case 4:
            citesteAbonamente(&abon, &nrAbon);
            camere = citesteCamere(&nrCam);
            ev = citesteEvenimente(&nrEv); 
            scanf("%d", &idCamera);
            afiseazaEvenimenteCamera(ev, nrEv, idCamera); 
            break;
        case 5:
            citesteAbonamente(&abon, &nrAbon);
            camere = citesteCamere(&nrCam);
            ev = citesteEvenimente(&nrEv); 
            afiseazaTaxe(abon, nrAbon, ev, nrEv, camere, nrCam); 
            break;
    }

    free(abon);
    free(camere);
    free(ev);
    return 0;
}