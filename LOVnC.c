#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define false 0
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
  char cle[4]; //pr la recherche
  char taille[4];
}Etudiant;

typedef struct Entete{
  int premier; //premier bloc
  int insert; // nb de caracteres inseres
  int sup; // nb de caractere supprimes
  int nbloc; // nb de blocs
}Entete;

typedef struct LOV{
  FILE *f;
  Entete entete;
}LOV;

void ouvrir (LOV **f, char nom[25]){ //liste chainee
 *f=malloc(sizeof(LOV)); //*f pointe sur la structure LOV
 (*f)->f = fopen(nom, wb+); //creer un f
 if((*f)->f != NULL){
  (*f)->entete.insert=0;
  (*f)->entete.nbloc=1;
  (*f)->entete.sup=0;
  (*f)->entete.premier=1;

 }

}

void fermer(LOV *f){
  rewind(f->f); //reinitialiser la position du curseur au debut de f 
  fwrite(&(f->entete),sizeof(Entete),1,f->f); //ecrire l'entete
  fclose(f->f); //fermer le f
  free(f); //liberer lespace memoire alloue
}

void liredir(FILE *f, int i, struct fbloc *buffer){
  fread(buffer,sizeof(struct Tbloc),1,f);
}
 
void ecriredir(FILE *f,int i, struct fbloc *buffer){
  fwrite(buffer, sizeof(struct Tbloc), 1, f);
}

int recupentete(FILE *f, int i){
  struct Entete entete;
  fread(&entete, sizeof(struct Entete),1,f);
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
  FILE *f;
f = fopen("","");

if (f == NULL){
printf("erreur");
return 1;
}
fclose(f);

}




/*
void affectation_entete(LOV *f,int i , int x);
int recupentete(LOV *f, int i);
void liredir( LOV *f, int i , Buffer *buf);
void ecriredir(LOV *f, int i, Buffer *buf);
LOV  ouvrir(LOV **f, char nom[]);
void fermer(LOV *f);
void alloc_bloc(LOV *f);
void turn_to_string(char chaine[], int n, int longueur);
void recuperer_chaine(LOV *f,int n , int *i, int *j, char chaine[],Buffer *buf);
void ecrire_chaine(LOV *f,int n , int *i, int *j, char chaine[],int *cpt,Buffer *buf);
void recherche_LOV(LOV *f,int cle,int *trouv,int *i, int *j );
void insertion_LOV(LOV *f, int cle, char *info) ;
void suppression_logique_LOV(LOV *f, int cle);
void afficher_bloc(LOV *f,int i);
void afficher_f(LOV *f);
void afficher_info_chevau(LOV *f);
void recherche_min(LOV *f,int MIN, int *i, int *j,int *val);
int max_f(LOV *f);
void reordonner_f(LOV *f1,char *nom);
void suppression_physique_LOV(LOV *f,char *nom);
void creation_f(LOV *f,int n);
void afficher_entete(LOV *f);
void cadre();
void choice();
void manuel(); */