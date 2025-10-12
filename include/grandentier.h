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

// FONCTIONS SOUSTRACTION & DIVISION
Grandentier* add_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* sous_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* div_GrandEntier(const Grandentier *a, const Grandentier *b);

// Fonctions d'aide (peuvent rester static dans le .c)
Grandentier* ge_copier(const Grandentier* src);
Grandentier* ge_etendre(const Grandentier* ge, int nouvelle_taille);
Grandentier* ge_normaliser(const Grandentier* ge);
int ge_comparer_abs(const Grandentier *a, const Grandentier *b);


#endif //PROJECT_C_ESIEA__GRANDENTIER_H

