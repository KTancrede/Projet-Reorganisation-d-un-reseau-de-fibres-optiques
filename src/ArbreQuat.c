#include "../header/Reseau.h"
#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/ArbreQuat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Fonction qui set le min et max d'une chaine aux paramètres en entrées
void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {
    if (C == NULL || C->chaines == NULL || C->chaines->points == NULL) {
        fprintf(stderr, "Erreur : Chaîne de caractères vide ou mal formatée dans chaineCoordMinMax\n");
        return;
    }
    //On prend les premiers points quon fixe à min et max
    *xmin = C->chaines->points->x;
    *ymin = C->chaines->points->y;
    *xmax = C->chaines->points->x;
    *ymax = C->chaines->points->y;
    //On parcourt la chaine pour pour trouver le min et max
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
//Fonction qui permet la création d'un arbre Quaternaire à l'aide d'un point c et de coteX et coteY
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat *AQ = (ArbreQuat *)malloc(sizeof(ArbreQuat));
    if (AQ == NULL) {
        fprintf(stderr, "Erreur : Allocation mémoire échouée pour l'arbre quaternaire\n");
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
//Fonction qui permet d'inserer un noeud à un arbre
void insererNoeudArbre(Noeud* nouveauNoeud, ArbreQuat** arbre, ArbreQuat* parent) {
    if (!nouveauNoeud || !parent) return ;

    // Arbre vide
    if ((*arbre) == NULL) {
        double nouveauCoteX = parent->coteX * 1.0 / 2;
        double nouveauCoteY = parent->coteY * 1.0 / 2;
        double nouveauXc, nouveauYc;

        if (nouveauNoeud->x < parent->xc) {
            nouveauXc = parent->xc - parent->coteX * 1.0 / 4;
        } else {
            nouveauXc = parent->xc + parent->coteX * 1.0 / 4;
        }
        if (nouveauNoeud->y < parent->yc) {
            nouveauYc = parent->yc - parent->coteY * 1.0 / 4;
        } else {
            nouveauYc = parent->yc + parent->coteY * 1.0 / 4;
        }
        (*arbre) = creerArbreQuat(nouveauXc, nouveauYc, nouveauCoteX, nouveauCoteY);
        (*arbre)->noeud = nouveauNoeud;
        return ;
    }

    // Feuille
    if ((*arbre)->noeud != NULL) {
        Noeud* ancienNoeud = (*arbre)->noeud;

        if (nouveauNoeud->x < (*arbre)->xc && nouveauNoeud->y < (*arbre)->yc) {
            insererNoeudArbre(nouveauNoeud, &((*arbre)->so), (*arbre));
        } else if (nouveauNoeud->x >= (*arbre)->xc && nouveauNoeud->y < (*arbre)->yc) {
            insererNoeudArbre(nouveauNoeud, &((*arbre)->se), (*arbre));
        } else if (nouveauNoeud->x < (*arbre)->xc && nouveauNoeud->y >= (*arbre)->yc) {
            insererNoeudArbre(nouveauNoeud, &((*arbre)->no), (*arbre));
        } else {
            insererNoeudArbre(nouveauNoeud, &((*arbre)->ne), (*arbre));
        }

        if (ancienNoeud->x < (*arbre)->xc && ancienNoeud->y < (*arbre)->yc) {
            insererNoeudArbre(ancienNoeud, &((*arbre)->so), (*arbre));
        } else if (ancienNoeud->x >= (*arbre)->xc && ancienNoeud->y < (*arbre)->yc) {
            insererNoeudArbre(ancienNoeud, &((*arbre)->se), (*arbre));
        } else if (ancienNoeud->x < (*arbre)->xc && ancienNoeud->y >= (*arbre)->yc) {
            insererNoeudArbre(ancienNoeud, &((*arbre)->no), (*arbre));
        } else {
            insererNoeudArbre(ancienNoeud, &((*arbre)->ne), (*arbre));
        }

        (*arbre)->noeud = NULL;
        return ;
    }

    // Cellule interne
    if (nouveauNoeud->x < (*arbre)->xc && nouveauNoeud->y < (*arbre)->yc) {
        insererNoeudArbre(nouveauNoeud, &((*arbre)->so), (*arbre));
    } else if (nouveauNoeud->x >= (*arbre)->xc && nouveauNoeud->y < (*arbre)->yc) {
        insererNoeudArbre(nouveauNoeud, &((*arbre)->se), (*arbre));
    } else if (nouveauNoeud->x < (*arbre)->xc && nouveauNoeud->y >= (*arbre)->yc) {
        insererNoeudArbre(nouveauNoeud, &((*arbre)->no), (*arbre));
    } else {
        insererNoeudArbre(nouveauNoeud, &((*arbre)->ne), (*arbre));
    }
}
//Fonction qui retourne un Noeud du réseau R correspondant au point de coordonnées (x, y) dans l’arbre quaternaire
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    ArbreQuat * arbre = *a;
    //Arbre null
    if (arbre == NULL) {
        Noeud * nouveau_noeud = (Noeud *)malloc(sizeof(Noeud));
        nouveau_noeud -> num = (R -> nbNoeuds) + 1;
        nouveau_noeud -> x = x;
        nouveau_noeud -> y = y;
        nouveau_noeud -> voisins = NULL;

        CellNoeud * nouvelle_cellule = (CellNoeud *)malloc(sizeof(CellNoeud));
        nouvelle_cellule -> nd = nouveau_noeud;
        nouvelle_cellule -> suiv = R -> noeuds;
        R -> noeuds = nouvelle_cellule;
        R -> nbNoeuds++;

        insererNoeudArbre(nouveau_noeud, &(*a), parent);
        return nouveau_noeud;
    }

    //Feuille
    if (arbre -> noeud != NULL) {
        if (arbre -> noeud -> x == x && arbre -> noeud -> y == y) {
            return arbre -> noeud;
        }
        Noeud * nouveau_noeud = (Noeud *)malloc(sizeof(Noeud));
        nouveau_noeud -> num = (R -> nbNoeuds) + 1;
        nouveau_noeud -> x = x;
        nouveau_noeud -> y = y;
        nouveau_noeud -> voisins = NULL;

        CellNoeud * nouvelle_cellule = (CellNoeud *)malloc(sizeof(CellNoeud));
        nouvelle_cellule -> nd = nouveau_noeud;
        nouvelle_cellule -> suiv = R -> noeuds;
        R -> noeuds = nouvelle_cellule;
        R -> nbNoeuds++;

        insererNoeudArbre(nouveau_noeud, &(*a), parent);
        return nouveau_noeud;
    }

    //Cellule interne
    if (x < arbre -> xc && y < arbre -> yc) {
        return rechercheCreeNoeudArbre(R, &(arbre -> so), arbre, x, y);
    }
    else if (x < arbre -> xc && y >= arbre -> yc) {
        return rechercheCreeNoeudArbre(R, &(arbre -> no), arbre, x, y);
    }
    else if (x >= arbre -> xc && y < arbre -> yc) {
        return rechercheCreeNoeudArbre(R, &(arbre -> se), arbre, x, y);
    }
    else {
        return rechercheCreeNoeudArbre(R, &(arbre -> ne), arbre, x, y);
    }
}
//Fonction qui crée un Réseau à partir d'une chaîne C
Reseau *reconstitueReseauArbre(Chaines *C) {
    if (C == NULL || C->chaines == NULL) {
        fprintf(stderr, "Erreur : Chaîne de caractères vide ou mal formatée dans reconstitueReseauArbre\n");
        return NULL;
    }

    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

    // Calcul des coordonnées du centre et de la taille de l'arbre
    double xc = (xmin + xmax) / 2.0;
    double yc = (ymin + ymax) / 2.0;
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    
    // Création de l'arbre
    ArbreQuat *arbre = creerArbreQuat(xc, yc, coteX, coteY);
    if (arbre == NULL) {
        fprintf(stderr, "Erreur : Échec de création de l'arbre quaternaire\n");
        return NULL;
    }

    // Création du réseau
    Reseau *reseau = (Reseau *)malloc(sizeof(Reseau));
    if (reseau == NULL) {
        fprintf(stderr, "Erreur : Échec de création du réseau\n");
        libererArbreQuat(arbre);
        return NULL;
    }

    reseau->noeuds = NULL;
    reseau->commodites = NULL;
    reseau->nbNoeuds = 0;
    reseau->gamma = C->gamma;

    // Parcours des chaines pour insérer les noeuds dans l'arbre et les ajouter au réseau
    CellChaine *cellChaine = C->chaines;
    while (cellChaine != NULL) {
        CellCommodite *ccom = NULL;
        Noeud *premierNoeud = NULL;

        CellPoint *cellPoint = cellChaine->points;
        Noeud *noeudPrecedent = NULL;

        while (cellPoint != NULL) {
            // Recherche ou création du nœud dans l'arbre
            Noeud *noeud = NULL;
            
            noeud=rechercheCreeNoeudArbre(reseau, &arbre, arbre, cellPoint->x, cellPoint->y);
            
            // Mémoriser le premier nœud de la chaîne
            if (premierNoeud == NULL) {
                premierNoeud = noeud;
            }

            // Gestion des commodités
            if (cellPoint == cellChaine->points) {
                // Si c'est le premier point de la chaîne, créer une nouvelle commodité
                ccom = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (ccom == NULL) {
                    perror("Erreur lors de l'allocation mémoire de ccom dans reconstitutionReseauListe");
                    return NULL;
                }
                ccom->extrA = noeud;
                ccom->extrB = NULL;

                // Gestion de la liste chaînée des commodités
                ccom->suiv = reseau->commodites;
                reseau->commodites = ccom;
            } else if (cellPoint->suiv == NULL) {
                // Si c'est le dernier point de la chaîne, mettre à jour la deuxième extrémité de la commodité
                if (ccom != NULL) {
                    ccom->extrB = noeud;
                }
            }
            // Gestion des voisins
            if (noeudPrecedent != NULL) {
                ajouterVoisins(noeudPrecedent, noeud);
                ajouterVoisins(noeud, noeudPrecedent);
            }
            noeudPrecedent = noeud;
            cellPoint = cellPoint->suiv;
        }
        cellChaine = cellChaine->suiv;
    }
    libererArbreQuat(arbre);
    return reseau;
}

//free
void libererArbreQuat(ArbreQuat *a) {
    if (a == NULL) {
        return;
    }
    if(a->noeud != NULL){
        free(a);
        return;
    }
    libererArbreQuat(a->so);
    libererArbreQuat(a->se);
    libererArbreQuat(a->no);
    libererArbreQuat(a->ne);
    free(a);
}
