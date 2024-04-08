#include "../header/Reseau.h"
#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/ArbreQuat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EPSILON 0.01

void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax) {
    if (C == NULL || C->chaines == NULL || C->chaines->points == NULL) {
        fprintf(stderr, "Erreur : Chaîne de caractères vide ou mal formatée dans chaineCoordMinMax\n");
        return;
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

char *ou_inserer(Noeud *n, ArbreQuat *parent) {
    if (n == NULL || parent == NULL) {
        fprintf(stderr, "Erreur : Paramètres n ou parent nuls dans ou_inserer\n");
        return NULL;
    }

    if (n->x < parent->xc) {
        if (n->y < parent->yc) {
            return "so";
        } else {
            return "no";
        }
    } else {
        if (n->y < parent->yc) {
            return "se";
        } else {
            return "ne";
        }
    }
}
void insererNoeudArbre(Noeud *n, ArbreQuat **a, ArbreQuat *parent) {
    // Vérifie si le nœud et le parent sont valides
    if (n == NULL || parent == NULL) {
        fprintf(stderr, "Erreur : Paramètres n ou parent sont nuls dans insererNoeudArbre\n");
        return;
    }

    // Si l'arbre est vide, crée un nouvel arbre avec le nœud donné
    if (*a == NULL) {
        *a = creerArbreQuat(n->x, n->y, parent->coteX / 2, parent->coteY / 2);
        if (*a == NULL) {
            fprintf(stderr, "Erreur : Échec de création de l'arbre quaternaire\n");
            return;
        }
        (*a)->noeud = n;
        printf("On crée un arbre pour n->:%d\n", n->num);
        return;
    }

    // Si l'arbre est une feuille
    if ((*a)->noeud != NULL) {
        // Vérifie si le nœud existe déjà dans l'arbre
        if ((*a)->noeud->x == n->x && (*a)->noeud->y == n->y) {
            printf("On est dans un point déjà présent pour n->:%d\n", n->num);
            return;
        } else {
            // Insère les deux nœuds dans l'arbre
            Noeud *old_noeud = (*a)->noeud;
            printf("L'arbre est une feuille dans un nœud inexistant pour n->:%d\n", n->num);
            (*a)->noeud = NULL; // L'arbre devient une cellule interne
            insererNoeudArbre(old_noeud, a, parent);
            insererNoeudArbre(n, a, parent);
            return;
        }
    }

    // Si l'arbre est une cellule interne
    printf("L'arbre est une cellule interne pour n->:%d\n", n->num);
    char *quadrant = ou_inserer(n, *a);
    ArbreQuat *enfant = NULL;
    if (strcmp(quadrant, "so") == 0) {
        enfant = (*a)->so;
    } else if (strcmp(quadrant, "se") == 0) {
        enfant = (*a)->se;
    } else if (strcmp(quadrant, "no") == 0) {
        enfant = (*a)->no;
    } else if (strcmp(quadrant, "ne") == 0) {
        enfant = (*a)->ne;
    } else {
        fprintf(stderr, "Erreur : Quadrant invalide dans insererNoeudArbre\n");
        return;
    }
    insererNoeudArbre(n, &enfant, *a);
}




Noeud *rechercheCreeNoeudArbre(Reseau *R, ArbreQuat **a, ArbreQuat *parent, double x, double y) {
    if (*a == NULL) {
        Noeud *new_node = (Noeud *)malloc(sizeof(Noeud));
        if (new_node == NULL) {
            fprintf(stderr, "Erreur : Allocation mémoire échouée pour le nouveau nœud\n");
            return NULL;
        }
        new_node->x = x;
        new_node->y = y;
        new_node->num = R->nbNoeuds + 1;
        insererNoeudArbre(new_node, a, parent);
        return new_node;

    } else if ((*a)->noeud != NULL) {
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) {
            return (*a)->noeud; // Le nœud existe déjà, retourne le nœud existant
        } else {
            // Les coordonnées ne correspondent pas, descendre plus profondément dans l'arbre
            // pour continuer la recherche ou l'insertion
            Noeud *new_node = (Noeud *)malloc(sizeof(Noeud));
            if (new_node == NULL) {
                fprintf(stderr, "Erreur : Allocation mémoire échouée pour le nouveau nœud\n");
                return NULL;
            }
            new_node->x = x;
            new_node->y = y;
            new_node->num = R->nbNoeuds + 1;
            insererNoeudArbre(new_node, a, parent);
            return new_node;
        }
    } else {
        // L'arbre est une cellule interne, continuez à descendre dans l'arbre pour rechercher ou insérer le nœud
        Noeud *nouveauNoeud = (Noeud *)malloc(sizeof(Noeud));
        if (nouveauNoeud == NULL) {
            fprintf(stderr, "Erreur : Allocation mémoire échouée pour le nouveau nœud\n");
            return NULL;
        }
        nouveauNoeud->x = x;
        nouveauNoeud->y = y;
        nouveauNoeud->num = R->nbNoeuds + 1;
        char *quadrant = ou_inserer(nouveauNoeud, *a);
        if (quadrant == NULL) {
            fprintf(stderr, "Erreur : Quadrant invalide dans rechercheCreeNoeudArbre\n");
            free(nouveauNoeud);
            return NULL;
        }
        ArbreQuat **child_tree = NULL;
        if (strcmp(quadrant, "so") == 0) {
            child_tree = &((*a)->so);
        } else if (strcmp(quadrant, "se") == 0) {
            child_tree = &((*a)->se);
        } else if (strcmp(quadrant, "no") == 0) {
            child_tree = &((*a)->no);
        } else if (strcmp(quadrant, "ne") == 0) {
            child_tree = &((*a)->ne);
        } else {
            fprintf(stderr, "Erreur : Quadrant invalide dans rechercheCreeNoeudArbre\n");
            free(nouveauNoeud);
            return NULL;
        }
        return rechercheCreeNoeudArbre(R, child_tree, *a, x, y);
    }
}

void libererArbreQuat(ArbreQuat *a) {
    if (a == NULL) {
        return;
    }
    libererArbreQuat(a->so);
    libererArbreQuat(a->se);
    libererArbreQuat(a->no);
    libererArbreQuat(a->ne);
    free(a->noeud);
    free(a);
}

int noeudExiste(Reseau *R, Noeud *n) {
    CellNoeud *tmp = R->noeuds;
    while (tmp != NULL) {
        if (tmp->nd == n) {
            return 1; // Le nœud existe déjà dans le réseau
        }
        tmp = tmp->suiv;
    }
    return 0; // Le nœud n'existe pas dans le réseau
}


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
        Noeud *premierNoeud = NULL;
        CellPoint *cellPoint = cellChaine->points;
        Noeud *noeudPrecedent = NULL;
        CellCommodite *ccom = NULL;

        while (cellPoint != NULL) {
            double x = cellPoint->x;
            double y = cellPoint->y;
            
            // Recherche ou création du nœud dans l'arbre
            Noeud *noeud = rechercheCreeNoeudArbre(reseau, &arbre, arbre, x, y);
            
            // Vérifie si le nœud existe déjà dans le réseau
            if (!noeudExiste(reseau, noeud)) {
                // Ajoute le nœud au réseau
                CellNoeud *nouveau = (CellNoeud *)malloc(sizeof(CellNoeud));
                if (nouveau == NULL) {
                    perror("Erreur lors de l'allocation mémoire de la cellule de nœud");
                    liberer_reseau(reseau);
                    libererArbreQuat(arbre);
                    return NULL;
                }
                nouveau->nd = noeud;
                nouveau->suiv = reseau->noeuds;
                reseau->noeuds = nouveau;
                reseau->nbNoeuds++;
            }

            // Mémoriser le premier nœud de la chaîne
            if (premierNoeud == NULL) {
                premierNoeud = noeud;
            }

            // Gestion des voisins
            if (noeudPrecedent != NULL) {
                ajouterVoisins(noeudPrecedent, noeud);
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

            noeudPrecedent = noeud;
            cellPoint = cellPoint->suiv;
        }
        cellChaine = cellChaine->suiv;
    }
    
    return reseau;
}







