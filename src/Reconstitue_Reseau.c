#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include "../header/Hachage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    
    Chaines *c=lectureChaines(f);

    Reseau *RH=reconstitueReseauHachage(c,42);
    Reseau *RL= reconstitueReseauListe(c);
    
    FILE *ff=fopen("texte/test_reseau.txt","w");
    ecrireReseau(RL,ff);

    afficheChainesSVG(c,"affichage_web/test_chaine");
    afficheReseauSVG(RL,"affichage_web/test_reseau_liste");
    afficheReseauSVG(RH,"affichage_web/test_reseau_hachage");

    liberer_reseau(RH);
    liberer_reseau(RL);
    liberer_les_chaines(c);

    fclose(ff);
    fclose(f);
}