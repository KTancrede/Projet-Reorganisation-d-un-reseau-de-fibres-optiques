#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include "../header/Hachage.h"
#include "../header/ArbreQuat.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    //FILE *f=fopen("Ressources/TESTS/USA-road-d.NY.co","r");
    Chaines *c=lectureChaines(f);
    
    Reseau *RH=reconstitueReseauHachage(c,M);
    Reseau *RL= reconstitueReseauListe(c);
    Reseau *RA=reconstitueReseauArbre(c);

    FILE *fa=fopen("texte/test_arbre.txt","w");
    ecrireReseau(RA,fa);

    afficheChainesSVG(c,"affichage_web/test_chaine");
    afficheReseauSVG(RL,"affichage_web/test_reseau_liste");
    afficheReseauSVG(RH,"affichage_web/test_reseau_hachage");
    afficheReseauSVG(RA,"affichage_web/test_arbre");
   

    liberer_reseau(RH);
    liberer_reseau(RL);
    //liberer_reseau(RA);
    liberer_les_chaines(c);


    fclose(fa);
    fclose(f);
}