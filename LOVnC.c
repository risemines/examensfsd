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

void ouvrir (LOV **f, char nom[25]){ //liste chainee
 *f=malloc(sizeof(LOV)); //*f pointe sur la structure LOV
 (*f)->fichier = fopen(nom, wb+); //creer un fichier
 if((*f)->fichier != NULL){
  (*f)->entete.insert=0;
  (*f)->entete.nbloc=1;
  (*f)->entete.sup=0;
  (*f)->entete.premier=1;

 }

}

void fermer(LOV *f){
  rewind(f->fichier); //reinitialiser la position du curseur au debut de fichier 
  fwrite(&(f->entete),sizeof(Entete),1,f->fichier); //ecrire l'entete
  fclose(f->fichier); //fermer le fichier
  free(f); //liberer lespace memoire alloue
}

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

void affectation_entete(LOV *f, int i, int x){
  switch(i){
   case 1: f->entete.premier=x;
   break;
   case 2: f->entete.insert=x;
   break;
   case 3: f->entete.sup=x;
   break;
   case 4: f->entete.nbloc=x;
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