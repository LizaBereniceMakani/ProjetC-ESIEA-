#include <stdio.h>
#include "../include/grandentier.h"

int main() {
    int n;
    printf("Entrez un nombre: ");
    scanf("%d", &n);

    Grandentier *g = ge_creer(n);

    ge_afficher(g);
    ge_afficher_taille(g);
    ge_afficher_signe(g);
    ge_liberer(g);
    return 0;
}