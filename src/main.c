#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/grandentier.h"

void afficher_menu() {
    printf("\n=== MENU DES OPERATIONS ===\n");
    printf("1. Addition\n");
    printf("2. Soustraction\n");
    printf("3. Division\n");
    printf("4. Toutes les operations\n");
    printf("5. Quitter\n");
    printf("Choix : ");
}

Grandentier* saisir_grandentier(const char* message) {
    char saisie[2048];
    char type;
    int valide = 0;

    printf("\n%s\n", message);

    // Choix du type de saisie
    while (1) {
        printf("Voulez-vous entrer un nombre decimal (taper 'd') ou un nombre binaire (taper 'b') ? ");
        if (scanf(" %c", &type) != 1) continue;
        if (type == 'd' || type == 'D' || type == 'b' || type == 'B') break;
        printf(" Votre choix est invalide, vous devez taper 'd' pour decimal ou 'b' pour binaire.\n");
    }

    // Lecture et vérification de la saisie
    while (!valide) {
        if (type == 'd' || type == 'D') {
            printf("Entrez un nombre decimal (entier, positif ou negatif) svp : ");
        } else {
            printf("Entrez un nombre binaire (suite de 0 et 1) svp : ");
        }

        if (scanf("%2047s", saisie) != 1) {
            printf("Erreur de lecture. Veuillez reessayez svp :\n");
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Vérification selon le type
        valide = 1;
        if (type == 'd' || type == 'D') {
            int i = 0;
            if (saisie[0] == '-' || saisie[0] == '+') i = 1;
            for (; saisie[i] != '\0'; i++) {
                if (!isdigit((unsigned char)saisie[i])) {
                    printf(" votre saisie est invalide : '%s' n'est pas un entier.\n", saisie);
                    valide = 0;
                    break;
                }
            }
            if (strlen(saisie) == 1 && (saisie[0] == '-' || saisie[0] == '+')) valide = 0;
        } else {
            for (int i = 0; saisie[i] != '\0'; i++) {
                if (saisie[i] != '0' && saisie[i] != '1') {
                    printf(" votre saisie est invalide : '%s' n'est pas une suite binaire.\n", saisie);
                    valide = 0;
                    break;
                }
            }
        }
    }

    // Création du Grandentier selon le type
    Grandentier *g = NULL;
    if (type == 'd' || type == 'D') {
        g = ge_creer(saisie);
    } else {
        g = ge_creer_from_binary(saisie);
    }

    if (!g) {
        printf(" Il y'a eu une erreur lors de la creation du Grandentier\n");
        return NULL;
    }

    return g;
}

void afficher_grandentier_details(const Grandentier* g, const char* nom) {
    if (!g) return;
    printf("\n--- %s ---\n", nom);
    printf("Representation binaire : ");
    ge_afficher(g);
    printf("Taille : ");
    ge_afficher_taille(g);
    ge_afficher_signe(g);
}
#include "../include/grandentier.h"

int main() {
    printf("=== Test de la bibliothèque GrandEntier ===\n\n");

    // Test création et affichage
    printf("1. Test création:\n");
    Grandentier *a = ge_creer("15");
    Grandentier *b = ge_creer("4");

    printf("a = "); ge_afficher(a);
    printf("b = "); ge_afficher(b);
    printf("\n");

    // Test addition
    printf("2. Test addition:\n");
    Grandentier *somme = add_GrandEntier(a, b);
    printf("a + b = "); ge_afficher(somme);
    printf("\n");

    // Test soustraction
    printf("3. Test soustraction:\n");
    Grandentier *difference = sous_GrandEntier(a, b);
    printf("a - b = "); ge_afficher(difference);
    printf("\n");

    // Test division
    printf("4. Test division:\n");
    Grandentier *quotient = div_GrandEntier(a, b);
    printf("a / b = "); ge_afficher(quotient);
    printf("\n");

    // Test avec nombres négatifs
    printf("5. Test avec nombres négatifs:\n");
    Grandentier *c = ge_creer("-10");
    Grandentier *d = ge_creer("3");

    printf("c = "); ge_afficher(c);
    printf("d = "); ge_afficher(d);

    Grandentier *div_neg = div_GrandEntier(c, d);
    printf("c / d = "); ge_afficher(div_neg);
    printf("\n");

    // Test division par zéro
    printf("6. Test division par zéro:\n");
    Grandentier *zero = ge_creer("0");
    Grandentier *div_zero = div_GrandEntier(a, zero);
    if (!div_zero) {
        printf("Division par zéro correctement gérée!\n");
    }

    // Nettoyage de la mémoire
    ge_liberer(a);
    ge_liberer(b);
    ge_liberer(somme);
    ge_liberer(difference);
    ge_liberer(quotient);
    ge_liberer(c);
    ge_liberer(d);
    ge_liberer(div_neg);
    ge_liberer(zero);

    printf("\n=== Tous les tests sont terminés ===\n");

    return 0;

}