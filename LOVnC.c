#include <stdio.h>
#include <stdlib.h>

typedef struct fbloc{
  char tab[100];
  int key;
  int svt;
};
int recherche(FILE *fichier, char nom){
  struct fbloc bloc;
  int i=0;
  while (fread(&bloc, sizeof(struct fbloc),1,fichier) == 1){
    i++;
    if(strncmp(bloc.tab[i], nom, strlen(nom))){
      printf("bloc trouve: %s avec sa cle %d ",bloc.tab[i], bloc.key);
      return 1;
    }
  }
  printf("bloc non trouve");
  return 0;
}  
int  main(){
  FILE *fichier;
  struct bloc buffer;
fichier = fopen("","");

if (fichier == NULL){
printf("erreur");
return 1;
}
}