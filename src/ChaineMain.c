#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    FILE *ff=fopen("texte/test_chaine.txt","w");

    Chaines *c=lectureChaines(f);
    printf("distance totale: %lf\nnombre de points: %d\n\n",longueurTotale(c),comptePointsTotal(c));
    
    afficheChainesSVG(c,"affichage_web/test_chaine");
    ecrireChaines(c,ff);
    
    liberer_les_chaines(c);
    fclose(ff);
    fclose(f);    
    return 0;
}