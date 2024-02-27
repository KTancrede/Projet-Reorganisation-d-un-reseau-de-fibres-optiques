#include "../header/Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char ** argv){
    FILE *f=fopen("Ressources/00014_burma.cha","r");
    FILE *ff=fopen("t.txt","w");
    
    Chaines *c=lectureChaines(f);

    ecrireChaines(c,ff);

    return 0;
}