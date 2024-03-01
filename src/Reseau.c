#include "../header/Reseau.h"
#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*Cette fonction permet de rechercher si un noeud est déjà présent dans le réseau
Si c'est le cas alors elle retourne ce noeud
Sinon elle en crée un en mettant à jour le nbNoeuds de R*/
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    if (R == NULL) {
        perror("R non définie dans rechercheCreeNoeudListe");
        return NULL;
    }

    // Recherche si le noeud existe déjà
    CellNoeud *cn = R->noeuds;
    while (cn != NULL) {
        if (cn->nd->x == x && cn->nd->y == y) {
            return cn->nd;
        }
        cn = cn->suiv;
    }

    // Si le noeud n'existe pas, on le crée
    printf("Création d'un nouveau noeud de coordonnées (x:%lf, y:%lf)\n", x, y);
    
    // Allocation mémoire pour le noeud
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));
    if (n == NULL) {
        perror("Erreur lors de l'allocation mémoire de n dans RechercheCreeeNoeudListe");
        return NULL;
    }

    // Initialisation des valeurs pour le noeud
    n->x = x;
    n->y = y;
    n->num = R->nbNoeuds + 1;

    // Allocation mémoire pour la cellule noeud
    CellNoeud *c_n = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (c_n == NULL) {
        perror("Erreur lors de l'allocation mémoire de c_n dans RechercheCreeeNoeudListe");
        free(n); // Libération de la mémoire allouée pour n
        return NULL;
    }

    // Relie le noeud à la liste chaînée de noeuds
    c_n->nd = n;
    c_n->suiv = R->noeuds;
    R->noeuds = c_n;

    // Incrémentation du nombre de noeuds du réseau
    R->nbNoeuds++;

    return n;
}

Reseau* reconstitueReseauListe(Chaines *C) {
    // Vérification de C
    if (C == NULL) {
        perror("Erreur de C dans reconstitutionReseauListe");
        return NULL;
    }

    // Création du réseau
    Reseau *r = (Reseau*)malloc(sizeof(Reseau));
    if (r == NULL) {
        perror("Erreur lors de l'allocation mémoire de r dans reconstitutionReseauListe");
        return NULL;
    }

    // Initialisation des attributs du réseau
    r->noeuds = NULL;
    r->commodites = NULL;
    r->nbNoeuds = 0;
    r->gamma = C->gamma;

    // Parcours des chaînes
    CellChaine *cellChaines = C->chaines;
    while (cellChaines != NULL) {
        CellNoeud *c_n_temp = NULL;
        CellCommodite *ccom = NULL;

        CellPoint *cellPoint = cellChaines->points;

        while (cellPoint != NULL) {
            // Recherche ou création du noeud
            Noeud *n = rechercheCreeNoeudListe(r, cellPoint->x, cellPoint->y);

            // Allocation mémoire pour la cellule noeud
            CellNoeud *c_n = (CellNoeud*)malloc(sizeof(CellNoeud));
            if (c_n == NULL) {
                perror("Erreur lors de l'allocation mémoire de c_n dans reconstitutionReseauListe");
                return NULL;
            }

            // Initialisation des valeurs pour la cellule noeud
            c_n->nd = n;

            // Relier la cellule noeud à la liste chaînée de noeuds
            if (r->noeuds == NULL) {
                r->noeuds = c_n;
            } else {
                c_n_temp->suiv = c_n;
            }
            c_n_temp = c_n;

            // Gestion des commodités
            if (cellPoint == cellChaines->points) {
                // Cad si c'est le premier point de la chaîne, on crée une nouvelle commodité
                ccom = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (ccom == NULL) {
                    perror("Erreur lors de l'allocation mémoire de ccom dans reconstitutionReseauListe");
                    return NULL;
                }
                ccom->extrA = n;
                ccom->extrB = NULL;

                //Gestion de la liste chaînee des commodites
                ccom->suiv = r->commodites;
                r->commodites = ccom;
            } else if (cellPoint->suiv == NULL) {
                // Cad si c'est le dernier point de la chaîne, on met à jour la deuxième extrémité de la commodité
                if (ccom != NULL) {
                    ccom->extrB = n;
                }
            }

            cellPoint = cellPoint->suiv;
        }

        cellChaines = cellChaines->suiv;
    }

    return r;
}
/*
// Cette fonction permet de compte le nombre de liaison dans un réseau
int nbLiaisons(Reseau *R){
    if(R==NULL){
        perror("Erreur dans nbLiaisons de R");
        return -1;
    }
    R->
        
    return 0;
}
*/

// Cette fonction permet de compter le nombre de commodites dans un réseau 
int nbCommodites(Reseau *R){
    if(R==NULL){
        perror("Erreur dans nbLiaisons de R");
        return -1;
    }

    CellCommodite* ccom=R->commodites;
    int compteur=0;
    
    while (ccom)
    {
        compteur++;
    }
    return compteur;
}



void ecrireReseau(Reseau *R, FILE *f){
    if(R==NULL){
        fprintf(stderr,"Erreur de R dnas ecrire Réseau");
        return;
    }
    if(f==NULL){
        fprintf(stderr,"Erreur de fichier dans ecrire réseau");
        return;
    }
    
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}