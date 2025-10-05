#include "../include/grandentier.h"
// Created by support on 01/10/2025.

// Creer un GrandEntier à partir d'un entier n

Grandentier* ge_creer(int n) {
    Grandentier *g = malloc(sizeof(Grandentier));
    if (!g) return NULL;

            if (n == 0) {
                g->Taille = 1;
                g->Signe = 0;
                g->Tdigts = malloc(sizeof(int));
                g->Tdigts[0] = 0;
                 return g;
            }

            g->Signe = (n > 0) ? 1 : -1;
            int temp= (n > 0) ? n : -n;

            int taille = 0;
            int t = temp;


            while (t>0) {
                t /= 2;
                taille++;
            }

            g->Taille = taille;
            g->Tdigts = malloc(taille * sizeof(int));

            if (!g->Tdigts ) {
                free(g);
                return NULL;
            }

            for (int i = taille - 1; i >= 0; i--) {
                g->Tdigts[i] = temp %2;
                temp /= 2;
            }

    return g;

}


// Afficher le GrandEntier en binaire

void ge_afficher(const Grandentier *g) {

        if (g->Signe == 0) {
            printf("0\n");
            return ;
        }

        if (g->Signe == -1) printf("-");

        for (int i = 0; i < g->Taille; i++) {
            printf("%d", g->Tdigts[i]);
        }
        printf("\n");

}

// Afficher le nombre de bits significatifs

void ge_afficher_taille(const Grandentier *g) {
    printf("%d\n", g->Taille);
}

// Afficher le signe

void ge_afficher_signe( const Grandentier *g) {
    printf("%d\n", g->Signe);
}

// Liberer la memoire

void ge_liberer(Grandentier *g) {
    if (g) {
        if (g->Tdigts) free(g->Tdigts);
    }
}




