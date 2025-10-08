#ifndef PROJECT_C_ESIEA__GRANDENTIER_H
#define PROJECT_C_ESIEA__GRANDENTIER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BASE 2

typedef struct {
    int *Tdigts;
    int Taille;
    int Signe;
} Grandentier;

// Fonction qui créée un Grandentier à partir d'une chaîne décimale
Grandentier* ge_creer(const char *str);

// Fonction qui créée un Grandentier directement à partir d'une chaîne binaire
Grandentier* ge_creer_from_binary(const char *str);

// Les entetes de toutes nos fonctions
void ge_afficher(const Grandentier *g);
void ge_afficher_taille(const Grandentier *g);
void ge_afficher_signe(const Grandentier *g);
void ge_liberer(Grandentier *g);

#endif //PROJECT_C_ESIEA__GRANDENTIER_H

