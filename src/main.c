#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/grandentier.h"

int main() {
    char saisie[2048];
    char type;
    int valide = 0;

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
        return 1;
    }

    // Affichage
    printf("\n Votre Grandentier est :\n");
    printf(" La representation binaire : ");
    ge_afficher(g);
    printf("Taille : ");
    ge_afficher_taille(g);
    ge_afficher_signe(g);

    // TEST DES NOUVELLES FONCTIONS
    printf("\n--- TESTS DES OPERATIONS ---\n");

    // Créer un deuxième nombre pour tester les opérations
    Grandentier* test_num = ge_creer("5");
    if (test_num) {
        printf("Nombre de test (5): ");
        ge_afficher(test_num);

        // Test addition
        Grandentier* somme = add_GrandEntier(g, test_num);
        if (somme) {
            printf("Addition avec 5: ");
            ge_afficher(somme);
            ge_liberer(somme);
        }

        // Test soustraction
        Grandentier* difference = sous_GrandEntier(g, test_num);
        if (difference) {
            printf("Soustraction avec 5: ");
            ge_afficher(difference);
            ge_liberer(difference);
        }

        ge_liberer(test_num);
    }

    ge_liberer(g);
    return 0;
}
