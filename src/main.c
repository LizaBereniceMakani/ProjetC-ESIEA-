#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/grandentier.h"

// Fonction pour saisir un Grandentier (détection automatique)
Grandentier* saisir_grandentier(const char* prompt) {
    char input[256];
    while (1) {
        printf("%s", prompt);
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = 0; // enlever le \n
        Grandentier* g = ge_creer_auto(input);
        if (!g) {
            printf("Saisie invalide. Essayez de nouveau.\n");
            continue;
        }
        return g;
    }
}

// Afficher un Grandentier
void afficher_grandentier(Grandentier* g) {
    if (!g) return;
    printf("\nNombre : ");
    ge_afficher(g);
    ge_afficher_taille(g);
    ge_afficher_signe(g);
}

// Fonction pour demander création spécifique (cas menu 1)
Grandentier* saisir_grandentier_menu1() {
    char choix;
    while (1) {
        printf("Comment voulez-vous creer le nombre ? b = binaire, d = decimal, p = base^expo\nVotre choix : ");
        if (scanf(" %c", &choix) != 1) continue;
        while (getchar() != '\n'); // vider le buffer
        if (choix=='b'||choix=='B'||choix=='d'||choix=='D'||choix=='p'||choix=='P') break;
        printf("Saisie invalide.\n");
    }

    char input[256];
    printf("Entrez la valeur : ");
    if (!fgets(input, sizeof(input), stdin)) return NULL;
    input[strcspn(input, "\n")] = 0;

    switch (choix) {
        case 'b': case 'B': return ge_creer_from_binary(input);
        case 'd': case 'D': return ge_creer(input);
        case 'p': case 'P': return ge_creer_puissance(input);
        default: return NULL;
    }
}

int main() {
    int choix;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Creer et afficher un Grandentier\n");
        printf("2. Additionner deux Grandentiers\n");
        printf("3. Soustraire deux Grandentiers\n");
        printf("4. Comparer deux Grandentiers\n");
        printf("5. Multiplier deux Grandentiers\n");
        printf("6. Diviser deux Grandentiers\n");
        printf("7. Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix)!=1) { while(getchar()!='\n'); continue; }
        while(getchar()!='\n');

        switch (choix) {
            case 1: {
                Grandentier* g = saisir_grandentier_menu1();
                if (!g) { printf("Erreur de création.\n"); break; }
                afficher_grandentier(g);
                ge_liberer(g);
                break;
            }
            case 2: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = add_GrandEntier(a,b);
                printf("\nResultat de l'addition :");
                afficher_grandentier(res);
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 3: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = sous_GrandEntier(a,b);
                printf("\nResultat de la soustraction :");
                afficher_grandentier(res);
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 4: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                int cmp = cmp_GrandEntier(a,b);
                if(cmp==0) printf("\nLes nombres sont egaux.\n");
                else if(cmp>0) printf("\nLe premier nombre est plus grand.\n");
                else printf("\nLe deuxieme nombre est plus grand.\n");
                ge_liberer(a); ge_liberer(b);
                break;
            }
            case 5: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = mul_GrandEntier(a,b);
                printf("\nResultat de la multiplication :");
                afficher_grandentier(res);
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 6: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = div_GrandEntier(a,b);
                if(res) {
                    printf("\nResultat de la division :");
                    afficher_grandentier(res);
                    ge_liberer(res);
                } else {
                    printf("Division non supportee.\n");
                }
                ge_liberer(a); ge_liberer(b);
                break;
            }
            case 7: printf("Au revoir !\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while(choix!=7);

    return 0;
}
