#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include "../header/Hachage.h"
#include "../header/ArbreQuat.h"
#include "../header/Graphe.h"
#include "../header/Struct_File.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_H 1000
int main(int argc,char ** argv){
    mkdir("Resultats",0777);
    mkdir("Resultats/affichage_web",0777);
    mkdir("Resultats/texte",0777);

    FILE *f=fopen("Ressources/05000_USA-road-d-NY.cha","r");
    Chaines *c=lectureChaines(f);
    afficheChainesSVG(c,"Resultats/affichage_web/test_chaine");

    /* ARBRE */
    FILE *fa=fopen("Resultats/texte/test_arbre.txt","w");
    Reseau *RA=reconstitueReseauArbre(c);
    ecrireReseau(RA,fa);
    afficheReseauSVG(RA,"Resultats/affichage_web/test_reseau_arbre");
    liberer_reseau(RA);
    fclose(fa);
    
    /* LISTE  */
    FILE *fl=fopen("Resultats/texte/test_liste.txt","w");
    Reseau *RL= reconstitueReseauListe(c);
    afficheReseauSVG(RL,"Resultats/affichage_web/test_reseau_liste");
    ecrireReseau(RL,fl);
    liberer_reseau(RL);
    fclose(fl);
    
    /* HACHAGE */
    FILE *fh=fopen("Resultats/texte/test_hachage.txt","w");
    Reseau *RH=reconstitueReseauHachage(c,TAILLE_H);
    afficheReseauSVG(RH,"Resultats/affichage_web/test_reseau_hachage");
    ecrireReseau(RH,fh);
    liberer_reseau(RH);
    fclose(fh);

    liberer_les_chaines(c);
    fclose(f);
    return 0;
}