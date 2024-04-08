#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../header/Reseau.h"
#include "../header/Hachage.h"
#include "../header/Chaine.h"

int M=150;

int fonction_hach(int k){
    float A=(sqrt(5)-1)/2;
    int H=floor(M*(k*A-floor(k*A)));
    return H;
}

TableHachage *creationTable(int taille){
    TableHachage *TH = (TableHachage*)malloc(sizeof(TableHachage));
    if (TH == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour la table de hachage");
        return NULL;
    }

    TH->tailleMax = taille;
    TH->T = (CellNoeud**)malloc(sizeof(CellNoeud*) * taille);
    if (TH->T == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour le tableau de hachage");
        free(TH); // Libérer la mémoire allouée pour TH
        return NULL;
    }

    for(int i = 0; i < TH->tailleMax; i++){
        TH->T[i] = NULL;
    }
    
    return TH;
}



Noeud *rechercheCreeNoeudHachage(Reseau *R,TableHachage *TH,double x,double y){
    if (R == NULL) {
        perror("R non définie dans rechercheCreeNoeudHachage");
        return NULL;
    }
    if (TH == NULL) {
        perror("TH non définie dans rechercheCreeNoeudHachage");
        return NULL;
    }
    //On cherche dans la table s'il y a un noeud de ces coordonnes
    int clef_noeud=fonction_hach(floor(x+y));
    CellNoeud* liste_chainee_clef=TH->T[clef_noeud];
    while(liste_chainee_clef!=NULL){
        Noeud *noeud=liste_chainee_clef->nd;
        if(noeud->x==x && noeud->y){
            return noeud;
        }
        liste_chainee_clef=liste_chainee_clef->suiv;
    }
    //SINON on cree un noeud
    Noeud *noeud_a_inserer=rechercheCreeNoeudListe(R,x,y);
    if (noeud_a_inserer != NULL) {
        CellNoeud *nouvelle_cellule = (CellNoeud *)malloc(sizeof(CellNoeud));
        if (nouvelle_cellule == NULL) {
            perror("Erreur lors de l'allocation mémoire de la nouvelle cellule dans rechercheCreeNoeudHachage");
            return NULL;
        }
        nouvelle_cellule->nd = noeud_a_inserer;
        nouvelle_cellule->suiv = NULL;

        if (TH->T[clef_noeud] == NULL) {
            TH->T[clef_noeud] = nouvelle_cellule;
        } else {
            CellNoeud *derniere_cellule = TH->T[clef_noeud];
            while (derniere_cellule->suiv != NULL) {
                derniere_cellule = derniere_cellule->suiv;
            }
            derniere_cellule->suiv = nouvelle_cellule;
        }
        return noeud_a_inserer;
    } else {
        fprintf(stderr, "Erreur lors de la création du nœud dans rechercheCreeNoeudHachage");
        return NULL;
    }
}

//On veut ajouter n1 à n2
void ajouterVoisins_H(Noeud *n1, Noeud *n2) {
    if (n1 == NULL || n2 == NULL) {
        perror("Erreur dans ajouterVoisins de paramètres\n");
        return;
    }

    // Vérifier si n2 est déjà dans la liste des voisins de n1
    CellNoeud *c_n_voisins = n1->voisins;
    while (c_n_voisins != NULL) {
        if (c_n_voisins->nd == n2) {
            // Si n2 est déjà un voisin de n1, ne rien faire
            return;
        }
        c_n_voisins = c_n_voisins->suiv;
    }

    // Allocation mémoire pour la nouvelle cellule de voisin
    CellNoeud *nouveau_voisin1 = (CellNoeud*)malloc(sizeof(CellNoeud));
    CellNoeud *nouveau_voisin2 = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (nouveau_voisin1 == NULL || nouveau_voisin2 == NULL) {
        perror("Erreur lors de l'allocation mémoire de nouveau_voisin dans ajouterVoisins");
        return;
    }

    // Assigner le nœud n2 à la nouvelle cellule de voisin pour n1
    nouveau_voisin1->nd = n2;
    nouveau_voisin1->suiv = n1->voisins;
    n1->voisins = nouveau_voisin1;

    // Assigner le nœud n1 à la nouvelle cellule de voisin pour n2
    nouveau_voisin2->nd = n1;
    nouveau_voisin2->suiv = n2->voisins;
    n2->voisins = nouveau_voisin2;
}



// Cette fonction permet de reconstituer un réseau à partir des chaînes
Reseau* reconstitueReseauHachage(Chaines *C,int m) {
    // Vérification de C
    if (C == NULL) {
        perror("Erreur de C dans reconstitutionReseauListe");
        return NULL;
    }

    // Création du réseau
    Reseau *r = (Reseau*)malloc(sizeof(Reseau));
    if (r == NULL) {
        perror("Erreur lors de l'allocation mémoire de r dans reconstitutionReseauListe");
        return NULL;
    }

    // Initialisation des attributs du réseau
    r->noeuds = NULL;
    r->commodites = NULL;
    r->nbNoeuds = 0;
    r->gamma = C->gamma;

    TableHachage *TH=creationTable(m);

    // Parcours des chaînes
    CellChaine *cellChaines = C->chaines;
    while (cellChaines != NULL) {
        CellCommodite *ccom = NULL;
        Noeud *premierNoeud = NULL;  // Pour garder une référence au premier nœud de la chaîne

        CellPoint *cellPoint = cellChaines->points;
        Noeud *noeudPrecedent = NULL;

        while (cellPoint != NULL) {
            // Recherche ou création du nœud
            Noeud *noeud = NULL;
            
            noeud = rechercheCreeNoeudHachage(r,TH, cellPoint->x, cellPoint->y);

            // Mémoriser le premier nœud de la chaîne
            if (premierNoeud == NULL) {
                premierNoeud = noeud;
            }

            // Gestion des commodités
            if (cellPoint == cellChaines->points) {
                // Si c'est le premier point de la chaîne, créer une nouvelle commodité
                ccom = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (ccom == NULL) {
                    perror("Erreur lors de l'allocation mémoire de ccom dans reconstitutionReseauListe");
                    return NULL;
                }
                ccom->extrA = noeud;
                ccom->extrB = NULL;

                // Gestion de la liste chaînée des commodités
                ccom->suiv = r->commodites;
                r->commodites = ccom;
            } else if (cellPoint->suiv == NULL) {
                // Si c'est le dernier point de la chaîne, mettre à jour la deuxième extrémité de la commodité
                if (ccom != NULL) {
                    ccom->extrB = noeud;
                }
            }

            // Ajout du nœud actuel à la liste des voisins du nœud précédent (si ce n'est pas le premier nœud)
            if (noeudPrecedent != NULL) {
                ajouterVoisins(noeudPrecedent, noeud);
            }

            noeudPrecedent = noeud;
            cellPoint = cellPoint->suiv;
        }
        cellChaines = cellChaines->suiv;
    }
    
    //afficher_table(TH);
    libererTableHachage(TH);
    return r;
}

void afficher_table(TableHachage *TH){
    if(TH==NULL){
        perror("TableHachage non définie dans afficher_table\n");
    }
    for(int i=0;i<TH->tailleMax;i++){
        printf("Index %d: ",i);
        CellNoeud *cour=TH->T[i];
        while(cour){
            printf("%d ",cour->nd->num);
            cour=cour->suiv;
        }
        printf("\n");
    }
}

void libererTableHachage(TableHachage *table) {
    if (table == NULL) {
        return;
    }

    // Libérer chaque cellule de la table
    for (int i = 0; i < table->tailleMax; ++i) {
        CellNoeud *currentCell = table->T[i];
        while (currentCell != NULL) {
            CellNoeud *temp = currentCell;
            currentCell = currentCell->suiv;
            free(temp); // Libérer la cellule
        }
    }

    // Libérer le tableau lui-même
    free(table->T);
    
    // Libérer la structure de la table
    free(table);
}

