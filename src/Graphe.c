#include "../header/Graphe.h"
#include "../header/Reseau.h"
#include "../header/Struct_File.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//Fonction permettant la création d'un graphe à partir d'un Réseau r
Graphe* creerGraphe(Reseau* r) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    if (graphe == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le graphe.\n");
        return NULL;
    }
    
    graphe->nbsom = r->nbNoeuds;
    graphe->T_som = (Sommet**)malloc(graphe->nbsom * sizeof(Sommet*));
    if (graphe->T_som == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de sommets.\n");
        free(graphe);
        return NULL;
    }
    
    // Initialisation des sommets
    CellNoeud* noeud = r->noeuds;
    for (int i = 0; i < graphe->nbsom; i++) {
        graphe->T_som[i] = (Sommet*)malloc(sizeof(Sommet));
        if (graphe->T_som[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour un sommet.\n");
            libererGraphe(graphe); // Libérer la mémoire allouée précédemment
            return NULL;
        }
        graphe->T_som[i]->num = noeud->nd->num;
        graphe->T_som[i]->x = noeud->nd->x;
        graphe->T_som[i]->y = noeud->nd->y;
        graphe->T_som[i]->L_voisin = NULL;
        noeud = noeud->suiv;
    }
    
    // Ajout des arêtes du graphe en parcourant les commodités du réseau
    graphe->gamma = r->gamma;
    graphe->nbcommod = 0;
    
    CellCommodite* commodite = r->commodites;
    while (commodite != NULL) {
        Arete* arete = (Arete*)malloc(sizeof(Arete));
        if (arete == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour une arête.\n");
            libererGraphe(graphe); 
            return NULL;
        }
        arete->u = commodite->extrA->num;
        arete->v = commodite->extrB->num;
        
        // Ajout de l'arête au sommet u
        Cellule_arete* cell_u = (Cellule_arete*)malloc(sizeof(Cellule_arete));
        if (cell_u == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour une cellule d'arête.\n");
            libererGraphe(graphe); 
            return NULL;
        }
        cell_u->a = arete;
        cell_u->suiv = graphe->T_som[arete->u]->L_voisin;
        graphe->T_som[arete->u]->L_voisin = cell_u;
        
        // Ajout de l'arête au sommet v
        Cellule_arete* cell_v = (Cellule_arete*)malloc(sizeof(Cellule_arete));
        if (cell_v == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour une cellule d'arête.\n");
            libererGraphe(graphe); 
            return NULL;
        }
        cell_v->a = arete;
        cell_v->suiv = graphe->T_som[arete->v]->L_voisin;
        graphe->T_som[arete->v]->L_voisin = cell_v;
        
        // Passage à la commodité suivante
        commodite = commodite->suiv;
        graphe->nbcommod++;
    }
    
    return graphe;
}

//FREE
void libererGraphe(Graphe* g) {
    if (g == NULL) return;
    if (g->T_som != NULL) {
        for (int i = 0; i < g->nbsom; i++) {
            Cellule_arete* current = g->T_som[i]->L_voisin;
            while (current != NULL) {
                Cellule_arete* next = current->suiv;
                free(current->a);
                free(current);
                current = next;
            }
            free(g->T_som[i]);
        }
        free(g->T_som);
    }
    free(g);
}

S_file* plusPetitNombreAretes(Sommet* u, Sommet* v, Graphe* graphe) {
    //On initialise la file pour le parcours en largeur
    S_file file;
    Init_file(&file);
    
    //Tableau pour stocker les sommets visités et les prédécesseurs
    int* visite = (int*)malloc(graphe->nbsom * sizeof(int));
    int* pred = (int*)malloc(graphe->nbsom * sizeof(int));
    
    //On initialise du tableau de visite à 0 et du tableau de prédécesseurs à -1
    for (int i = 0; i < graphe->nbsom; i++) {
        visite[i] = 0;
        pred[i] = -1;
    }
    
    //on enfile le sommet u et le marquer comme visité
    enfile(&file, u->num);
    visite[u->num] = 1;
    
    //On parcourt en largeur
    while (!estFileVide(&file)) {
        int sommet_courant = defile(&file);
        if (sommet_courant == v->num) {
            break;
        }
        //Parcours des voisins du sommet courant
        Cellule_arete* voisin = graphe->T_som[sommet_courant]->L_voisin;
        while (voisin != NULL) {
            int num_voisin = voisin->a->v;
            if (visite[num_voisin] == 0) {
                enfile(&file, num_voisin);
                visite[num_voisin] = 1;
                pred[num_voisin] = sommet_courant;
            }
            voisin = voisin->suiv;
        }
    }
    
    //Construction de la chaîne de u à v
    S_file* chaine = (S_file*)malloc(sizeof(S_file));
    Init_file(chaine);
    int sommet_courant = v->num;
    while (sommet_courant != -1) {
        enfile(chaine, sommet_courant);
        sommet_courant = pred[sommet_courant];
    }
    
    free(visite);
    free(pred);
    
    return chaine;
}

int reorganiseReseau(Reseau* r) {
    // Création du graphe correspondant au réseau
    Graphe* g = creerGraphe(r);

    // Matrice pour compter le nombre de chaînes passant par chaque arête
    int** matrice = (int**)malloc(g->nbsom * sizeof(int*));
    for (int i = 0; i < g->nbsom; i++) {
        matrice[i] = (int*)calloc(g->nbsom, sizeof(int));
    }

    // Calcul de la plus courte chaîne pour chaque commodité à l'aide de notre fonction 
    for (int i = 0; i < g->nbcommod; i++) {
        Commod commodite = g->T_commod[i];
        S_file* chaine = plusPetitNombreAretes(g->T_som[commodite.e1], g->T_som[commodite.e2], g);

        // Mise à jour de la matrice avec la chaîne calculée
        Cellule_file* cell = chaine->tete;
        while (cell != NULL && cell->suiv != NULL) {
            matrice[cell->val][cell->suiv->val]++;
            matrice[cell->suiv->val][cell->val]++;
            cell = cell->suiv;
        }

        // On libere  la mémoire allouée pour la chaîne
        while (!estFileVide(chaine)) {
            defile(chaine);
        }
        free(chaine);
    }

    //On vérifie si le nombre de chaînes qui passent par chaque arête est inférieur à γ
    for (int i = 0; i < g->nbsom; i++) {
        for (int j = i + 1; j < g->nbsom; j++) { // On parcourt seulement la moitié supérieure de la matrice car elle est symétrique
            if (matrice[i][j] > g->gamma) {
                for (int k = 0; k < g->nbsom; k++) {
                    free(matrice[k]);
                }
                free(matrice);
                libererGraphe(g);
                return 0; // Faux
            }
        }
    }

    // on libére de la mémoire allouée pour la matrice et le graphe
    for (int i = 0; i < g->nbsom; i++) {
        free(matrice[i]);
    }
    free(matrice);
    libererGraphe(g);

    return 1; // wouhou ca marche
}