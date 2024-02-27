#include "../header/Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OCTET_LIGNE 256

Chaines* lectureChaines(FILE* f) {
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la lecture\n");
        return NULL;
    }

    Chaines* chaines = (Chaines*)malloc(sizeof(Chaines));
    fscanf(f, "%d %d", &chaines->nbChaines, &chaines->gamma);

    char ligne[OCTET_LIGNE];
    while (fgets(ligne, OCTET_LIGNE, f) != NULL) {
        CellChaine* cellchaine = (CellChaine*)malloc(sizeof(CellChaine));
        int nb_points;
        sscanf(ligne, "%d %d", &cellchaine->numero, &nb_points);
        
        cellchaine->points = NULL;

        for (int i = 0; i < nb_points; i++) {
            CellPoint* cellpoint = (CellPoint*)malloc(sizeof(CellPoint));
            fscanf(f, "%lf %lf", &cellpoint->x, &cellpoint->y);

            // Ajoutez le point à la liste des points de la chaine
            cellpoint->suiv = cellchaine->points;
            cellchaine->points = cellpoint;
        }

        // Ajoutez la chaine à la liste des chaines
        cellchaine->suiv = chaines->chaines;
        chaines->chaines = cellchaine;
    }
    return chaines;
}
int nb_points(CellPoint *c){
    int compteur=0;
    while(c!=NULL){
        compteur++;
        c=c->suiv;
    }
    return compteur;
}
void ecrireChaines(Chaines *C, FILE *f){
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour l'écriture\n");
        return;
    }
    if(C==NULL)
        return;
        
    fprintf(f,"NbChain: %d\nGamma: %d\n",C->nbChaines,C->gamma);
    while(C->chaines!=NULL){
        int nb_points_chaine=nb_points(C->chaines->points);
        fprintf(f,"%d %d ",C->chaines->numero,nb_points_chaine);
        for(int i=0;i<nb_points_chaine;i++){
            CellPoint *cp=C->chaines->points;
            fprintf(f,"%lf %lf ",cp->x,cp->y);
            cp=cp->suiv;
        }
        fprintf(f,"\n");
    }
}
/*
void afficheChainesSVG(Chaines *C, char* nomInstance){

}
double longueurTotale(Chaines *C){

}
int comptePointsTotal(Chaines *C){

}
*/