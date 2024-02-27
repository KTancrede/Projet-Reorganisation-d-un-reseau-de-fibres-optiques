#include "../header/Reseau.h"
#include "../header/Chaine.h"
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
    if(c_n==NULL){
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
Reseau* reconstitueReseauListe(Chaines *C){
    return NULL;
}
void ecrireReseau(Reseau *R, FILE *f){

}
int nbLiaisons(Reseau *R){
    return 0;
}
int nbCommodites(Reseau *R){
    return 0;
}
void afficheReseauSVG(Reseau *R, char* nomInstance){

}