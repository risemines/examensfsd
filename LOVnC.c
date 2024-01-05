#include <stdio.h>
#include <stdlib.h>

typedef struct fbloc{
  char tab[100];
  int key;
  int svt;
};

typedef struct Entete{
  int nb_bloc;
  int compteur;
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


void lire(FILE *fichier, int i, struct fbloc *buffer){
  fread(buffer,sizeof(struct fbloc),1,fichier);
}

void ecrire(FILE *fichier,int i, struct fbloc *buffer){
  fwrite(buffer, sizeof(struct fbloc), 1, fichier);
}

int recupentete(FILE *fichier, int i){
  struct Entete entete;
  fread(&entete, sizeof(struct Entete),1,fichier);
  if(i==1){
    return entete.nb_bloc;
  }
  else if(i==2){
    return entete.compteur;
  }
  else{
printf("chiffre invalide");
return -1;}
}

int  main(){
  FILE *fichier;
  struct fbloc buffer;
fichier = fopen("","");

if (fichier == NULL){
printf("erreur");
return 1;
}
fclose(fichier);

}