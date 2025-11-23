#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/grandentier.h"

void afficherMenuPrincipal();
void menuPhase1();
void menuPhase2();
void menuPhase3();

GrandEntier* saisirGrandEntier(const char* prompt) {
    char input[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0) continue;

        int valide = 1;
        int start = 0;

        // Permettre le signe '-' au début
        if (input[0] == '-') {
            start = 1;
            if (strlen(input) == 1) {
                valide = 0;
            }
        }

        // Vérifier que le reste est bien binaire
        for (int i = start; input[i]; i++) {
            if (input[i] != '0' && input[i] != '1') {
                valide = 0;
                break;
            }
        }

        if (!valide) {
            printf("Erreur: Format invalide. Utilisez: -1010 ou 1010\n");
            continue;
        }

        GrandEntier* ge = creerGrandEntierDepuisChaine(input);
        if (!ge) {
            printf("Erreur de creation du GrandEntier\n");
            continue;
        }
        return ge;
    }
}

void afficherResultat(const GrandEntier *resultat, const char *operation) {
    printf("\n=== %s ===\n", operation);
    if (resultat) {
        afficherGrandEntierDetail(resultat);
    } else {
        printf("Erreur lors du calcul\n");
    }
    printf("====================\n\n");
}

void menuPhase1() {
    int choix;
    do {
        printf("\n*** PHASE 1 - OPERATIONS DE BASE ***\n");
        printf("1. Creer et afficher un GrandEntier\n");
        printf("2. Addition\n");
        printf("3. Soustraction\n");
        printf("4. Comparaison\n");
        printf("5. Retour au menu principal\n");
        printf("Choix: ");

        scanf("%d", &choix);
        getchar();

        switch(choix) {
            case 1: {
                GrandEntier *ge = saisirGrandEntier("Entrez une chaine binaire: ");
                afficherGrandEntierDetail(ge);
                libererGrandEntier(ge);
                break;
            }
            case 2: {
                GrandEntier *a = saisirGrandEntier("Premier nombre binaire: ");
                GrandEntier *b = saisirGrandEntier("Deuxieme nombre binaire: ");
                GrandEntier *resultat = additionGrandEntier(a, b);
                afficherResultat(resultat, "ADDITION");
                libererGrandEntier(a);
                libererGrandEntier(b);
                libererGrandEntier(resultat);
                break;
            }
            case 3: {
                GrandEntier *a = saisirGrandEntier("Premier nombre binaire: ");
                GrandEntier *b = saisirGrandEntier("Deuxieme nombre binaire: ");
                GrandEntier *resultat = soustractionGrandEntier(a, b);
                afficherResultat(resultat, "SOUSTRACTION");
                libererGrandEntier(a);
                libererGrandEntier(b);
                libererGrandEntier(resultat);
                break;
            }
            case 4: {
                GrandEntier *a = saisirGrandEntier("Premier nombre binaire: ");
                GrandEntier *b = saisirGrandEntier("Deuxieme nombre binaire: ");
                int cmp = comparerGrandEntier(a, b);
                if (cmp == 0) {
                    printf("Les nombres sont EGAUX\n");
                } else if (cmp == -1) {
                    printf("Le premier nombre est PLUS PETIT\n");
                } else {
                    printf("Le premier nombre est PLUS GRAND\n");
                }
                libererGrandEntier(a);
                libererGrandEntier(b);
                break;
            }
            case 5:
                return;
            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 5);
}

void menuPhase2() {
    int choix;
    do {
        printf("\n*** PHASE 2 - FONCTIONS AVANCEES ***\n");
        printf("1. PGCD Binaire\n");
        printf("2. Modulo\n");
        printf("3. Exponentiation Modulaire\n");
        printf("4. Multiplication Egyptienne\n");
        printf("5. Retour au menu principal\n");
        printf("Choix: ");

        scanf("%d", &choix);
        getchar();

        switch(choix) {
            case 1: {
                GrandEntier *a = saisirGrandEntier("Premier nombre binaire: ");
                GrandEntier *b = saisirGrandEntier("Deuxieme nombre binaire: ");
                GrandEntier *resultat = pgcdBinaire(a, b);
                afficherResultat(resultat, "PGCD BINAIRE");
                libererGrandEntier(a);
                libererGrandEntier(b);
                libererGrandEntier(resultat);
                break;
            }
            case 2: {
                GrandEntier *a = saisirGrandEntier("Nombre: ");
                GrandEntier *b = saisirGrandEntier("Modulo: ");
                GrandEntier *resultat = moduloGrandEntier(a, b);
                afficherResultat(resultat, "MODULO");
                libererGrandEntier(a);
                libererGrandEntier(b);
                libererGrandEntier(resultat);
                break;
            }
            case 3: {
                GrandEntier *base = saisirGrandEntier("Base: ");
                GrandEntier *mod = saisirGrandEntier("Modulo: ");
                unsigned int exp;
                printf("Exposant (entier non signe): ");
                scanf("%u", &exp);
                getchar();

                GrandEntier *resultat = exponentiationModulaire(base, exp, mod);
                afficherResultat(resultat, "EXPONENTIATION MODULAIRE");
                libererGrandEntier(base);
                libererGrandEntier(mod);
                libererGrandEntier(resultat);
                break;
            }
            case 4: {
                GrandEntier *a = saisirGrandEntier("Premier facteur: ");
                GrandEntier *b = saisirGrandEntier("Deuxieme facteur: ");
                GrandEntier *resultat = multiplicationEgyptienne(a, b);
                afficherResultat(resultat, "MULTIPLICATION EGYPTIENNE");
                libererGrandEntier(a);
                libererGrandEntier(b);
                libererGrandEntier(resultat);
                break;
            }
            case 5:
                return;
            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 5);
}

void menuPhase3() {
    int choix;
    do {
        printf("\n*** PHASE 3 - BONUS RSA ***\n");
        printf("1. Chiffrement RSA\n");
        printf("2. Déchiffrement RSA\n");
        printf("3. Retour au menu principal\n");
        printf("Choix: ");

        scanf("%d", &choix);
        getchar();

        switch(choix) {
            case 1: {
                GrandEntier *message = saisirGrandEntier("Message à chiffrer: ");
                GrandEntier *n = saisirGrandEntier("Module RSA (n): ");
                unsigned int e;
                printf("Exposant public e: ");
                scanf("%u", &e);
                getchar();

                GrandEntier *chiffre = chiffrementRSA(message, e, n);
                afficherResultat(chiffre, "CHIFFREMENT RSA");
                libererGrandEntier(message);
                libererGrandEntier(n);
                libererGrandEntier(chiffre);
                break;
            }
            case 2: {
                GrandEntier *chiffre = saisirGrandEntier("Message chiffre: ");
                GrandEntier *n = saisirGrandEntier("Module RSA (n): ");
                unsigned int d;
                printf("Exposant prive d: ");
                scanf("%u", &d);
                getchar();

                GrandEntier *message = dechiffrementRSA(chiffre, d, n);
                afficherResultat(message, "DECHIFFREMENT RSA");
                libererGrandEntier(chiffre);
                libererGrandEntier(n);
                libererGrandEntier(message);
                break;
            }
            case 3:
                return;
            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 3);
}

void afficherMenuPrincipal() {
    int choix;
    do {
        printf("\n========== GRANDS ENTIERS BINAIRES ==========\n");
        printf("1. Phase 1 - Operations de base\n");
        printf("2. Phase 2 - Fonctions avancees\n");
        printf("3. Phase 3 - Bonus RSA\n");
        printf("4. Quitter\n");
        printf("Choix: ");

        scanf("%d", &choix);
        getchar();

        switch(choix) {
            case 1:
                menuPhase1();
                break;
            case 2:
                menuPhase2();
                break;
            case 3:
                menuPhase3();
                break;
            case 4:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 4);
}

int main() {
    printf("=== PROJET GRANDS ENTIERS BINAIRES ===\n");
    printf("Implementation conforme aux specifications\n\n");

    afficherMenuPrincipal();

    return 0;
}