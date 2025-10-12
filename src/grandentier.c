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


// FONCTION D'AIDE - Comparaison de valeurs absolues (static pour usage interne)
static int ge_compare_abs(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return 0;

    // Si un nombre est zéro
    if (a->Signe == 0 && b->Signe == 0) return 0;
    if (a->Signe == 0) return -1;
    if (b->Signe == 0) return 1;

    // Comparaison par taille
    if (a->Taille > b->Taille) return 1;
    if (a->Taille < b->Taille) return -1;

    // Même taille, comparer bit par bit (du plus significatif au moins significatif)
    for (int i = 0; i < a->Taille; i++) {
        if (a->Tdigts[i] > b->Tdigts[i]) return 1;
        if (a->Tdigts[i] < b->Tdigts[i]) return -1;
    }

    return 0; // Égaux
}

// FONCTION DE DIVISION
Grandentier* div_GrandEntier(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;

    // Division par zéro
    if (b->Signe == 0) {
        printf("Erreur: Division par zéro!\n");
        return NULL;
    }

    // Dividende = 0
    if (a->Signe == 0) {
        return ge_creer("0");
    }

    // Travailler avec des copies pour ne pas modifier les originaux
    Grandentier *dividende = ge_copier(a);
    Grandentier *diviseur = ge_copier(b);

    if (!dividende || !diviseur) {
        if (dividende) ge_liberer(dividende);
        if (diviseur) ge_liberer(diviseur);
        return NULL;
    }

    // Travailler en valeur absolue
    dividende->Signe = 1;
    diviseur->Signe = 1;

    // Si diviseur > dividende, résultat = 0
    if (ge_compare_abs(diviseur, dividende) > 0) {
        ge_liberer(dividende);
        ge_liberer(diviseur);
        return ge_creer("0");
    }

    // Initialiser le quotient à 0
    Grandentier *quotient = ge_creer("0");
    Grandentier *un = ge_creer("1");

    if (!quotient || !un) {
        if (quotient) ge_liberer(quotient);
        if (un) ge_liberer(un);
        ge_liberer(dividende);
        ge_liberer(diviseur);
        return NULL;
    }

    // Algorithme de division par soustractions successives
    Grandentier *reste = ge_copier(dividende);

    while (reste && ge_compare_abs(reste, diviseur) >= 0) {
        // Soustraire le diviseur du reste
        Grandentier *nouveau_reste = sous_GrandEntier(reste, diviseur);
        if (!nouveau_reste) break;

        // Incrémenter le quotient
        Grandentier *nouveau_quotient = add_GrandEntier(quotient, un);
        if (!nouveau_quotient) {
            ge_liberer(nouveau_reste);
            break;
        }

        ge_liberer(reste);
        ge_liberer(quotient);

        reste = nouveau_reste;
        quotient = nouveau_quotient;
    }

    // Déterminer le signe du résultat
    if (quotient) {
        if (a->Signe != b->Signe) {
            quotient->Signe = -1;
        } else {
            quotient->Signe = 1;
        }
    }

    // Nettoyer la mémoire
    ge_liberer(dividende);
    ge_liberer(diviseur);
    ge_liberer(un);
    if (reste) ge_liberer(reste);

    return quotient;
}

// [Le reste de votre code existant reste inchangé...]


// Les autres fonctions existantes...