#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OCTET_LIGNE 256

Chaines* lectureChaines(FILE *f) {
    if(f==NULL){
        fprintf(stderr,"Erreur lors de l'ouverture du fichier dans lectureChaines\n");
    }
    Chaines *ensembleChaines = NULL;
    int nbChaines, gamma;
    
    // Lecture du nombre de chaînes et de gamma
    fscanf(f, "NbChain: %d\nGamma: %d\n", &nbChaines, &gamma);

    // Allocation de la mémoire pour l'ensemble des chaînes
    ensembleChaines = (Chaines*)malloc(sizeof(Chaines));
    if(ensembleChaines==NULL){
        fprintf(stderr,"Erreur lors de l'allocation mémoire de Chaines dans lectureChaine\n");
    }
    ensembleChaines->gamma = gamma;
    ensembleChaines->nbChaines = nbChaines;
    ensembleChaines->chaines = NULL;

    // Lecture des chaînes
    for (int i = 0; i < nbChaines; i++) {
        CellChaine *chaine = (CellChaine*)malloc(sizeof(CellChaine));
        if(ensembleChaines==NULL){
            fprintf(stderr,"Erreur lors de l'allocation mémoire de CellCHaine dans lectureChaine\n");
        }
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
            if(ensembleChaines==NULL){
                fprintf(stderr,"Erreur lors de l'allocation mémoire de point dans lectureChaine\n");
            }
            fscanf(f, "%lf %lf", &(point->x), &(point->y));
            point->suiv = chaine->points;
            chaine->points = point;        }
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
        fprintf(stderr,"Erreur lors de l'ouverture du fichier pour l'écriture\n");
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
    //int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double distance_euclidienne(double xa,double xb,double ya,double yb){
    double x=pow((xb-xa),2);
    double y=pow((yb-ya),2);
    return sqrt(x+y);
}

double longueurChaine(CellChaine *c){
    if(c==NULL){
        fprintf(stderr, "Erreur: Chaîne mal définie dans longueurChaine\n");        return 0.0;
    }
    double somme=0.0;
    CellPoint *cp=c->points;
    //On parcourt les points d'une chaîne
    while(cp && cp->suiv){
        somme+=distance_euclidienne(cp->x,cp->suiv->x,cp->y,cp->suiv->y);
        cp=cp->suiv;
    }
    return somme;
}

double longueurTotale(Chaines *C){
    if(C==NULL){
        fprintf(stderr, "Erreur: Structure Chaines mal définie dans longueurTotale\n");        return 0.0;
        return 0.0;
    }
    double somme=0.0;
    //On parcourt les chaines
    CellChaine *cc=C->chaines;
    while(cc!=NULL){
        somme+=longueurChaine(cc);
        cc=cc->suiv;
    }
    return somme;
}

int comptePointsTotal(Chaines *C){
    if(C==NULL){
        fprintf(stderr, "Erreur: Structure Chaines mal définie dans comptePointsTotal\n");        return 0.0;
        return -1;
    }
    int i=0;
    //On parcourt les chaines
    CellChaine* cc=C->chaines;
    while(cc!=NULL){
        CellPoint* cp=cc->points;
        i+=nb_points(cp);
        cc=cc->suiv;
    }
    return i;
}

void liberer_les_chaines(Chaines *C){
        if(C==NULL) return;
        CellChaine *cc=C->chaines;
        CellChaine *cc_temp=NULL;
        //On parcourt les chaines
        while(cc){
            cc_temp=cc;
            CellPoint *cp=cc->points;
            CellPoint *cp_temp=NULL;
            while(cp){
                cp_temp = cp;
                cp = cp->suiv;
                free(cp_temp);
            }
            cc=cc_temp->suiv;
            free(cc_temp);
        }
    free(C);
}