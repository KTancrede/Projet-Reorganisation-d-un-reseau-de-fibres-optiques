#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OCTET_LIGNE 256

Chaines* lectureChaines(FILE *f) {
    Chaines *ensembleChaines = NULL;
    int nbChaines, gamma;
    
    // Lecture du nombre de chaînes et de gamma
    fscanf(f, "NbChain: %d\nGamma: %d\n", &nbChaines, &gamma);

    // Allocation de la mémoire pour l'ensemble des chaînes
    ensembleChaines = (Chaines*)malloc(sizeof(Chaines));
    ensembleChaines->gamma = gamma;
    ensembleChaines->nbChaines = nbChaines;
    ensembleChaines->chaines = NULL;

    // Lecture des chaînes
    for (int i = 0; i < nbChaines; i++) {
        CellChaine *chaine = (CellChaine*)malloc(sizeof(CellChaine));
        chaine->suiv = ensembleChaines->chaines;
        ensembleChaines->chaines = chaine;

        chaine->points = NULL;
        
        // Lecture du numéro de la chaîne
        fscanf(f, "%d", &(chaine->numero));

        // Lecture des points de la chaîne
        int nbPoints;
        fscanf(f, "%d ", &nbPoints);
        
        for (int j = 0; j < nbPoints; j++) {
            CellPoint *point = (CellPoint*)malloc(sizeof(CellPoint));
            fscanf(f, "%lf %lf", &(point->x), &(point->y));
            point->suiv = chaine->points;
            chaine->points = point;
            //printf("x:%.2lf y:%.2lf\n",(point->x), (point->y));
        }
    }

    return ensembleChaines;
}


int nb_points(CellPoint *c){
    int compteur=0;
    while(c!=NULL){
        compteur++;
        c=c->suiv;
    }
    return compteur;
}

void ecrireChaines(Chaines* C, FILE* f) {
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour l'écriture\n");
        return;
    }
    if (C == NULL)
        return;

    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    CellChaine* cc = C->chaines;

    while (cc != NULL) {
        int nb_points_chaine = nb_points(cc->points);
        fprintf(f, "%d %d ", cc->numero, nb_points_chaine);

        CellPoint* cp = cc->points;

        while (cp != NULL) {
            fprintf(f, "%.2lf %.2lf ", cp->x, cp->y);
            cp = cp->suiv;
        }

        fprintf(f, "\n");
        cc = cc->suiv;
    }
}


void afficheChainesSVG(Chaines *C, char* nomInstance){
    SVGwriter *svg=(SVGwriter*)malloc(sizeof(SVGwriter));

    if(svg==NULL){
        return;
    }
    if(C==NULL){
        printf("Erreur de Chaines dans afficheSVG");
        return;
    }

    SVGinit(svg,nomInstance,800,600);
    SVGlineRandColor(svg);
    SVGpointColor(svg,Red);

    CellChaine *chaines=C->chaines;
    while(chaines){
        CellPoint *p=chaines->points;

        while(p!=NULL){
            if(p->suiv!=NULL){
                SVGline(svg,p->x,p->y,p->suiv->x,p->suiv->y);
            }
            SVGpoint(svg,p->x,p->y);
            p=p->suiv;
        }

        chaines=chaines->suiv;
    }
    SVGfinalize(svg);
    free(svg);
}
/*
double longueurTotale(Chaines *C){

}
int comptePointsTotal(Chaines *C){

}
*/