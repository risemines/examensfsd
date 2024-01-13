#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"

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
  FILE *fichier;
  Entete entete;
}LOV;

void ouvrir (LOV **f, char nom[25]){ //liste chainee
 *f=malloc(sizeof(LOV)); //*f pointe sur la structure LOV
 (*f)->fichier = fopen(nom, "wb+"); //creer un nouveau f
 if((*f)->fichier != NULL){ 
  (*f)->entete.insert=0; // initialiser l'entete
  (*f)->entete.nbloc=1;
  (*f)->entete.sup=0;
  (*f)->entete.premier=1;

 }

}

void fermer(LOV *f){
  rewind(f->fichier); //reinitialiser la position du curseur au debut de f 
  fwrite(&(f->entete),sizeof(Entete),1,f->fichier); //ecrire l'entete
  fclose(f->fichier); //fermer le f
  free(f); //liberer lespace memoire alloue
}

void lireBloc(LOV *f, int i, Buffer *buf){  //lire un bloc
 fseek(f->fichier,(sizeof(Entete)+sizeof(Tbloc)*(i-1)),SEEK_SET); // positionnement au debut du bloc numero i
 fread(buf,sizeof(Buffer),1,f->fichier);                         //lecture d'un seul bloc de caractère correspondant a la taille du bloc dans le buffer
 rewind(f->fichier);                                            // repositionnement au debut du fichier
}
 
void ecrireBloc(LOV *f,int i, Buffer *buf){  //ecrire un bloc
  fseek(f->fichier, sizeof(Entete) + sizeof(Tbloc)*(i-1), SEEK_SET); // positionnement au debut du bloc numero i
  fwrite(buf, sizeof(struct Tbloc), 1, f);  //ecrire un seul bloc de caractère correspondant a la taille du bloc dans le buffer
}

int EnTete(LOV *f, int i){
  Entete entete;
  fread(&entete, sizeof(struct Entete),1,f); //lecture de l'en tete
  switch(i){
   case 1: return f->entete.premier;
   break;
   case 2: return f->entete.insert;
   break;
   case 3: return f->entete.sup;
   break;
   case 4: return f->entete.nbloc;
   break;
  }
}

void affecterEntete(LOV *f, int i, int x){ // changer les valeurs de l'en tete
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

int allouerBloc(LOV *f) //permet dallouer un nv bloc
{
    affecterEntete(f, 4, entete(f, 4) + 1); //incremente le nb de bloc par 1
    return EnTete(f, 4); //retourne le nb de blocs
}


//----------------------- fonction de la recherche dans le fichier--------------------------------------------//

void recherche(LOV *f, int cle, bool *trouve, int *i, int *j){
 *trouve=0;
 *i= EnTete(f,1); //indice pointe sur le numero de bloc;
 *j=0;
 Buffer buf;
 lireBloc(f, *i, &buf);

}

//----------------------- fonction de suppression logique dans le fichier--------------------------------------------//
void suppression_logique(LOV *f, int cle)
{
    int i,j,trouve;
    Buffer buf;
    char *chaine=malloc(sizeof(char)*3);
    recherche(f,cle,&trouve,&i,&j); // recherche de la cle fdans le fichihre
    if(trouve==1)                                // si la cle a ete trouvee
    {
        liredir(f,i,&buf);   // lecture du bloc dans lequel on a trouvé l'info
        recuperer_chaine(f,3,&i,&j,chaine,&buf); // recuperation de la chaine correpondant a la taille de l'info
        buf.tab[j]='v';          // mise du champs efface a vrai
        ecriredir(f,i,&buf);                  // reecriture du bloc
        affectation_entete(f,4,entete(f,4)+atoi(chaine)+9); // mise a jour du nombre de caractère supprimes
        printf("\nsuppression logique reussie\n");
    }
    else
    {
        printf("\n   suppression impossible clé inexistante\n");
    }
}
