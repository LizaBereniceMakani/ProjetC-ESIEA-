#include "../include/grandentier.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// ---- Création depuis binaire ----
Grandentier* ge_creer_from_binary(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    Grandentier* g = malloc(sizeof(Grandentier));
    g->Tdigts = malloc(len * sizeof(int));
    g->Taille = len;
    g->Signe = 0;

    int allZero = 1;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '0') g->Tdigts[i] = 0;
        else if (str[i] == '1') { g->Tdigts[i] = 1; allZero = 0; }
        else { free(g->Tdigts); free(g); return NULL; }
    }
    if (!allZero) g->Signe = 1;
    return g;
}

// ---- Création depuis décimal ----
Grandentier* ge_creer(const char *str) {
    if (!str) return NULL;
    int sign = 1;
    size_t start = 0;
    if (str[0] == '-') { sign = -1; start = 1; }
    else if (str[0] == '+') start = 1;

    if (strcmp(str + start, "0") == 0) {
        Grandentier* g = malloc(sizeof(Grandentier));
        g->Taille = 1;
        g->Tdigts = malloc(sizeof(int));
        g->Tdigts[0] = 0;
        g->Signe = 0;
        return g;
    }

    char* temp = strdup(str + start);
    int capacity = strlen(temp) * 4;
    int* bits = malloc(capacity * sizeof(int));
    int bitCount = 0;

    while (strlen(temp) > 0 && strcmp(temp, "0") != 0) {
        int carry = 0;
        size_t len = strlen(temp);
        for (size_t i = 0; i < len; i++) {
            int cur = carry * 10 + (temp[i] - '0');
            temp[i] = (cur / 2) + '0';
            carry = cur % 2;
        }
        bits[bitCount++] = carry;
        size_t newStart = 0;
        while (newStart < len && temp[newStart] == '0') newStart++;
        if (newStart > 0) memmove(temp, temp+newStart, len-newStart+1);
    }

    Grandentier* g = malloc(sizeof(Grandentier));
    g->Taille = bitCount;
    g->Tdigts = malloc(bitCount * sizeof(int));
    g->Signe = sign;
    for (int i = 0; i < bitCount; i++) g->Tdigts[i] = bits[bitCount - 1 - i];

    free(bits); free(temp);
    return g;
}

// ---- Création depuis base^expo (ex: "2^34") ----
Grandentier* ge_creer_puissance(const char *str) {
    int base=0, expo=0;
    if (sscanf(str, "%d^%d", &base, &expo) != 2) return NULL;
    if (base != 2 || expo < 0) return NULL;

    Grandentier* g = malloc(sizeof(Grandentier));
    g->Taille = expo + 1;
    g->Tdigts = calloc(g->Taille, sizeof(int));
    g->Tdigts[0] = 1;
    g->Signe = 1;
    return g;
}

// ---- Détection automatique ----
Grandentier* ge_creer_auto(const char *input) {
    if (strchr(input,'^')) return ge_creer_puissance(input);

    int isBinary = 1;
    for (size_t i=0;i<strlen(input);i++)
        if (input[i]!='0' && input[i]!='1') { isBinary=0; break; }

    if (isBinary) return ge_creer_from_binary(input);
    return ge_creer(input);
}

// ---- Affichage ----
void ge_afficher(const Grandentier *g) {
    if (!g) return;
    if (g->Signe==0) { printf("0\n"); return; }
    if (g->Signe==-1) printf("-");
    for (int i=0;i<g->Taille;i++) printf("%d",g->Tdigts[i]);
    printf("\n");
}

void ge_afficher_taille(const Grandentier *g){if(g) printf("%d bits\n",g->Taille);}
void ge_afficher_signe(const Grandentier *g){if(g) printf("Signe: %d\n",g->Signe);}

// ---- Libération ----
void ge_liberer(Grandentier *g){if(g){free(g->Tdigts); free(g);}}

// ---- Copier ----
static Grandentier* ge_copier(const Grandentier *g){
    if(!g) return NULL;
    Grandentier* c = malloc(sizeof(Grandentier));
    c->Taille = g->Taille;
    c->Signe = g->Signe;
    c->Tdigts = malloc(c->Taille*sizeof(int));
    for(int i=0;i<c->Taille;i++) c->Tdigts[i] = g->Tdigts[i];
    return c;
}

// ---- Addition ----
Grandentier* add_GrandEntier(const Grandentier *a,const Grandentier *b){
    if(!a) return ge_copier(b);
    if(!b) return ge_copier(a);
    int taille = (a->Taille>b->Taille?a->Taille:b->Taille)+1;
    int* resbits = calloc(taille,sizeof(int));
    for(int i=0;i<a->Taille;i++) resbits[taille - i -1] += a->Tdigts[a->Taille - i -1];
    for(int i=0;i<b->Taille;i++) resbits[taille - i -1] += b->Tdigts[b->Taille - i -1];
    for(int i=taille-1;i>0;i--){
        if(resbits[i]>=2){resbits[i]-=2; resbits[i-1]++;}
    }
    int start=0; while(start<taille-1 && resbits[start]==0) start++;
    Grandentier* res=malloc(sizeof(Grandentier));
    res->Taille = taille - start;
    res->Tdigts = malloc(res->Taille*sizeof(int));
    for(int i=0;i<res->Taille;i++) res->Tdigts[i]=resbits[start+i];
    res->Signe=1;
    free(resbits);
    return res;
}

// ---- Soustraction (binaire simple, a>=b) ----
Grandentier* sous_GrandEntier(const Grandentier *a,const Grandentier *b){
    // Complément à 2 simplifié
    Grandentier* res=ge_copier(a);
    int borrow=0;
    for(int i=0;i<res->Taille;i++){
        int bi=(i<b->Taille)?b->Tdigts[b->Taille-1-i]:0;
        int ai=res->Tdigts[res->Taille-1-i]-borrow;
        if(ai<bi){ai+=2; borrow=1;} else borrow=0;
        res->Tdigts[res->Taille-1-i]=ai-bi;
    }
    // Normaliser taille
    int start=0; while(start<res->Taille-1 && res->Tdigts[start]==0) start++;
    if(start>0){
        int newT=res->Taille-start;
        int* newBits=malloc(newT*sizeof(int));
        for(int i=0;i<newT;i++) newBits[i]=res->Tdigts[start+i];
        free(res->Tdigts); res->Tdigts=newBits; res->Taille=newT;
    }
    return res;
}

// ---- Multiplication ----
Grandentier* mul_GrandEntier(const Grandentier *a,const Grandentier *b){
    if(!a||!b) return NULL;
    Grandentier* res=ge_creer("0");
    for(int i=b->Taille-1;i>=0;i--){
        if(b->Tdigts[i]==1){
            Grandentier* temp=ge_copier(a);
            int shift=b->Taille-1-i;
            Grandentier* shifted=malloc(sizeof(Grandentier));
            shifted->Taille=temp->Taille+shift;
            shifted->Tdigts=calloc(shifted->Taille,sizeof(int));
            for(int j=0;j<temp->Taille;j++)
                shifted->Tdigts[j]=temp->Tdigts[j];
            for(int j=0;j<shift;j++) shifted->Tdigts[shifted->Taille-1-j]=0;
            shifted->Signe=1;
            Grandentier* sum=add_GrandEntier(res,shifted);
            ge_liberer(res); ge_liberer(temp); ge_liberer(shifted);
            res=sum;
        }
    }
    return res;
}

// ---- Division (binaire simplifiée, retourne NULL si complexe) ----
Grandentier* div_GrandEntier(const Grandentier *a,const Grandentier *b){
    if(!a||!b) return NULL;
    if(b->Taille==1 && b->Tdigts[0]==1) return ge_copier(a);
    printf("Division complexe non implémentée.\n");
    return NULL;
}

// ---- Comparaison ----
int cmp_GrandEntier(const Grandentier *a,const Grandentier *b){
    if(!a||!b) return 0;
    if(a->Taille>b->Taille) return 1;
    if(a->Taille<b->Taille) return -1;
    for(int i=0;i<a->Taille;i++){
        if(a->Tdigts[i]>b->Tdigts[i]) return 1;
        if(a->Tdigts[i]<b->Tdigts[i]) return -1;
    }
    return 0;
}
