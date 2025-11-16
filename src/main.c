#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
        printf("Comment voulez-vous creer le nombre ?\n");
        printf("b = binaire, d = decimal, p = base^expo, a = auto-detection\n");
        printf("Votre choix : ");
        if (scanf(" %c", &choix) != 1) continue;
        while (getchar() != '\n'); // vider le buffer
        if (choix=='b'||choix=='B'||choix=='d'||choix=='D'||choix=='p'||choix=='P'||choix=='a'||choix=='A') break;
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
        case 'a': case 'A': return ge_creer_auto(input);
        default: return NULL;
    }
}

// Fonction pour afficher un Grandentier en hexadécimal
void afficher_hexadecimal(Grandentier* g) {
    if (!g) return;
    char* hex = ge_vers_hexadecimal(g);
    printf("Hexadecimal: %s\n", hex);
    free(hex);
}

// Menu pour les opérations de phase 3
void menu_phase3() {
    int choix;
    do {
        printf("\n=== OPERATIONS AVANCEES (Phase 3) ===\n");
        printf("1.  Exponentiation modulaire (a^b mod m)\n");
        printf("2.  PGCD de deux nombres\n");
        printf("3.  Inverse modulaire\n");
        printf("4.  Test de primalite (Miller-Rabin)\n");
        printf("5.  Generer un nombre premier\n");
        printf("6.  Algorithme d'Euclide etendu\n");
        printf("7.  Conversion hexadecimal\n");
        printf("8.  ET bit-a-bit\n");
        printf("9.  OU bit-a-bit\n");
        printf("10. Retour au menu principal\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        switch (choix) {
            case 1: { // Exponentiation modulaire
                printf("\n--- Exponentiation modulaire ---\n");
                Grandentier* base = saisir_grandentier("Base (a) : ");
                Grandentier* exp = saisir_grandentier("Exposant (b) : ");
                Grandentier* mod = saisir_grandentier("Modulo (m) : ");

                Grandentier* res = ge_pow_mod(base, exp, mod);
                if (res) {
                    printf("\nResultat de %s^%s mod %s :",
                           ge_vers_hexadecimal(base),
                           ge_vers_hexadecimal(exp),
                           ge_vers_hexadecimal(mod));
                    afficher_grandentier(res);
                    afficher_hexadecimal(res);
                } else {
                    printf("Erreur lors du calcul.\n");
                }

                ge_liberer(base);
                ge_liberer(exp);
                ge_liberer(mod);
                ge_liberer(res);
                break;
            }

            case 2: { // PGCD
                printf("\n--- Calcul du PGCD ---\n");
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");

                Grandentier* pgcd = ge_gcd(a, b);
                if (pgcd) {
                    printf("\nPGCD :");
                    afficher_grandentier(pgcd);
                    afficher_hexadecimal(pgcd);
                }

                ge_liberer(a);
                ge_liberer(b);
                ge_liberer(pgcd);
                break;
            }

            case 3: { // Inverse modulaire
                printf("\n--- Inverse modulaire ---\n");
                Grandentier* a = saisir_grandentier("Nombre a : ");
                Grandentier* m = saisir_grandentier("Modulo m : ");

                Grandentier* inverse = ge_mod_inverse(a, m);
                if (inverse) {
                    printf("\nInverse modulaire de %s mod %s :",
                           ge_vers_hexadecimal(a),
                           ge_vers_hexadecimal(m));
                    afficher_grandentier(inverse);
                    afficher_hexadecimal(inverse);

                    // Vérification
                    Grandentier* verif = mul_GrandEntier(a, inverse);
                    Grandentier* verif_mod = Grandentier_mod(verif, m);
                    printf("Verification (a * inverse mod m) :");
                    afficher_grandentier(verif_mod);

                    ge_liberer(verif);
                    ge_liberer(verif_mod);
                } else {
                    printf("Pas d'inverse modulaire (a et m ne sont pas premiers entre eux).\n");
                }

                ge_liberer(a);
                ge_liberer(m);
                ge_liberer(inverse);
                break;
            }

            case 4: { // Test de primalité
                printf("\n--- Test de primalite (Miller-Rabin) ---\n");
                Grandentier* n = saisir_grandentier("Nombre a tester : ");
                int iterations;
                printf("Nombre d'iterations pour le test (recommande: 5-10) : ");
                scanf("%d", &iterations);
                while(getchar() != '\n');

                int est_premier = ge_est_probablement_premier(n, iterations);
                printf("\nLe nombre ");
                ge_afficher(n);
                printf(" est %s.\n", est_premier ? "PROBABLEMENT PREMIER" : "COMPOSE");

                ge_liberer(n);
                break;
            }

            case 5: { // Génération nombre premier
                printf("\n--- Generation d'un nombre premier ---\n");
                int nb_bits;
                printf("Nombre de bits du nombre premier : ");
                scanf("%d", &nb_bits);
                while(getchar() != '\n');

                if (nb_bits < 2) {
                    printf("Nombre de bits trop petit.\n");
                    break;
                }

                printf("Generation en cours...\n");
                Grandentier* premier = ge_generer_premier(nb_bits);
                if (premier) {
                    printf("\nNombre premier genere :");
                    afficher_grandentier(premier);
                    afficher_hexadecimal(premier);

                    // Vérification
                    int est_premier = ge_est_probablement_premier(premier, 5);
                    printf("Verification primalite : %s\n", est_premier ? "OK" : "ECHEC");
                } else {
                    printf("Erreur lors de la generation.\n");
                }

                ge_liberer(premier);
                break;
            }

            case 6: { // Euclide étendu
                printf("\n--- Algorithme d'Euclide etendu ---\n");
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");

                EuclideResult* result = ge_euclide_etendu(a, b);
                if (result) {
                    printf("\nResultat Euclide etendu :\n");
                    printf("PGCD : "); ge_afficher(result->gcd);
                    printf("Coefficient x : "); ge_afficher(result->x);
                    printf("Coefficient y : "); ge_afficher(result->y);

                    // Vérification : a*x + b*y = pgcd
                    Grandentier* ax = mul_GrandEntier(a, result->x);
                    Grandentier* by = mul_GrandEntier(b, result->y);
                    Grandentier* somme = add_GrandEntier(ax, by);

                    printf("Verification (a*x + b*y) : "); ge_afficher(somme);
                    printf("Devrait etre egal au PGCD : "); ge_afficher(result->gcd);

                    ge_liberer(ax);
                    ge_liberer(by);
                    ge_liberer(somme);

                    ge_liberer(result->gcd);
                    ge_liberer(result->x);
                    ge_liberer(result->y);
                    free(result);
                }

                ge_liberer(a);
                ge_liberer(b);
                break;
            }

            case 7: { // Conversion hexadécimale
                printf("\n--- Conversion hexadecimal ---\n");
                Grandentier* g = saisir_grandentier("Nombre a convertir : ");
                afficher_hexadecimal(g);
                ge_liberer(g);
                break;
            }

            case 8: { // ET bit-à-bit
                printf("\n--- ET bit-a-bit ---\n");
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");

                Grandentier* res = ge_et_bitwise(a, b);
                if (res) {
                    printf("\nResultat ET bit-a-bit :");
                    afficher_grandentier(res);
                    afficher_hexadecimal(res);
                }

                ge_liberer(a);
                ge_liberer(b);
                ge_liberer(res);
                break;
            }

            case 9: { // OU bit-à-bit
                printf("\n--- OU bit-a-bit ---\n");
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");

                Grandentier* res = ge_ou_bitwise(a, b);
                if (res) {
                    printf("\nResultat OU bit-a-bit :");
                    afficher_grandentier(res);
                    afficher_hexadecimal(res);
                }

                ge_liberer(a);
                ge_liberer(b);
                ge_liberer(res);
                break;
            }

            case 10:
                printf("Retour au menu principal...\n");
                break;

            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 10);
}

int main() {
    srand(time(NULL)); // Pour la génération de nombres premiers
    int choix;

    do {
        printf("\n=== MENU PRINCIPAL - Grands Entiers ===\n");
        printf("1.  Creer et afficher un Grandentier\n");
        printf("2.  Additionner deux Grandentiers\n");
        printf("3.  Soustraire deux Grandentiers\n");
        printf("4.  Comparer deux Grandentiers\n");
        printf("5.  Multiplier deux Grandentiers\n");
        printf("6.  Diviser deux Grandentiers\n");
        printf("7.  Modulo de deux Grandentiers\n");
        printf("8.  OPERATIONS AVANCEES (Phase 3)\n");
        printf("9.  Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');

        switch (choix) {
            case 1: {
                Grandentier* g = saisir_grandentier_menu1();
                if (!g) {
                    printf("Erreur de creation.\n");
                    break;
                }
                afficher_grandentier(g);
                afficher_hexadecimal(g); // Nouveau : affichage hexa
                ge_liberer(g);
                break;
            }
            case 2: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = add_GrandEntier(a, b);
                printf("\nResultat de l'addition :");
                afficher_grandentier(res);
                afficher_hexadecimal(res); // Nouveau
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 3: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = sous_GrandEntier(a, b);
                printf("\nResultat de la soustraction :");
                afficher_grandentier(res);
                afficher_hexadecimal(res); // Nouveau
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 4: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                int cmp = cmp_GrandEntier(a, b);
                if (cmp == 0) printf("\nLes nombres sont egaux.\n");
                else if (cmp > 0) printf("\nLe premier nombre est plus grand.\n");
                else printf("\nLe deuxieme nombre est plus grand.\n");
                ge_liberer(a); ge_liberer(b);
                break;
            }
            case 5: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = mul_GrandEntier(a, b);
                printf("\nResultat de la multiplication :");
                afficher_grandentier(res);
                afficher_hexadecimal(res); // Nouveau
                ge_liberer(a); ge_liberer(b); ge_liberer(res);
                break;
            }
            case 6: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = div_GrandEntier(a, b);
                if (res) {
                    printf("\nResultat de la division :");
                    afficher_grandentier(res);
                    afficher_hexadecimal(res); // Nouveau
                    ge_liberer(res);
                } else {
                    printf("Division non supportee.\n");
                }
                ge_liberer(a); ge_liberer(b);
                break;
            }
            case 7: {
                Grandentier* a = saisir_grandentier("Premier nombre : ");
                Grandentier* b = saisir_grandentier("Deuxieme nombre : ");
                Grandentier* res = Grandentier_mod(a, b);
                if (res) {
                    printf("\nResultat du modulo :");
                    afficher_grandentier(res);
                    afficher_hexadecimal(res); // Nouveau
                    ge_liberer(res);
                } else {
                    printf("Erreur : division par zero ou operation non supportee.\n");
                }
                ge_liberer(a); ge_liberer(b);
                break;
            }
            case 8: {
                menu_phase3();
                break;
            }
            case 9:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 9);

    return 0;
}

