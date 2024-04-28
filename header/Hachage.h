#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include "Reseau.h"
#include "Chaine.h"


typedef struct{
  int nbElement; 
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

int fonction_hach(int k,int M);
Noeud *rechercheCreeNoeudHachage(Reseau *R,TableHachage *TH,double x,double y);
Reseau* reconstitueReseauHachage(Chaines *C, int m);
void ajouterVoisins_H(Noeud *n1, Noeud *n2);
void afficher_table(TableHachage *TH);
void libererTableHachage(TableHachage* table) ;
#endif