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

// FONCTION D'AIDE - Créer une copie d'un Grandentier
static Grandentier* ge_copier(const Grandentier* src) {
    if (!src) return NULL;

    Grandentier* copie = malloc(sizeof(Grandentier));
    if (!copie) return NULL;

    copie->Tdigts = malloc(src->Taille * sizeof(int));
    if (!copie->Tdigts) {
        free(copie);
        return NULL;
    }

    memcpy(copie->Tdigts, src->Tdigts, src->Taille * sizeof(int));
    copie->Taille = src->Taille;
    copie->Signe = src->Signe;

    return copie;
}

// FONCTION D'AIDE - Étendre la taille d'un Grandentier
static Grandentier* ge_etendre(const Grandentier* ge, int nouvelle_taille) {
    if (!ge || nouvelle_taille <= ge->Taille) return NULL;

    Grandentier* etendu = malloc(sizeof(Grandentier));
    if (!etendu) return NULL;

    etendu->Tdigts = malloc(nouvelle_taille * sizeof(int));
    if (!etendu->Tdigts) {
        free(etendu);
        return NULL;
    }

    // Copier les bits existants
    for (int i = 0; i < ge->Taille; i++) {
        etendu->Tdigts[i] = ge->Tdigts[i];
    }

    // Remplir avec des 0 (pour l'extension des positifs)
    for (int i = ge->Taille; i < nouvelle_taille; i++) {
        etendu->Tdigts[i] = 0;
    }

    etendu->Taille = nouvelle_taille;
    etendu->Signe = ge->Signe;

    return etendu;
}

// FONCTION D'AIDE - Normaliser (supprimer les zéros non significatifs)
static Grandentier* ge_normaliser(const Grandentier* ge) {
    if (!ge) return NULL;

    // Trouver la position du premier '1' en partant de la gauche
    int premier_un = -1;
    for (int i = 0; i < ge->Taille; i++) {
        if (ge->Tdigts[i] == 1) {
            premier_un = i;
            break;
        }
    }

    // Si aucun '1' trouvé, c'est zéro
    if (premier_un == -1) {
        Grandentier* zero = malloc(sizeof(Grandentier));
        zero->Taille = 1;
        zero->Tdigts = malloc(sizeof(int));
        zero->Tdigts[0] = 0;
        zero->Signe = 0;
        return zero;
    }

    int nouvelle_taille = ge->Taille - premier_un;

    Grandentier* normalise = malloc(sizeof(Grandentier));
    if (!normalise) return NULL;

    normalise->Tdigts = malloc(nouvelle_taille * sizeof(int));
    if (!normalise->Tdigts) {
        free(normalise);
        return NULL;
    }

    // Copier les bits significatifs
    for (int i = 0; i < nouvelle_taille; i++) {
        normalise->Tdigts[i] = ge->Tdigts[premier_un + i];
    }

    normalise->Taille = nouvelle_taille;
    normalise->Signe = ge->Signe;

    return normalise;
}

// FONCTION D'ADDITION
Grandentier* add_GrandEntier(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;

    // Cas particuliers simples
    if (a->Signe == 0) return ge_copier(b);
    if (b->Signe == 0) return ge_copier(a);

    // Déterminer la taille maximale nécessaire
    int taille_max = (a->Taille > b->Taille) ? a->Taille : b->Taille;
    taille_max++; // Pour la retenue éventuelle

    // Étendre les deux nombres à la même taille
    Grandentier* a_etendu = ge_etendre(a, taille_max);
    Grandentier* b_etendu = ge_etendre(b, taille_max);

    if (!a_etendu || !b_etendu) {
        if (a_etendu) ge_liberer(a_etendu);
        if (b_etendu) ge_liberer(b_etendu);
        return NULL;
    }

    // Créer le résultat
    Grandentier* resultat = malloc(sizeof(Grandentier));
    if (!resultat) {
        ge_liberer(a_etendu);
        ge_liberer(b_etendu);
        return NULL;
    }

    resultat->Tdigts = malloc(taille_max * sizeof(int));
    if (!resultat->Tdigts) {
        ge_liberer(a_etendu);
        ge_liberer(b_etendu);
        free(resultat);
        return NULL;
    }

    resultat->Taille = taille_max;

    // Addition binaire avec retenue
    int retenue = 0;
    for (int i = 0; i < taille_max; i++) {
        int bit_a = (i < a_etendu->Taille) ? a_etendu->Tdigts[i] : 0;
        int bit_b = (i < b_etendu->Taille) ? b_etendu->Tdigts[i] : 0;

        int somme = bit_a + bit_b + retenue;
        resultat->Tdigts[i] = somme % 2;
        retenue = somme / 2;
    }

    // Déterminer le signe du résultat
    if (a->Signe == b->Signe) {
        resultat->Signe = a->Signe; // Même signe = résultat a ce signe
    } else {
        // Signes différents : le résultat prend le signe du plus grand en valeur absolue
        // Pour simplifier, on utilise le bit le plus significatif
        resultat->Signe = (resultat->Tdigts[taille_max - 1] == 0) ? 1 : -1;
    }

    // Normaliser le résultat
    Grandentier* resultat_final = ge_normaliser(resultat);

    // Nettoyer la mémoire temporaire
    ge_liberer(a_etendu);
    ge_liberer(b_etendu);
    ge_liberer(resultat);

    return resultat_final;
}

// FONCTION DE SOUSTRACTION
Grandentier* sous_GrandEntier(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;

    // Cas particuliers simples
    if (b->Signe == 0) return ge_copier(a); // a - 0 = a
    if (a->Signe == 0) {
        // 0 - b = -b
        Grandentier* moins_b = ge_copier(b);
        if (moins_b) {
            moins_b->Signe = -b->Signe;
        }
        return moins_b;
    }

    // a - b = a + (-b)
    Grandentier* moins_b = ge_copier(b);
    if (!moins_b) return NULL;

    moins_b->Signe = -b->Signe;

    Grandentier* resultat = add_GrandEntier(a, moins_b);

    ge_liberer(moins_b);

    return resultat;
}