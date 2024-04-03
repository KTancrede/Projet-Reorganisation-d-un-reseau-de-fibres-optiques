#include "../header/Reseau.h"
#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/ArbreQuat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {
    if(C==NULL){
        perror("Erreur C vide dans chaineCoordMinMax\n");
    }
    *xmin = C->chaines->points->x;
    *ymin = C->chaines->points->y;
    *xmax = C->chaines->points->x;
    *ymax = C->chaines->points->y;

    CellChaine *cc = C->chaines;
    while (cc != NULL) {
        CellPoint *cp = cc->points;
        while (cp != NULL) {
            if (cp->x > *xmax) {
                *xmax = cp->x;
            }
            if (cp->y > *ymax) {
                *ymax = cp->y;
            }
            if (cp->x < *xmin) {
                *xmin = cp->x;
            }
            if (cp->y < *ymin) {
                *ymin = cp->y;
            }

            cp = cp->suiv;
        }
        cc = cc->suiv;
    }
}

ArbreQuat *creerArbreQuat(double xc, double yc, double coteX,double coteY){
    ArbreQuat *AQ=(ArbreQuat *)malloc(sizeof(ArbreQuat));
    AQ->xc=xc;
    AQ->xc=yc;
    AQ->coteX=coteX;
    AQ->coteY=coteY;

    Noeud * n=(Noeud *)malloc(sizeof(Noeud));
    //n->x=xc;
    //n->y=yc;
    CellNoeud *cn=(CellNoeud*)malloc(sizeof(CellNoeud));
    n->voisins=cn;

    ArbreQuat *so=(ArbreQuat *)malloc(sizeof(ArbreQuat));
    ArbreQuat *se=(ArbreQuat *)malloc(sizeof(ArbreQuat));
    ArbreQuat *no=(ArbreQuat *)malloc(sizeof(ArbreQuat));
    ArbreQuat *ne=(ArbreQuat *)malloc(sizeof(ArbreQuat));

    AQ->so=so;
    AQ->se=se;
    AQ->no=no;
    AQ->ne=ne;

    return AQ;
}
char* ou_inserer(Noeud* n,ArbreQuat *parent){
    if(n==NULL){
        perror("n null dans ou_inserer\n");
        return NULL;
    }
    if(parent==NULL){
        perror("parent null dans ou_inserer\n");
        return NULL;
    }
    if (parent->se->xc > n->x) {
        if (parent->se->yc > n->y) {
            return "so";
        } else {
            return "no";
        }
    } else {
        if (parent->se->yc > n->y) {
            return "se";
        } else {
            return "ne";
        }
    }
    
}
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat *parent){
    if(n==NULL){
        perror("Erreur de n dans insererNoeudArbre\n");
    }
    if(parent==NULL){
        perror("Erreur de n dans insererNoeudArbre\n");
    }
    // Cas d'un arbre vide
    if(*a==NULL){
        char *loc=ou_inserer(n,parent);
        double new_xc;
        double new_yc;

        double moitie_coteX=parent->coteX/2.0;
        double moitie_coteY=parent->coteY/2.0;

        if (strcmp(loc, "so") == 0) {
            new_xc = parent->xc - moitie_coteX;
            new_yc = parent->yc - moitie_coteY;
        } else if (strcmp(loc, "se") == 0) {
            new_xc = parent->xc + moitie_coteX;
            new_yc = parent->yc - moitie_coteY;
        } else if (strcmp(loc, "no") == 0) {
            new_xc = parent->xc - moitie_coteX;
            new_yc = parent->yc + moitie_coteY;
        } else if (strcmp(loc, "ne") == 0) {
            new_xc = parent->xc + moitie_coteX;
            new_yc = parent->yc + moitie_coteY;
        } else {
            perror("loc invalide\n");
            return;
        }      

        // Création de l'arbre
        *a = creerArbreQuat(new_xc, new_yc, moitie_coteX, moitie_coteY);
        //insererNoeudArbre(n, a, paent); // Insertion récursive dans le nouvel arbre
    }
    else if ((*a)->noeud != NULL) {
        // Sauvegarde de l'ancien noeud
        Noeud* old_node = (*a)->noeud;
        // Insertion récursive du nœud actuel et de l'ancien nœud
        insererNoeudArbre(n, a, parent);
        insererNoeudArbre(old_node, a, parent);
        // Mise à jour du noeud de la feuille
        (*a)->noeud = NULL;
    } 
    // Cas d'une cellule interne
    else {
        // Insertion récursive dans la cellule appropriée en fonction des coordonnées du nœud
        char* quadrant = ou_inserer(n, *a);
        ArbreQuat** enfant = NULL;
        if (strcmp(quadrant, "so") == 0) {
            enfant = &((*a)->so);
        } else if (strcmp(quadrant, "se") == 0) {
            enfant = &((*a)->se);
        } else if (strcmp(quadrant, "no") == 0) {
            enfant = &((*a)->no);
        } else if (strcmp(quadrant, "ne") == 0) {
            enfant = &((*a)->ne);
        } else {
            perror("Quadrant invalide\n");
            return;
        }
        insererNoeudArbre(n, enfant, *a);
    }

}