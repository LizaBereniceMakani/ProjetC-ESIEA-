#include "../include/grandentier.h"

// Convertir une chaîne décimale en binaire
Grandentier* ge_creer(const char *str) {
    if (!str) return NULL;

    Grandentier *g = malloc(sizeof(Grandentier));
    if (!g) return NULL;

    // Gestion du signe
    int start = 0;
    if (str[0] == '-') {
        g->Signe = -1;
        start = 1;
    } else {
        g->Signe = 1;
    }

    // Cas du zéro
    if (strcmp(str + start, "0") == 0) {
        g->Signe = 0;
        g->Taille = 1;
        g->Tdigts = malloc(sizeof(int));
        g->Tdigts[0] = 0;
        return g;
    }

    // Copie de la partie numérique
    char *temp = strdup(str + start);
    int len = strlen(temp);

    // On va diviser le nombre décimal par 2 jusqu’à 0
    int capacity = len * 4;
    int *bits = malloc(capacity * sizeof(int));
    int bitCount = 0;

    while (!(len == 1 && temp[0] == '0')) {
        int carry = 0;
        for (int i = 0; i < len; i++) {
            int current = carry * 10 + (temp[i] - '0');
            temp[i] = (current / 2) + '0';
            carry = current % 2;
        }
        bits[bitCount++] = carry;

        // Supprimer les zéros de tête
        while (len > 1 && temp[0] == '0') {
            temp++;
            len--;
        }
    }

    // Inverser l’ordre pour avoir le bit de poids fort en premier
    g->Taille = bitCount;
    g->Tdigts = malloc(g->Taille * sizeof(int));
    for (int i = 0; i < g->Taille; i++) {
        g->Tdigts[i] = bits[g->Taille - 1 - i];
    }

    free(bits);
    free(strdup(str + start)); // libère la copie initiale

    return g;
}

// Afficher le GrandEntier en binaire
void ge_afficher(const Grandentier *g) {
    if (!g) return;

    if (g->Signe == 0) {
        printf("0\n");
        return;
    }

    if (g->Signe == -1) printf("-");

    for (int i = 0; i < g->Taille; i++) {
        printf("%d", g->Tdigts[i]);
    }
    printf("\n");
}

void ge_afficher_taille(const Grandentier *g) {
    printf("%d bits\n", g->Taille);
}

void ge_afficher_signe(const Grandentier *g) {
    printf("Signe: %d\n", g->Signe);
}

void ge_liberer(Grandentier *g) {
    if (g) {
        free(g->Tdigts);
        free(g);
    }
}
