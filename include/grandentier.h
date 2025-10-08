#ifndef PROJECT_C_ESIEA__GRANDENTIER_H
#define PROJECT_C_ESIEA__GRANDENTIER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BASE 2

typedef struct {
    int *Tdigts;  // Tableau de bits
    int Taille;   // Nombre de bits
    int Signe;    // 1 = positif, -1 = négatif, 0 = zéro
} Grandentier;

// Créer un Grandentier à partir d'une chaîne décimale
Grandentier* ge_creer(const char *str);

// Créer un Grandentier directement à partir d'une chaîne binaire
Grandentier* ge_creer_from_binary(const char *str);

// Affichage et gestion
void ge_afficher(const Grandentier *g);
void ge_afficher_taille(const Grandentier *g);
void ge_afficher_signe(const Grandentier *g);
void ge_liberer(Grandentier *g);

#endif //PROJECT_C_ESIEA__GRANDENTIER_H

