#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    Chaines *c=lectureChaines(f);

    afficheChainesSVG(c,"test");

    fclose(f);    
    return 0;
}