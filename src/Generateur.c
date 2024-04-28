#include <stdlib.h>
#include <time.h>

#include "../header/Chaine.h"

//Fonction qui permet la génération d'une Chaîne en fonction de divers paramètres
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    //Pour l'aléatoire
    srand(time(NULL)); 

    //On instancie notre Chaines
    Chaines* chaines = malloc(sizeof(Chaines));
    chaines->gamma = nbPointsChaine;
    chaines->nbChaines = nbChaines;
    chaines->chaines = NULL;
    //On parcourt les différentes chaines
    for (int i = 0; i < nbChaines; i++) {
        CellChaine* chaine = malloc(sizeof(CellChaine));
        chaine->numero = i;
        chaine->points = NULL;
        chaine->suiv = chaines->chaines;
        chaines->chaines = chaine;
        //On y crée des points
        for (int j = 0; j < nbPointsChaine; j++) {
            CellPoint* point = malloc(sizeof(CellPoint));
            point->x = (double)rand() / RAND_MAX * xmax;
            point->y = (double)rand() / RAND_MAX * ymax;
            point->suiv = chaine->points;
            chaine->points = point;
        }
    }

    return chaines;
}
