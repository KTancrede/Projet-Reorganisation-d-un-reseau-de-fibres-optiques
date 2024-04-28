#include "../header/Chaine.h"
#include "../header/SVGwriter.h"
#include "../header/Reseau.h"
#include "../header/Hachage.h"
#include "../header/ArbreQuat.h"
#include "../header/Generateur.h" 

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NB_CHAINES_MIN 500
#define NB_CHAINES_MAX 5000
#define PAS_CHAINES 500
#define NB_POINTS_CHAINE 20
#define X_MAX 5000
#define Y_MAX 5000
#define TAILLE_TABLE 1000

// Structure pour stocker les temps de calcul
typedef struct {
    int nbPointsTotal;
    double tempsListe;
    double tempsHachage;
    double tempsArbre;
} TempsCalcul;

int main(int argc, char **argv) {
    // Création du fichier CSV
    FILE *csv = fopen("Resultats/temps_execution.csv", "w");
    if (csv == NULL) {
        perror("Erreur lors de la création du fichier CSV");
        return EXIT_FAILURE;
    }
    fprintf(csv, "NbPointsTotal,TempsListe,TempsHachage,TempsArbre\n");
    
    // Boucle pour faire varier le nombre de chaînes
    for (int nbChaines = NB_CHAINES_MIN; nbChaines <= NB_CHAINES_MAX; nbChaines += PAS_CHAINES) {
        // Génération aléatoire des chaînes
        Chaines *chainesAleatoires = generationAleatoire(nbChaines, NB_POINTS_CHAINE, X_MAX, Y_MAX);
        
        // Calcul du nombre total de points dans les chaînes générées
        int nbPointsTotal = nbChaines * NB_POINTS_CHAINE;

        // Chronométrage et exécution des fonctions
        clock_t debut, fin;

        
        // Chronométrage pour la liste chaînée
        debut = clock();
        Reseau *RL = reconstitueReseauListe(chainesAleatoires);
        fin = clock();
        double tempsListe = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_reseau(RL);
        
       
        // Chronométrage pour la table de hachage
        debut = clock();
        Reseau *RH = reconstitueReseauHachage(chainesAleatoires,TAILLE_TABLE);
        fin = clock();
        double tempsHachage = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_reseau(RH);
        
        // Chronométrage pour l'arbre quaternaire
        debut = clock();
        Reseau *RA = reconstitueReseauArbre(chainesAleatoires);
        fin = clock();
        double tempsArbre = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        liberer_reseau(RA);

        // Enregistrement des temps de calcul dans le fichier CSV
        fprintf(csv, "%d, %lf, %lf, %lf\n", nbPointsTotal, tempsListe, tempsHachage, tempsArbre);

        // Libération de la mémoire des chaînes générées
        liberer_les_chaines(chainesAleatoires);
    }

    fclose(csv);
    return 0;
}
