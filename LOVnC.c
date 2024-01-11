#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fonction.h"
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




//*************************************************************************************************************************//
void aff_entete(LOV *fichier,int i , int valeur);
int entete(LOV *fichier, int i);
void liredir( LOV *fichier, int i , Buffer *buf);
void ecriredir(LOV *fichier, int i, Buffer *buf);
LOV *ouvrir(char nom_physique[], char mode);
void fermer(LOV *fichier);
void alloc_bloc(LOV *fichier);
void turn_to_string(char chaine[], int n, int longueur);
void recuperer_chaine(LOV *fichier,int n , int *i, int *j, char chaine[],Buffer *buf);
void ecrire_chaine(LOV *fichier,int n , int *i, int *j, char chaine[],int *cpt,Buffer *buf);
void recherche_LOV(LOV *fichier,int cle,int *trouv,int *i, int *j );
void insertion_LOV(LOV *fichier, int cle, char *info) ;
void suppression_logique_LOV(LOV *fichier, int cle);
void afficher_bloc(LOV *fichier,int i);
void afficher_fichier(LOV *fichier);
void afficher_info_chevau(LOV *fichier);
void recherche_min(LOV *fichier,int MIN, int *i, int *j,int *val);
int max_fichier(LOV *fichier);
void reordonner_fichier(LOV *fichier1,char *nom_physique);
void suppression_physique_LOV(LOV *fichier,char *nom_physique);
void creation_fichier(LOV *fichier,int n);
void afficher_entete(LOV *fichier);
void cadre();
void choice();
void manuel();