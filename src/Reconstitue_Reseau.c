#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    Chaines *c=lectureChaines(f);
    Reseau *r= reconstitueReseauListe(c);
    
    FILE *ff=fopen("texte/test_reseau.txt","w");
    ecrireReseau(r,ff);
    afficheReseauSVG(r,"affichage_web/test_reseau");
}