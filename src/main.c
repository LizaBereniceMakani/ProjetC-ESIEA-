#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/grandentier.h"

int main() {
    char saisie[2048];
    int valide = 0;

    while (!valide) {
        printf("Entrez le nombre (entier, positif ou negatif) : ");
        if (scanf("%2047s", saisie) != 1) {
            printf(" Erreur de lecture. Reessayez.\n");
            // vider le buffer au cas où
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        int i = 0;
        if (saisie[0] == '-' || saisie[0] == '+') i = 1;

        valide = 1;
        for (; saisie[i] != '\0'; i++) {
            if (!isdigit((unsigned char)saisie[i])) {
                printf(" Saisie invalide : '%s' n'est pas un nombre entier.\n", saisie);
                valide = 0;
                break;
            }
        }

        if (!valide) continue;

        // Vérifier qu’il ne s’agit pas juste d’un signe sans chiffre

        if (strlen(saisie) == 1 && (saisie[0] == '-' || saisie[0] == '+')) {
            printf(" Vous devez entrer au moins un chiffre apres le signe.\n");
            valide = 0;
        }
    }

    // Création et affichage du GrandEntier
    Grandentier *g = ge_creer(saisie);
    if (!g) {
        printf(" Erreur lors de la creation du GrandEntier.\n");
        return 1;
    }

    printf("\n Resultat :\n");
    printf("Representation binaire : ");
    ge_afficher(g);

    printf("Taille : ");
    ge_afficher_taille(g);

    ge_afficher_signe(g);

    ge_liberer(g);
    return 0;
}
