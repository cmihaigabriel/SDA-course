#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////
// Definire structuri - singura idee neobisnuita aici este structura de Program - care
// reprezinta listele imbricate - fiecare nod din lista Program contine 
// o lista de Fisiere
///////////

typedef struct Fisier {
    char nume[256];
    char acces;
    struct Fisier *urm;
} Fisier;

typedef struct Program {
    char nume[256];
    float cpuMax, cpuInst;
    float memMax, memInst;
    Fisier *fisiere;
    struct Program *urm;
} Program;