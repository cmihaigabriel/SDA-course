/*
Enunt problema:
https://curs.upb.ro/2026/pluginfile.php/138922/mod_resource/content/1/Management_Taxe_Vehicule%20%28pointeri%2Bstructuri%29.pdf

Plan de dezvoltare:
 
1. De ce structuri avem nevoie?
- 

2. De ce functii avem nevoie? Cum le aplicam asupra datelor?

*/
#include <stdio.h>

struct Abonament{
    char numar[15];
    int tip;
} *abonament;

struct Camera{
    int id;
    int zona_intrare;
    int zona_iesire;
} *camera;

struct Eveniment{
    char numar[15];
    int id_camera;
    int secunde;
} *eveniment;

void afisare_abonament(){
    struct Abonament *abonament = 0;
    int nr_abonamente;
    char zona[10];

    scanf("%d", &nr_abonamente);
    abonament = malloc(nr_abonamente * sizeof(struct Abonament));
    for (int i = 0; i < nr_abonamente; i++){
        scanf("%s %s", abonament[i].numar, zona);
        if (strcmp(zona, "Periferie") == 0)
            abonament[i].tip = 1;
        else   
            if (strcmp(zona, "Oras") == 0)
                abonament[i].tip = 2;
            else
                abonament[i].tip = 3;

    }

    for (int i = 0; i < nr_abonamente; i++){
        printf("%s %d\n", abonament[i].numar, abonament[i].tip);
    }
}

int main () {
    printf("testiiiing");
    eveniment = 0;

    int comanda;
    scanf("%d", &comanda);

    if (comanda == 1){
        afisare_abonament();
    }
}