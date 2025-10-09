#include "../include/grandentier.h"

// Fonction qui créée un Grandentier depuis un décimal
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
        if (str[0] == '+') start = 1;
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

    int capacity = len * 4;
    int *bits = malloc(capacity * sizeof(int));
    int bitCount = 0;

    // Conversion décimal à binaire
    while (!(len == 1 && temp[0] == '0')) {
        int carry = 0;
        for (int i = 0; i < len; i++) {
            int current = carry * 10 + (temp[i] - '0');
            temp[i] = (current / 2) + '0';
            carry = current % 2;
        }
        bits[bitCount++] = carry;

        while (len > 1 && temp[0] == '0') {
            temp++;
            len--;
        }
    }

    g->Taille = bitCount;
    g->Tdigts = malloc(g->Taille * sizeof(int));
    for (int i = 0; i < g->Taille; i++) {
        g->Tdigts[i] = bits[g->Taille - 1 - i];
    }

    free(bits);
    free(strdup(str + start));
    return g;
}

// Fonctio qui créée un entier depuis une chaine binaire
Grandentier* ge_creer_from_binary(const char *str) {
    if (!str) return NULL;

    Grandentier *g = malloc(sizeof(Grandentier));
    if (!g) return NULL;

    int len = strlen(str);
    g->Tdigts = malloc(len * sizeof(int));
    if (!g->Tdigts) {
        free(g);
        return NULL;
    }

    int isZero = 1;
    for (int i = 0; i < len; i++) {
        if (str[i] == '0') g->Tdigts[i] = 0;
        else if (str[i] == '1') {
            g->Tdigts[i] = 1;
            isZero = 0;
        } else {
            // caractère invalide qui entraine la libération et une erreur
            free(g->Tdigts);
            free(g);
            return NULL;
        }
    }

    g->Taille = len;
    g->Signe = isZero ? 0 : 1;
    return g;
}

// Les fonctions qui affichent notre Grandentier
void ge_afficher(const Grandentier *g) {
    if (!g) return;
    if (g->Signe == 0) {
        printf("0\n");
        return;
    }
    if (g->Signe == -1) printf("-");
    for (int i = 0; i < g->Taille; i++) printf("%d", g->Tdigts[i]);
    printf("\n");
}

void ge_afficher_taille(const Grandentier *g) {
    if (!g) return;
    printf("%d bits\n", g->Taille);
}

void ge_afficher_signe(const Grandentier *g) {
    if (!g) return;
    printf("Signe: %d\n", g->Signe);
}

// Libération de la mémoire
void ge_liberer(Grandentier *g) {
    if (g) {
        free(g->Tdigts);
        free(g);
    }
}