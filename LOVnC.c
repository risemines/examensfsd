#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define false 
#define true 1


typedef struct Tbloc{
  char tab[100];
  int svt;
  int prd;
}Tbloc;

typedef Tbloc Buffer;

typedef struct Etudiant{
  char nom[25];
  char prenom[25];
  char key[4]; //pr la recherche
  char taille[4];
}Etudiant;

typedef struct Entete{
  int premier; //premier bloc
  int insert; // nb de caracteres inseres
  int sup; // nb de caractere supprimes
  int nbloc; // nb de blocs
}Entete;

typedef struct LOV{
  FILE *fichier;
  Entete entete;
}LOV;


void lire(FILE *fichier, int i, struct fbloc *buffer){
  fread(buffer,sizeof(struct Tbloc),1,fichier);
}

void ecrire(FILE *fichier,int i, struct fbloc *buffer){
  fwrite(buffer, sizeof(struct Tbloc), 1, fichier);
}

int recupentete(FILE *fichier, int i){
  struct Entete entete;
  fread(&entete, sizeof(struct Entete),1,fichier);
  switch(i){
   case 1: return entete.premier;
   break;
   case 2: return entete.insert;
   break;
   case 3: return entete.sup;
   break;
   case 4: return entete.nbloc;
   break;
  }
}

int  main(){
  FILE *fichier;
fichier = fopen("","");

if (fichier == NULL){
printf("erreur");
return 1;
}
fclose(fichier);

}