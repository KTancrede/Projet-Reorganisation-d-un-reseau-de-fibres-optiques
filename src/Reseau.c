#include "../header/Reseau.h"
#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if(R==NULL){
        fprintf(stderr,"R non définie dans rechercheCreeNoeudListe\n");
        return NULL;
    }
    //Verification de s'il existe ou pas déjà ce noeud
    CellNoeud *cn=R->noeuds;
    while(cn!=NULL){
        if(cn->nd->x==x&&cn->nd->y==y){
            return cn->nd;
        }
        cn=cn->suiv;
    }

    //SInon on le crée et on vérifie les mallocs
    printf("Création d'un nouveau noeud de coordonnées (x:%lf,y:%lf)\n",x,y);
    CellNoeud *c_n=(CellNoeud*)malloc(sizeof(CellNoeud));
    if(c_n==NULL){
        fprintf(stderr,"Erreur lors de l'allocation mémoire de cn dans RechercheCreeeNoeudListe\n");
        return NULL;
    }
    Noeud *n=(Noeud*)malloc(sizeof(Noeud));
    if(n==NULL){
        fprintf(stderr,"Erreur lors de l'allocation mémoire de n dans RechercheCreeeNoeudListe\n");
        return NULL;
    }

    //On ajoute les valeurs pour le noeud
    n->x=x;
    n->y=y;
    n->num=R->nbNoeuds+1;
    
    //On relie le noeud a la lc et on le nomme
    c_n->nd=n;
    c_n->suiv=R->noeuds;

    //On incrémente le nombre de noeud
    R->nbNoeuds++;
    return n;
}
Reseau* reconstitueReseauListe(Chaines *C) {
    // Vérification de C
    if (C == NULL) {
        fprintf(stderr, "Erreur de C dans reconstitutionReseauListe");
        return NULL;
    }

    // Création du réseau
    Reseau *r = (Reseau*)malloc(sizeof(Reseau));
    if (r == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation mémoire de r dans reconstitutionReseauListe\n");
        return NULL;
    }

    // On fixe les attributs du réseau
    r->noeuds = NULL;
    r->commodites = NULL;
    r->nbNoeuds = 0;

    // Parcours des chaînes
    CellChaine *cellChaines = C->chaines;
    while (cellChaines != NULL) {
        CellNoeud *c_n = NULL;  // Initialisation de c_n
        CellNoeud *c_n_temp = NULL;

        CellPoint *cellPoint = cellChaines->points;
        while (cellPoint != NULL) {
            // On récupère (ou on crée) un noeud
            Noeud *n = rechercheCreeNoeudListe(r, cellPoint->x, cellPoint->y);

            // On alloue la mémoire pour c_n
            CellNoeud *c_n = (CellNoeud*)malloc(sizeof(CellNoeud));
            if (c_n == NULL) {
                fprintf(stderr, "Erreur lors de l'allocation mémoire de c_n dans reconstitutionReseauListe\n");
                return NULL;
            }

            // On ajoute les valeurs pour le noeud
            c_n->nd = n;

            // On relie le noeud à la liste chaînée de noeuds
            if (r->noeuds == NULL) {
                r->noeuds = c_n;
            } else {
                c_n_temp->suiv = c_n;
            }
            c_n_temp = c_n;

            cellPoint = cellPoint->suiv;
        }

        cellChaines = cellChaines->suiv;
    }

    return r;
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
int nbLiaisons(Reseau *R){
    return 0;
}
int nbCommodites(Reseau *R){
    return 0;
}
