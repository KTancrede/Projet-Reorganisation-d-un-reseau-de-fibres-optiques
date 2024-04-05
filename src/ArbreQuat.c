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
/*
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
*/
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat *AQ = (ArbreQuat *)malloc(sizeof(ArbreQuat));
    if (AQ == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour l'arbre quaternaire\n");
        return NULL;
    }

    AQ->xc = xc;
    AQ->yc = yc;
    AQ->coteX = coteX;
    AQ->coteY = coteY;
    AQ->noeud = NULL;
    AQ->so = NULL;
    AQ->se = NULL;
    AQ->no = NULL;
    AQ->ne = NULL;

    return AQ;
}
char* ou_inserer(Noeud* n, ArbreQuat *parent) {
    if (n == NULL || parent == NULL) {
        perror("Erreur : Paramètres n ou parent nuls dans ou_inserer\n");
        return NULL;
    }

    if (parent->so != NULL && parent->so->xc <= n->x && parent->so->yc <= n->y) {
        return "so";
    } else if (parent->se != NULL && parent->se->xc > n->x && parent->se->yc <= n->y) {
        return "se";
    } else if (parent->no != NULL && parent->no->xc <= n->x && parent->no->yc > n->y) {
        return "no";
    } else if (parent->ne != NULL && parent->ne->xc > n->x && parent->ne->yc > n->y) {
        return "ne";
    } else {
        perror("Erreur : Impossible de déterminer le quadrant dans ou_inserer\n");
        return NULL;
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
        Noeud* old_noeud = (*a)->noeud;
        // Insertion récursive du nœud actuel et de l'ancien nœud
        insererNoeudArbre(n, a, parent);
        insererNoeudArbre(old_noeud, a, parent);
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

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    // Cas d'un arbre vide
    if (*a == NULL) {
        // Création du nœud correspondant au point et insertion dans le réseau et l'arbre
        Noeud* new_node = (Noeud*)malloc(sizeof(Noeud));
        if (new_node == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour le nouveau nœud\n");
            return NULL;
        }
        new_node->x = x;
        new_node->y = y;
        insererNoeudArbre(new_node, a, parent); // Insertion du nœud dans l'arbre
        return new_node;
    } 
    // Cas d'une feuille
    else if ((*a)->noeud != NULL) {
        // Vérification si le nœud recherché correspond au nœud de la feuille
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) {
            return (*a)->noeud;
        } else {
            // Création du nœud correspondant au point et insertion dans le réseau et l'arbre
            Noeud* new_node = (Noeud*)malloc(sizeof(Noeud));
            if (new_node == NULL) {
                perror("Erreur lors de l'allocation de mémoire pour le nouveau nœud\n");
                return NULL;
            }
            new_node->x = x;
            new_node->y = y;
            insererNoeudArbre(new_node, a, parent); // Insertion du nœud dans l'arbre
            return new_node;
        }
    } 
    // Cas d'une cellule interne
    else {
        // Détermination de la cellule de l'arbre dans laquelle chercher le nœud
        Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
        if (nouveauNoeud == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour le nouveau nœud\n");
            return NULL;
        }
        nouveauNoeud->x = x;
        nouveauNoeud->y = y;

        char* quadrant = ou_inserer(nouveauNoeud, *a);
        if (quadrant == NULL) {
            perror("Erreur lors de l'insertion du nouveau nœud\n");
            free(nouveauNoeud); // Libération de la mémoire allouée
            return NULL;
        }
        ArbreQuat** child_tree = NULL;
        if (strcmp(quadrant, "so") == 0) {
            child_tree = &((*a)->so);
        } else if (strcmp(quadrant, "se") == 0) {
            child_tree = &((*a)->se);
        } else if (strcmp(quadrant, "no") == 0) {
            child_tree = &((*a)->no);
        } else if (strcmp(quadrant, "ne") == 0) {
            child_tree = &((*a)->ne);
        } else {
            perror("Quadrant invalide\n");
            return NULL;
        }
        // Recherche récursive dans la cellule de l'arbre correspondante
        return rechercheCreeNoeudArbre(R, child_tree, *a, x, y);
    }
}

Reseau* reconstitueReseauArbre(Chaines *C) {
    if (C == NULL) {
        fprintf(stderr, "Erreur : Chaîne de caractères nulle dans reconstitueReseauArbre\n");
        return NULL;
    }

    // Détermination des coordonnées minimales et maximales de la chaîne
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

    // Calcul du centre et de la longueur/hauteur de l'arbre
    double xc = (xmin + xmax) / 2.0;
    double yc = (ymin + ymax) / 2.0;
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;

    // Création de l'arbre quaternaire
    ArbreQuat* arbre = creerArbreQuat(xc, yc, coteX, coteY);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur : Échec de création de l'arbre quaternaire\n");
        return NULL;
    }

    // Création du réseau
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
    if (reseau == NULL) {
        fprintf(stderr, "Erreur : Échec de création du réseau\n");
        libererArbreQuat(arbre); // Libération de la mémoire de l'arbre en cas d'échec
        return NULL;
    }

    // Initialisation des attributs du réseau
    reseau->noeuds = NULL;
    reseau->commodites = NULL;
    reseau->nbNoeuds = 0;
    reseau->gamma = C->gamma;

    // Parcours des chaînes pour insérer les nœuds dans l'arbre et le réseau
    CellChaine* cellChaine = C->chaines;
    while (cellChaine != NULL) {
        CellPoint* cellPoint = cellChaine->points;
        while (cellPoint != NULL) {
            double x = cellPoint->x;
            double y = cellPoint->y;
            // Recherche ou création du nœud dans l'arbre et le réseau
            Noeud* noeud = rechercheCreeNoeudArbre(reseau, &arbre, arbre, x, y);
            if (noeud == NULL) {
                fprintf(stderr, "Erreur : Échec de recherche ou création du nœud\n");
                libererArbreQuat(arbre); // Libération de la mémoire de l'arbre en cas d'échec
                free(reseau); // Libération de la mémoire du réseau en cas d'échec
                return NULL;
            }
            // Passage au point suivant dans la chaîne
            cellPoint = cellPoint->suiv;
        }
        // Passage à la chaîne suivante
        cellChaine = cellChaine->suiv;
    }
    libererArbreQuat(arbre);
    // Retour du réseau créé
    return reseau;
}


void libererArbreQuat(ArbreQuat* a) {
    if (a == NULL) {
        return;
    }

    // Libérer les enfants de manière récursive
    libererArbreQuat(a->so);
    libererArbreQuat(a->se);
    libererArbreQuat(a->no);
    libererArbreQuat(a->ne);

    free(a->noeud);
    // Libérer la cellule actuelle
    free(a);
}