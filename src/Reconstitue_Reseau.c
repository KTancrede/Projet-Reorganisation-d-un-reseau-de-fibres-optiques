#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include "../header/Hachage.h"
#include "../header/ArbreQuat.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char ** argv){
    mkdir("affichage_web",0777);
    mkdir("texte",0777);

    FILE *f=fopen("Ressources/00014_burma.cha","r");
    Chaines *c=lectureChaines(f);
    afficheChainesSVG(c,"affichage_web/test_chaine");

    /* ARBRE
    FILE *fa=fopen("texte/test_arbre.txt","w");
    Reseau *RA=reconstitueReseauArbre(c);
    afficheReseauSVG(RH,"affichage_web/test_reseau_arbre");
    ecrireReseau(RA,fa);
    liberer_reseau(RA);
    fclose(fa);
    */

    /* LISTE */
    FILE *fl=fopen("texte/test_liste.txt","w");
    Reseau *RL= reconstitueReseauListe(c);
    afficheReseauSVG(RL,"affichage_web/test_reseau_liste");
    ecrireReseau(RL,fl);
    liberer_reseau(RL);
    fclose(fl);
    

    /* HACHAGE */
    FILE *fh=fopen("texte/test_hachage.txt","w");
    Reseau *RH=reconstitueReseauHachage(c,M);
    afficheReseauSVG(RH,"affichage_web/test_reseau_hachage");
    ecrireReseau(RH,fh);
    liberer_reseau(RH);
    fclose(fh);
   
    liberer_les_chaines(c);
    fclose(f);
}