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
            //Si le point est deja present à ces coordonnes on return le noeud
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
    n->voisins=NULL;
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
//On veut ajouter n1 à n2
void ajouterVoisins(Noeud *n1, Noeud *n2) {
    if (n1 == NULL || n2 == NULL) {
        perror("Erreur dans ajouterVoisins de paramètres\n");
        return;
    }

    // Vérifier si n2 est déjà dans la liste des voisins de n1
    CellNoeud *c_n_voisins = n1->voisins;
    while (c_n_voisins != NULL) {
        if (c_n_voisins->nd == n2) {
            // Si n2 est déjà un voisin de n1, ne rien faire
            return;
        }
        c_n_voisins = c_n_voisins->suiv;
    }

    // Allocation mémoire pour la nouvelle cellule de voisin
    CellNoeud *nouveau_voisin1 = (CellNoeud*)malloc(sizeof(CellNoeud));
    CellNoeud *nouveau_voisin2 = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (nouveau_voisin1 == NULL || nouveau_voisin2 == NULL) {
        perror("Erreur lors de l'allocation mémoire de nouveau_voisin dans ajouterVoisins");
        return;
    }

    // Assigner le nœud n2 à la nouvelle cellule de voisin pour n1
    nouveau_voisin1->nd = n2;
    nouveau_voisin1->suiv = n1->voisins;
    n1->voisins = nouveau_voisin1;

    // Assigner le nœud n1 à la nouvelle cellule de voisin pour n2
    nouveau_voisin2->nd = n1;
    nouveau_voisin2->suiv = n2->voisins;
    n2->voisins = nouveau_voisin2;
}



// Cette fonction permet de reconstituer un réseau à partir des chaînes
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
        CellCommodite *ccom = NULL;
        Noeud *premierNoeud = NULL;  // Pour garder une référence au premier nœud de la chaîne

        CellPoint *cellPoint = cellChaines->points;
        Noeud *noeudPrecedent = NULL;

        while (cellPoint != NULL) {
            // Recherche ou création du nœud
            Noeud *n = NULL;
            n = rechercheCreeNoeudListe(r, cellPoint->x, cellPoint->y);

            // Mémoriser le premier nœud de la chaîne
            if (premierNoeud == NULL) {
                premierNoeud = n;
            }

            // Gestion des commodités
            if (cellPoint == cellChaines->points) {
                // Si c'est le premier point de la chaîne, créer une nouvelle commodité
                ccom = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (ccom == NULL) {
                    perror("Erreur lors de l'allocation mémoire de ccom dans reconstitutionReseauListe");
                    return NULL;
                }
                ccom->extrA = n;
                ccom->extrB = NULL;

                // Gestion de la liste chaînée des commodités
                ccom->suiv = r->commodites;
                r->commodites = ccom;
            } else if (cellPoint->suiv == NULL) {
                // Si c'est le dernier point de la chaîne, mettre à jour la deuxième extrémité de la commodité
                if (ccom != NULL) {
                    ccom->extrB = n;
                }
            }

            // Ajout du nœud actuel à la liste des voisins du nœud précédent (si ce n'est pas le premier nœud)
            if (noeudPrecedent != NULL) {
                ajouterVoisins(noeudPrecedent, n);
            }

            noeudPrecedent = n;
            cellPoint = cellPoint->suiv;
        }

        // Relie le dernier nœud de la chaîne au premier nœud de la chaîne suivante
        if (premierNoeud != NULL && noeudPrecedent != NULL) {
            //ajouterVoisins(noeudPrecedent, premierNoeud);
        }

        cellChaines = cellChaines->suiv;
    }

    return r;
}


void afficher_voisin(Reseau *R) {
    if (R == NULL) {
        printf("Le réseau est vide.\n");
        return;
    }

    printf("Voisins de chaque nœud :\n");

    // Parcours de chaque nœud
    CellNoeud *courant_noeud = R->noeuds;
    while (courant_noeud != NULL) {
        printf("Noeud %d : ", courant_noeud->nd->num);

        // Parcours des voisins du nœud courant
        CellNoeud *courant_voisin = courant_noeud->nd->voisins;
        while (courant_voisin != NULL) {
            printf("%d ", courant_voisin->nd->num);
            courant_voisin = courant_voisin->suiv;
        }

        printf("\n");
        courant_noeud = courant_noeud->suiv;
    }
}


// Cette fonction permet de compte le nombre de liaison dans un réseau
int nbLiaisons(Reseau *R){
    if(R==NULL){
        perror("Erreur dans nbLiaisons de R\n");
        return -1;
    }
    int compteur=0;
    CellNoeud *c_n=R->noeuds;

    while (c_n!=NULL){
        CellNoeud *c_n_voisins=c_n->nd->voisins;
        while(c_n_voisins!=NULL){
            compteur++;
            c_n_voisins=c_n_voisins->suiv;
        }
        c_n=c_n->suiv;
    }
    
        
    return compteur/2;
}

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
        ccom=ccom->suiv;
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
    fprintf(f,"NbNoeuds: %d\n",R->nbNoeuds);
    fprintf(f,"NbLiaisons: %d\n",nbLiaisons(R));
    fprintf(f,"NbCommodites: %d\n",nbCommodites(R));
    fprintf(f,"Gamma: %d\n\n",R->gamma);

    // Écrire les coordonnées de chaque nœud
    CellNoeud *c_n = R->noeuds;
    while (c_n != NULL) {
        fprintf(f, "v %d %.6lf %.6lf\n", c_n->nd->num, c_n->nd->x, c_n->nd->y);
        c_n = c_n->suiv;
    }

    fprintf(f, "\n");

    // Écrire les liaisons entre les nœuds
    CellNoeud *c_n_voisin1 = R->noeuds;
    while (c_n_voisin1 != NULL) {
        
        CellNoeud *c_n_voisin = c_n_voisin1->nd->voisins;
        while (c_n_voisin != NULL) {
            //printf("%d %d\n",c_n_voisin1->nd->num ,c_n_voisin->nd->num);
            // ICI on empeche qu'une liaisons soit ecrites plusieurs fois en verifiant le num
            if (c_n_voisin1->nd->num < c_n_voisin->nd->num) {   
                fprintf(f, "l %d %d\n", c_n_voisin1->nd->num, c_n_voisin->nd->num);
            }
            c_n_voisin = c_n_voisin->suiv;
        }
        c_n_voisin1 = c_n_voisin1->suiv;
    }

    fprintf(f, "\n");

    // Écrire les commodités
    CellCommodite *ccom = R->commodites;
    while (ccom != NULL) {
        fprintf(f, "k %d %d\n", ccom->extrA->num, ccom->extrB->num);
        ccom = ccom->suiv;
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

void liberer_commodite(CellCommodite *liste_commodite) {
    CellCommodite *courant_commodite = liste_commodite;
    while (courant_commodite != NULL) {
        CellCommodite *suivant_commodite = courant_commodite->suiv;
        free(courant_commodite);
        courant_commodite = suivant_commodite;
    }
}

void liberer_noeud(Noeud *n){
    if(n == NULL) {
        return;
    }
    CellNoeud *courant = n->voisins;
    while (courant != NULL) {
        CellNoeud *suivant = courant->suiv;
        free(courant);
        courant = suivant;
    }
    free(n);
}

void liberer_reseau(Reseau *R){
    if(R == NULL) {
        return; 
    }
    liberer_commodite(R->commodites);

    CellNoeud *courant_noeud = R->noeuds;
    while (courant_noeud != NULL) {
        CellNoeud *suivant_noeud = courant_noeud->suiv;
        liberer_noeud(courant_noeud->nd); 
        free(courant_noeud);
        courant_noeud = suivant_noeud;
    }
    free(R);
}
