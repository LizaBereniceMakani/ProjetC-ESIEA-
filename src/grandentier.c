// ---- Exponentiation modulaire (critique pour RSA) ----
Grandentier* ge_pow_mod(const Grandentier *base, const Grandentier *exp, const Grandentier *mod) {
    if (!base || !exp || !mod) return NULL;
    
    // Cas particuliers
    if (exp->Taille == 1 && exp->Tdigts[0] == 0) {
        return ge_creer("1"); // base^0 = 1
    }
    if (cmp_GrandEntier(mod, ge_creer("1")) == 0) {
        return ge_creer("0"); // x mod 1 = 0
    }
    
    Grandentier *result = ge_creer("1");
    Grandentier *base_mod = Grandentier_mod(base, mod);
    Grandentier *exponent = ge_copier(exp);
    
    // Exponentiation binaire (méthode square-and-multiply)
    while (cmp_GrandEntier(exponent, ge_creer("0")) > 0) {
        // Si l'exposant est impair
        if (exponent->Tdigts[exponent->Taille - 1] == 1) {
            Grandentier *temp = mul_GrandEntier(result, base_mod);
            Grandentier *new_result = Grandentier_mod(temp, mod);
            ge_liberer(result);
            ge_liberer(temp);
            result = new_result;
        }
        
        // Carré de la base
        Grandentier *temp_base = mul_GrandEntier(base_mod, base_mod);
        Grandentier *new_base = Grandentier_mod(temp_base, mod);
        ge_liberer(base_mod);
        ge_liberer(temp_base);
        base_mod = new_base;
        
        // Division de l'exposant par 2 (décalage à droite)
        Grandentier *new_exp = ge_creer("0");
        if (exponent->Taille > 1 || exponent->Tdigts[0] > 1) {
            // Implémentation du décalage à droite
            int new_size = exponent->Taille;
            if (exponent->Tdigts[exponent->Taille - 1] == 1 && exponent->Taille > 1) {
                new_size--;
            }
            
            new_exp->Taille = new_size;
            new_exp->Tdigts = malloc(new_size * sizeof(int));
            for (int i = 0; i < new_size; i++) {
                new_exp->Tdigts[i] = exponent->Tdigts[i];
            }
        }
        ge_liberer(exponent);
        exponent = new_exp;
    }
    
    ge_liberer(base_mod);
    ge_liberer(exponent);
    return result;
}

// ---- Algorithme d'Euclide étendu (PGCD + coefficients) ----
typedef struct {
    Grandentier *gcd;
    Grandentier *x;
    Grandentier *y;
} EuclideResult;

EuclideResult* ge_euclide_etendu(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;
    
    EuclideResult *result = malloc(sizeof(EuclideResult));
    
    if (cmp_GrandEntier(b, ge_creer("0")) == 0) {
        result->gcd = ge_copier(a);
        result->x = ge_creer("1");
        result->y = ge_creer("0");
        return result;
    }
    
    Grandentier *a_mod_b = Grandentier_mod(a, b);
    EuclideResult *temp = ge_euclide_etendu(b, a_mod_b);
    
    // x = ancien y
    result->x = temp->y;
    
    // y = ancien x - (a/b) * ancien y
    Grandentier *a_div_b = div_GrandEntier(a, b);
    Grandentier *temp_prod = mul_GrandEntier(a_div_b, temp->y);
    result->y = sous_GrandEntier(temp->x, temp_prod);
    
    result->gcd = temp->gcd;
    
    ge_liberer(a_mod_b);
    ge_liberer(a_div_b);
    ge_liberer(temp_prod);
    free(temp);
    
    return result;
}

// ---- PGCD simple ----
Grandentier* ge_gcd(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;
    
    Grandentier *x = ge_copier(a);
    Grandentier *y = ge_copier(b);
    
    while (cmp_GrandEntier(y, ge_creer("0")) != 0) {
        Grandentier *temp = y;
        y = Grandentier_mod(x, y);
        ge_liberer(x);
        x = temp;
    }
    
    ge_liberer(y);
    return x;
}

// ---- Inverse modulaire ----
Grandentier* ge_mod_inverse(const Grandentier *a, const Grandentier *m) {
    if (!a || !m) return NULL;
    
    EuclideResult *euclide = ge_euclide_etendu(a, m);
    
    // L'inverse n'existe que si PGCD(a, m) = 1
    if (cmp_GrandEntier(euclide->gcd, ge_creer("1")) != 0) {
        ge_liberer(euclide->gcd);
        ge_liberer(euclide->x);
        ge_liberer(euclide->y);
        free(euclide);
        return NULL; // Pas d'inverse modulaire
    }
    
    // x mod m est l'inverse modulaire
    Grandentier *inverse = Grandentier_mod(euclide->x, m);
    
    // Si négatif, ajouter m
    if (inverse->Signe == -1) {
        Grandentier *temp = add_GrandEntier(inverse, m);
        ge_liberer(inverse);
        inverse = temp;
    }
    
    ge_liberer(euclide->gcd);
    ge_liberer(euclide->x);
    ge_liberer(euclide->y);
    free(euclide);
    
    return inverse;
}

// ---- Test de primalité (Miller-Rabin simplifié) ----
int ge_est_probablement_premier(const Grandentier *n, int iterations) {
    if (!n) return 0;
    
    // Petits nombres premiers
    if (cmp_GrandEntier(n, ge_creer("2")) == 0) return 1;
    if (cmp_GrandEntier(n, ge_creer("1")) <= 0) return 0;
    
    // Nombres pairs
    if (n->Tdigts[n->Taille - 1] % 2 == 0) return 0;
    
    // Écrire n-1 = 2^s * d
    Grandentier *n_minus_1 = sous_GrandEntier(n, ge_creer("1"));
    int s = 0;
    Grandentier *d = ge_copier(n_minus_1);
    
    while (d->Tdigts[d->Taille - 1] % 2 == 0) {
        // Division par 2 (décalage à droite)
        d->Tdigts[d->Taille - 1] = 0;
        // Réorganiser les bits...
        s++;
    }
    
    // Test Miller-Rabin
    for (int i = 0; i < iterations; i++) {
        Grandentier *a = ge_creer("2"); // Base fixe pour simplification
        Grandentier *x = ge_pow_mod(a, d, n);
        
        if (cmp_GrandEntier(x, ge_creer("1")) == 0 || 
            cmp_GrandEntier(x, n_minus_1) == 0) {
            ge_liberer(a);
            ge_liberer(x);
            continue;
        }
        
        int continue_test = 0;
        for (int j = 0; j < s - 1; j++) {
            x = mul_GrandEntier(x, x);
            Grandentier *x_mod = Grandentier_mod(x, n);
            ge_liberer(x);
            x = x_mod;
            
            if (cmp_GrandEntier(x, n_minus_1) == 0) {
                continue_test = 1;
                break;
            }
        }
        
        ge_liberer(a);
        ge_liberer(x);
        
        if (!continue_test) {
            ge_liberer(n_minus_1);
            ge_liberer(d);
            return 0; // Composé
        }
    }
    
    ge_liberer(n_minus_1);
    ge_liberer(d);
    return 1; // Probablement premier
}

// ---- Génération de nombres premiers ----
Grandentier* ge_generer_premier(int nb_bits) {
    if (nb_bits < 2) return NULL;
    
    // Générer un nombre aléatoire de nb_bits
    Grandentier *candidat = malloc(sizeof(Grandentier));
    candidat->Taille = nb_bits;
    candidat->Tdigts = malloc(nb_bits * sizeof(int));
    candidat->Signe = 1;
    
    // Premier bit = 1, dernier bit = 1 (impair)
    candidat->Tdigts[0] = 1;
    candidat->Tdigts[nb_bits - 1] = 1;
    
    // Bits intermédiaires aléatoires (simplifié)
    for (int i = 1; i < nb_bits - 1; i++) {
        candidat->Tdigts[i] = rand() % 2;
    }
    
    // Trouver le premier nombre premier
    while (!ge_est_probablement_premier(candidat, 5)) {
        // Incrémenter de 2 (rester impair)
        Grandentier *two = ge_creer("10"); // 2 en binaire
        Grandentier *temp = add_GrandEntier(candidat, two);
        ge_liberer(candidat);
        candidat = temp;
        ge_liberer(two);
    }
    
    return candidat;
}

// ---- Conversion vers différentes bases ----
char* ge_vers_hexadecimal(const Grandentier *g) {
    if (!g) return NULL;
    
    // Conversion via binaire vers hexadécimal
    int hex_len = (g->Taille + 3) / 4;
    char *hex = malloc(hex_len + 1);
    hex[hex_len] = '\0';
    
    for (int i = 0; i < hex_len; i++) {
        int valeur = 0;
        for (int j = 0; j < 4; j++) {
            int pos = g->Taille - 1 - (i * 4 + j);
            if (pos >= 0) {
                valeur |= (g->Tdigts[pos] << j);
            }
        }
        hex[hex_len - 1 - i] = (valeur < 10) ? ('0' + valeur) : ('A' + valeur - 10);
    }
    
    return hex;
}

// ---- Opérations bit-à-bit ----
Grandentier* ge_et_bitwise(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;
    
    int taille = (a->Taille < b->Taille) ? a->Taille : b->Taille;
    Grandentier *result = malloc(sizeof(Grandentier));
    result->Taille = taille;
    result->Tdigts = malloc(taille * sizeof(int));
    result->Signe = 1;
    
    for (int i = 0; i < taille; i++) {
        int pos_a = a->Taille - taille + i;
        int pos_b = b->Taille - taille + i;
        result->Tdigts[i] = a->Tdigts[pos_a] & b->Tdigts[pos_b];
    }
    
    return result;
}

Grandentier* ge_ou_bitwise(const Grandentier *a, const Grandentier *b) {
    if (!a || !b) return NULL;
    
    int taille = (a->Taille > b->Taille) ? a->Taille : b->Taille;
    Grandentier *result = malloc(sizeof(Grandentier));
    result->Taille = taille;
    result->Tdigts = malloc(taille * sizeof(int));
    result->Signe = 1;
    
    for (int i = 0; i < taille; i++) {
        int bit_a = (i >= taille - a->Taille) ? a->Tdigts[i - (taille - a->Taille)] : 0;
        int bit_b = (i >= taille - b->Taille) ? b->Tdigts[i - (taille - b->Taille)] : 0;
        result->Tdigts[i] = bit_a | bit_b;
    }
    
    return result;
}