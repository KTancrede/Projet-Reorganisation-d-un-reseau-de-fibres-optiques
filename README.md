# Projet : Réorganisation d’un réseau de fibres optiques
## Réalisé par Tancrède KOWAL (28606347) et Nassim

Le projet se présente sous la forme suivante. Les fichiers compilés sont dans le fichier ```/build/``` qui s'obtient après avoir fait ```make```. Les fichiers ```.h``` sont dans header et les ```.c``` dans src. 

Nous avons, afin d'organiser au mieux le projet, crée ```/affichage_web/``` et ```/texte/``` dans lesquelles nous retrouvons les fichier ```.html``` et ```.txt``` générés au fur et à mesure du projet.
La commande ```make clean``` permet d'effacer tous les exécutables ainsi que les ```.html``` et ```.txt```

```
.
├── affichage_web
│   ├── test_chaine.html
│   └── test_reseau.html
├── build
│   ├── ChaineMain.o
│   ├── Chaine.o
│   ├── Reconstitue_Reseau.o
│   ├── Reseau.o
│   └── SVGwriter.o
├── chaine_main
├── header
│   ├── ArbreQuat.h
│   ├── Chaine.h
│   ├── Hachage.h
│   ├── Reseau.h
│   └── SVGwriter.h
├── Makefile
├── README.md
├── reconstitue_Reseau
├── Ressources
│   ├── 00014_burma.cha
│   ├── 00014_burma.res
│   ├── affichageChaines.txt
│   ├── affichageReseau.txt
│   ├── enonceV1.pdf
│   └── Schéma Chaine Réseau .pdf
├── src
│   ├── Chaine.c
│   ├── ChaineMain.c
│   ├── Reconstitue_Reseau.c
│   ├── Reseau.c
│   └── SVGwriter.c
└── texte
    └── test_reseau.txt
```