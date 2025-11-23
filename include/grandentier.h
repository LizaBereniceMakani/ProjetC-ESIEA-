#ifndef GRANDENTIER_H
#define GRANDENTIER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BASE 2

typedef struct {
    int *Tdigits;    // Tdigits[0] = MSB (bit de poids fort), Tdigits[Taille-1] = LSB
    int Taille;      // Nombre de bits significatifs
    int Signe;       // +1 positif, -1 négatif, 0 nul
} GrandEntier;

// Création et destruction
GrandEntier* creerGrandEntier(int taille);
GrandEntier* creerGrandEntierDepuisChaine(const char *chaine_binaire);
GrandEntier* creerGrandEntierZero();
GrandEntier* copierGrandEntier(const GrandEntier *ge);
void libererGrandEntier(GrandEntier *ge);

// Affichage
void afficherGrandEntier(const GrandEntier *ge);
void afficherGrandEntierDetail(const GrandEntier *ge);

// Opérations de base (Phase 1)
GrandEntier* additionGrandEntier(const GrandEntier *a, const GrandEntier *b);
GrandEntier* soustractionGrandEntier(const GrandEntier *a, const GrandEntier *b);
int comparerGrandEntier(const GrandEntier *a, const GrandEntier *b);

// Opérations utilitaires
void diviserParDeux(GrandEntier *ge);
GrandEntier* multiplicationEgyptienne(const GrandEntier *a, const GrandEntier *b);

// Phase 2 - Fonctions avancées
GrandEntier* pgcdBinaire(const GrandEntier *a, const GrandEntier *b);
GrandEntier* moduloGrandEntier(const GrandEntier *a, const GrandEntier *b);
GrandEntier* exponentiationModulaire(const GrandEntier *base, unsigned int exposant, const GrandEntier *mod);

// Phase 3 - Bonus RSA
GrandEntier* chiffrementRSA(const GrandEntier *message, unsigned int e, const GrandEntier *n);
GrandEntier* dechiffrementRSA(const GrandEntier *chiffre, unsigned int d, const GrandEntier *n);

#endif