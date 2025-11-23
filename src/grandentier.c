#include "../include/grandentier.h"
#include <string.h>
#include <ctype.h>

// ==================== CRÉATION ET DESTRUCTION ====================

GrandEntier* creerGrandEntier(int taille) {
    GrandEntier *ge = (GrandEntier*)malloc(sizeof(GrandEntier));
    ge->Taille = taille;
    ge->Signe = 0;
    ge->Tdigits = (int*)malloc(taille * sizeof(int));
    for (int i = 0; i < taille; i++) {
        ge->Tdigits[i] = 0;
    }
    return ge;
}

GrandEntier* creerGrandEntierDepuisChaine(const char *chaine_binaire) {
    if (!chaine_binaire) return NULL;

    int signe = 1;
    int start = 0;

    // Gérer le signe
    if (chaine_binaire[0] == '-') {
        signe = -1;
        start = 1;
    } else if (chaine_binaire[0] == '+') {
        start = 1;
    }

    // Compter les bits valides
    int n = strlen(chaine_binaire);
    int taille = 0;
    for (int i = start; i < n; i++) {
        if (chaine_binaire[i] == '0' || chaine_binaire[i] == '1') {
            taille++;
        }
    }

    if (taille == 0) return creerGrandEntierZero();

    GrandEntier *ge = creerGrandEntier(taille);
    ge->Signe = signe;

    // Remplir le tableau
    int index = 0;
    int tousZeros = 1;
    for (int i = start; i < n; i++) {
        if (chaine_binaire[i] == '0' || chaine_binaire[i] == '1') {
            ge->Tdigits[index] = chaine_binaire[i] - '0';
            if (ge->Tdigits[index] == 1) tousZeros = 0;
            index++;
        }
    }

    // Supprimer les zéros non significatifs à gauche
    int debut = 0;
    while (debut < ge->Taille - 1 && ge->Tdigits[debut] == 0) {
        debut++;
    }

    if (debut > 0) {
        int nouvelle_taille = ge->Taille - debut;
        int *nouveaux_digits = (int*)malloc(nouvelle_taille * sizeof(int));
        for (int i = 0; i < nouvelle_taille; i++) {
            nouveaux_digits[i] = ge->Tdigits[debut + i];
        }
        free(ge->Tdigits);
        ge->Tdigits = nouveaux_digits;
        ge->Taille = nouvelle_taille;
    }

    if (tousZeros) {
        ge->Signe = 0;
        if (ge->Taille > 1) {
            free(ge->Tdigits);
            ge->Tdigits = (int*)malloc(sizeof(int));
            ge->Tdigits[0] = 0;
            ge->Taille = 1;
        }
    }

    return ge;
}

GrandEntier* creerGrandEntierZero() {
    GrandEntier *ge = creerGrandEntier(1);
    ge->Tdigits[0] = 0;
    ge->Signe = 0;
    return ge;
}

GrandEntier* copierGrandEntier(const GrandEntier *ge) {
    if (!ge) return NULL;
    GrandEntier *copie = creerGrandEntier(ge->Taille);
    copie->Signe = ge->Signe;
    for (int i = 0; i < ge->Taille; i++) {
        copie->Tdigits[i] = ge->Tdigits[i];
    }
    return copie;
}

void libererGrandEntier(GrandEntier *ge) {
    if (!ge) return;
    if (ge->Tdigits) free(ge->Tdigits);
    free(ge);
}

// ==================== AFFICHAGE ====================

void afficherGrandEntier(const GrandEntier *ge) {
    if (!ge) {
        printf("NULL\n");
        return;
    }

    if (ge->Signe == -1) printf("-");
    if (ge->Signe == 0) {
        printf("0\n");
        return;
    }

    for (int i = 0; i < ge->Taille; i++) {
        printf("%d", ge->Tdigits[i]);
    }
    printf("\n");
}

void afficherGrandEntierDetail(const GrandEntier *ge) {
    if (!ge) {
        printf("GrandEntier: NULL\n");
        return;
    }

    printf("Signe: %d\n", ge->Signe);
    printf("Taille: %d bits\n", ge->Taille);
    printf("Valeur binaire: ");
    afficherGrandEntier(ge);
}

// ==================== COMPARAISON ====================

int comparerGrandEntier(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b) return 0;

    if (a->Signe < b->Signe) return -1;
    if (a->Signe > b->Signe) return 1;

    if (a->Taille != b->Taille) {
        if (a->Taille < b->Taille) return (a->Signe >= 0) ? -1 : 1;
        if (a->Taille > b->Taille) return (a->Signe >= 0) ? 1 : -1;
    }

    for (int i = 0; i < a->Taille; i++) {
        if (a->Tdigits[i] < b->Tdigits[i]) return (a->Signe >= 0) ? -1 : 1;
        if (a->Tdigits[i] > b->Tdigits[i]) return (a->Signe >= 0) ? 1 : -1;
    }

    return 0;
}

// ==================== ADDITION ====================

GrandEntier* additionGrandEntier(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b) return NULL;

    if (a->Signe == 0) return copierGrandEntier(b);
    if (b->Signe == 0) return copierGrandEntier(a);

    // Pour l'instant, on suppose que a et b sont positifs
    if (a->Signe == -1 || b->Signe == -1) {
        printf("ERREUR: Addition non implémentée pour les négatifs\n");
        return NULL;
    }

    int taille_max = (a->Taille > b->Taille) ? a->Taille : b->Taille;
    GrandEntier *resultat = creerGrandEntier(taille_max + 1);
    resultat->Signe = 1;

    int retenue = 0;
    int idx_a = a->Taille - 1;
    int idx_b = b->Taille - 1;
    int idx_res = resultat->Taille - 1;

    while (idx_res >= 0) {
        int bit_a = (idx_a >= 0) ? a->Tdigits[idx_a] : 0;
        int bit_b = (idx_b >= 0) ? b->Tdigits[idx_b] : 0;

        int somme = bit_a + bit_b + retenue;
        resultat->Tdigits[idx_res] = somme % 2;
        retenue = somme / 2;

        idx_a--;
        idx_b--;
        idx_res--;
    }

    if (resultat->Tdigits[0] == 0 && resultat->Taille > 1) {
        int nouvelle_taille = resultat->Taille - 1;
        int *nouveaux_digits = (int*)malloc(nouvelle_taille * sizeof(int));
        for (int i = 0; i < nouvelle_taille; i++) {
            nouveaux_digits[i] = resultat->Tdigits[i + 1];
        }
        free(resultat->Tdigits);
        resultat->Tdigits = nouveaux_digits;
        resultat->Taille = nouvelle_taille;
    }

    return resultat;
}

// ==================== SOUSTRACTION ====================

GrandEntier* soustractionGrandEntier(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b) return NULL;

    int comparaison = comparerGrandEntier(a, b);

    if (comparaison == 0) {
        return creerGrandEntierZero();
    }

    if (comparaison == -1) {
        GrandEntier *temp = soustractionGrandEntier(b, a);
        if (temp) temp->Signe = -1;
        return temp;
    }

    GrandEntier *resultat = creerGrandEntier(a->Taille);
    resultat->Signe = 1;

    int emprunt = 0;
    int idx_a = a->Taille - 1;
    int idx_b = b->Taille - 1;
    int idx_res = resultat->Taille - 1;

    while (idx_res >= 0) {
        int bit_a = (idx_a >= 0) ? a->Tdigits[idx_a] : 0;
        int bit_b = (idx_b >= 0) ? b->Tdigits[idx_b] : 0;

        int difference = bit_a - bit_b - emprunt;
        if (difference < 0) {
            difference += 2;
            emprunt = 1;
        } else {
            emprunt = 0;
        }

        resultat->Tdigits[idx_res] = difference;

        idx_a--;
        idx_b--;
        idx_res--;
    }

    int debut = 0;
    while (debut < resultat->Taille - 1 && resultat->Tdigits[debut] == 0) {
        debut++;
    }

    if (debut > 0) {
        int nouvelle_taille = resultat->Taille - debut;
        int *nouveaux_digits = (int*)malloc(nouvelle_taille * sizeof(int));
        for (int i = 0; i < nouvelle_taille; i++) {
            nouveaux_digits[i] = resultat->Tdigits[debut + i];
        }
        free(resultat->Tdigits);
        resultat->Tdigits = nouveaux_digits;
        resultat->Taille = nouvelle_taille;
    }

    return resultat;
}

// ==================== OPÉRATIONS UTILITAIRES ====================

void diviserParDeux(GrandEntier *ge) {
    if (!ge || ge->Signe == 0) return;

    if (ge->Taille == 1) {
        ge->Tdigits[0] = 0;
        ge->Signe = 0;
        return;
    }

    int nouvelle_taille = ge->Taille - 1;
    int *nouveaux_digits = (int*)malloc(nouvelle_taille * sizeof(int));
    for (int i = 0; i < nouvelle_taille; i++) {
        nouveaux_digits[i] = ge->Tdigits[i];
    }
    free(ge->Tdigits);
    ge->Tdigits = nouveaux_digits;
    ge->Taille = nouvelle_taille;
}

// ==================== FONCTIONS AVANCÉES ====================

GrandEntier* multiplicationEgyptienne(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b) return NULL;

    if (a->Signe == 0 || b->Signe == 0) {
        return creerGrandEntierZero();
    }

    GrandEntier *resultat = creerGrandEntierZero();
    GrandEntier *multiplicande = copierGrandEntier(a);
    GrandEntier *multiplicateur = copierGrandEntier(b);

    while (multiplicateur->Signe != 0) {
        if (multiplicateur->Tdigits[multiplicateur->Taille - 1] == 1) {
            GrandEntier *nouveau_resultat = additionGrandEntier(resultat, multiplicande);
            libererGrandEntier(resultat);
            resultat = nouveau_resultat;
        }

        GrandEntier *nouveau_multiplicande = creerGrandEntier(multiplicande->Taille + 1);
        nouveau_multiplicande->Signe = multiplicande->Signe;
        for (int i = 0; i < multiplicande->Taille; i++) {
            nouveau_multiplicande->Tdigits[i] = multiplicande->Tdigits[i];
        }
        nouveau_multiplicande->Tdigits[multiplicande->Taille] = 0;
        libererGrandEntier(multiplicande);
        multiplicande = nouveau_multiplicande;

        diviserParDeux(multiplicateur);
    }

    libererGrandEntier(multiplicande);
    libererGrandEntier(multiplicateur);

    return resultat;
}

GrandEntier* pgcdBinaire(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b) return NULL;

    GrandEntier *u = copierGrandEntier(a);
    GrandEntier *v = copierGrandEntier(b);

    if (u->Signe == 0) {
        libererGrandEntier(u);
        return v;
    }
    if (v->Signe == 0) {
        libererGrandEntier(v);
        return u;
    }

    int k = 0;
    GrandEntier *temp_u = copierGrandEntier(u);
    GrandEntier *temp_v = copierGrandEntier(v);

    while (temp_u->Tdigits[temp_u->Taille-1] == 0 && temp_v->Tdigits[temp_v->Taille-1] == 0) {
        diviserParDeux(temp_u);
        diviserParDeux(temp_v);
        k++;
    }

    GrandEntier *resultat = NULL;

    while (temp_v->Signe != 0) {
        while (temp_v->Tdigits[temp_v->Taille-1] == 0) {
            diviserParDeux(temp_v);
        }

        if (comparerGrandEntier(temp_u, temp_v) == -1) {
            GrandEntier *temp = temp_u;
            temp_u = temp_v;
            temp_v = temp;
        }

        GrandEntier *nouveau_u = soustractionGrandEntier(temp_u, temp_v);
        libererGrandEntier(temp_u);
        temp_u = nouveau_u;
    }

    resultat = copierGrandEntier(temp_u);
    for (int i = 0; i < k; i++) {
        GrandEntier *temp = creerGrandEntier(resultat->Taille + 1);
        temp->Signe = resultat->Signe;
        for (int j = 0; j < resultat->Taille; j++) {
            temp->Tdigits[j] = resultat->Tdigits[j];
        }
        temp->Tdigits[resultat->Taille] = 0;
        libererGrandEntier(resultat);
        resultat = temp;
    }

    libererGrandEntier(temp_u);
    libererGrandEntier(temp_v);
    libererGrandEntier(u);
    libererGrandEntier(v);

    return resultat;
}

GrandEntier* moduloGrandEntier(const GrandEntier *a, const GrandEntier *b) {
    if (!a || !b || b->Signe == 0) return NULL;

    if (comparerGrandEntier(a, b) == -1) {
        return copierGrandEntier(a);
    }

    if (comparerGrandEntier(a, b) == 0) {
        return creerGrandEntierZero();
    }

    GrandEntier *resultat = copierGrandEntier(a);
    GrandEntier *temp_b = copierGrandEntier(b);

    while (comparerGrandEntier(resultat, temp_b) != -1) {
        GrandEntier *nouveau_temp_b = creerGrandEntier(temp_b->Taille + 1);
        nouveau_temp_b->Signe = temp_b->Signe;
        for (int i = 0; i < temp_b->Taille; i++) {
            nouveau_temp_b->Tdigits[i] = temp_b->Tdigits[i];
        }
        nouveau_temp_b->Tdigits[temp_b->Taille] = 0;
        libererGrandEntier(temp_b);
        temp_b = nouveau_temp_b;
    }

    diviserParDeux(temp_b);

    while (comparerGrandEntier(resultat, b) != -1) {
        if (comparerGrandEntier(temp_b, resultat) == 1) {
            diviserParDeux(temp_b);
            continue;
        }

        GrandEntier *nouveau_resultat = soustractionGrandEntier(resultat, temp_b);
        libererGrandEntier(resultat);
        resultat = nouveau_resultat;
    }

    libererGrandEntier(temp_b);
    return resultat;
}

GrandEntier* exponentiationModulaire(const GrandEntier *base, unsigned int exposant, const GrandEntier *mod) {
    if (!base || !mod || mod->Signe == 0) return NULL;

    if (exposant == 0) {
        GrandEntier *un = creerGrandEntierDepuisChaine("1");
        GrandEntier *resultat = moduloGrandEntier(un, mod);
        libererGrandEntier(un);
        return resultat;
    }

    GrandEntier *resultat = creerGrandEntierDepuisChaine("1");
    GrandEntier *base_temp = moduloGrandEntier(base, mod);

    while (exposant > 0) {
        if (exposant % 2 == 1) {
            GrandEntier *temp = multiplicationEgyptienne(resultat, base_temp);
            GrandEntier *nouveau_resultat = moduloGrandEntier(temp, mod);
            libererGrandEntier(resultat);
            libererGrandEntier(temp);
            resultat = nouveau_resultat;
        }

        GrandEntier *carre = multiplicationEgyptienne(base_temp, base_temp);
        GrandEntier *nouvelle_base = moduloGrandEntier(carre, mod);
        libererGrandEntier(base_temp);
        libererGrandEntier(carre);
        base_temp = nouvelle_base;

        exposant /= 2;
    }

    libererGrandEntier(base_temp);
    return resultat;
}

// ==================== BONUS RSA ====================

GrandEntier* chiffrementRSA(const GrandEntier *message, unsigned int e, const GrandEntier *n) {
    if (!message || !n) return NULL;

    if (comparerGrandEntier(message, n) != -1) {
        printf("ERREUR: Message trop grand pour le module RSA\n");
        return NULL;
    }

    return exponentiationModulaire(message, e, n);
}

GrandEntier* dechiffrementRSA(const GrandEntier *chiffre, unsigned int d, const GrandEntier *n) {
    if (!chiffre || !n) return NULL;

    return exponentiationModulaire(chiffre, d, n);
}