#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdlib.h>
#include <stdio.h>

#include "../header/Struct_File.h"
#include "../header/Reseau.h"
typedef struct {
    int u, v;
} Arete;
/* Numeros des sommets extremite */

typedef struct cellule_arete {
    Arete *a; /* pointeur sur l’arete */
    struct cellule_arete *suiv;
} Cellule_arete;

typedef struct {
    int num; /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete *L_voisin; /* Liste chainee des voisins */
} Sommet;

typedef struct {
    int e1, e2; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
    int nbsom; /* Nombre de sommets */
    Sommet **T_som; /* Tableau de pointeurs sur sommets */
    int gamma;
    int nbcommod; /* Nombre de commodites */
    Commod *T_commod; /* Tableau des commodites */
} Graphe;

Graphe* creerGraphe(Reseau* r);
void libererGraphe(Graphe* g);
S_file* plusPetitNombreAretes(Sommet* u, Sommet* v, Graphe* graphe);
int reorganiseReseau(Reseau* r);
#endif
