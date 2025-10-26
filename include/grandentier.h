#ifndef GRANDENTIER_H
#define GRANDENTIER_H

#include <stddef.h>

typedef struct {
    int *Tdigts;   // Tableau de bits (0 ou 1)
    int Taille;    // Nombre de bits
    int Signe;     // 1 = positif, 0 = zéro, -1 = négatif
} Grandentier;

// Création
Grandentier* ge_creer(const char *str);             // depuis un nombre décimal
Grandentier* ge_creer_from_binary(const char *str); // depuis une chaîne binaire
Grandentier* ge_creer_puissance(const char *str);   // base^exposant (ex: "2^100")
Grandentier* ge_creer_auto(const char *input);      // détection automatique

// Affichage
void ge_afficher(const Grandentier *g);
void ge_afficher_taille(const Grandentier *g);
void ge_afficher_signe(const Grandentier *g);

// Libération
void ge_liberer(Grandentier *g);

// Opérations arithmétiques
Grandentier* add_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* sous_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* mul_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* div_GrandEntier(const Grandentier *a, const Grandentier *b);

// Comparaison (-1 si a<b, 0 si a==b, 1 si a>b)
int cmp_GrandEntier(const Grandentier *a, const Grandentier *b);

// PGCD - Algorithme binaire d'Euclide
Grandentier* ge_pgcd(const Grandentier *a, const Grandentier *b);


#endif // GRANDENTIER_H
