
#ifndef GRANDENTIER_H
#define GRANDENTIER_H

#include <stdio.h>
#include <stdlib.h>

// Structure pour un grand entier
typedef struct {
    int* Tdigts;    // Tableau des digits binaires
    int Taille;     // Nombre de digits
    int Signe;      // 1 positif, -1 négatif, 0 zéro
} Grandentier;

// ---- Structure pour Euclide étendu ----
typedef struct {
    Grandentier *gcd;
    Grandentier *x;
    Grandentier *y;
} EuclideResult;

// ===== FONCTIONS DE BASE (Phase 1 & 2) =====

// Création
Grandentier* ge_creer(const char *str);
Grandentier* ge_creer_from_binary(const char *str);
Grandentier* ge_creer_puissance(const char *str);
Grandentier* ge_creer_auto(const char *input);

// Affichage
void ge_afficher(const Grandentier *g);
void ge_afficher_taille(const Grandentier *g);
void ge_afficher_signe(const Grandentier *g);

// Mémoire
void ge_liberer(Grandentier *g);

// Opérations arithmétiques
Grandentier* add_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* sous_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* mul_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* div_GrandEntier(const Grandentier *a, const Grandentier *b);
Grandentier* Grandentier_mod(const Grandentier *a, const Grandentier *b);

// Comparaison
int cmp_GrandEntier(const Grandentier *a, const Grandentier *b);

// ===== FONCTIONS AVANCÉES (Phase 3) =====

// ---- Exponentiation et arithmétique modulaire ----
Grandentier* ge_pow_mod(const Grandentier *base, const Grandentier *exp, const Grandentier *mod);

// ---- Algorithmes de théorie des nombres ----
EuclideResult* ge_euclide_etendu(const Grandentier *a, const Grandentier *b);
Grandentier* ge_gcd(const Grandentier *a, const Grandentier *b);
Grandentier* ge_mod_inverse(const Grandentier *a, const Grandentier *m);

// ---- Nombres premiers ----
int ge_est_probablement_premier(const Grandentier *n, int iterations);
Grandentier* ge_generer_premier(int nb_bits);

// ---- Conversions ----
char* ge_vers_hexadecimal(const Grandentier *g);

// ---- Opérations bit-à-bit ----
Grandentier* ge_et_bitwise(const Grandentier *a, const Grandentier *b);
Grandentier* ge_ou_bitwise(const Grandentier *a, const Grandentier *b);
Grandentier* ge_xor_bitwise(const Grandentier *a, const Grandentier *b);
Grandentier* ge_decalage_gauche(const Grandentier *a, int positions);
Grandentier* ge_decalage_droite(const Grandentier *a, int positions);

// ===== FONCTIONS INTERNES (utilitaires) =====

// Copie d'un Grandentier (fonction interne mais utile)
Grandentier* ge_copier(const Grandentier *g);

#endif // GRANDENTIER_H