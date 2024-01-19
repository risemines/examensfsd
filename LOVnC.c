#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fonction.h"


int  main(){
  printf("\n \t \t \t \t \t\t BIENVENUE DANS NOTRE PROGRAMME") ;
   printf("\n  \t \t \t \t  la cle doit etre un entier positif sur 5 positions ") ;
  LOV *f;
  int choix;
  int cle, trouve, i, j;
  char nom[25];
  printf("entrer un nom pr votre fichier");
  scanf("%s", &nom);
  printf("entrez un chiffre:/n");
  printf("1. creation/n 2.insertion/n 3.recherche/n 4.suppression logique/n");
  scanf("%d", &choix);
  switch(choix){
    case 1:
       creation(&f);
      break;
    case 2:
    case 3:
       printf("entrer la cle de la recherche, un identifiant entre 0000 et 9999:\n");
       scanf("%d", &cle);
       ouvrir(&f, nom, 'A');
       recherche(f, cle, &trouve, &i, &j);
       fermer(f);
      break;
    case 4:
       ouvrir(&f, nom, 'a');
       suppression_logique(f, cle);
       fermer(f);
      break;
  }
}




/*
void affecterEntete(LOV *f,int i , int x);
int EnTete(LOV *f, int i);
void lireBloc( LOV *f, int i , Buffer *buf);
void ecrireBloc(LOV *f, int i, Buffer *buf);
LOV  ouvrir(LOV **f, char nom[]);
void fermer(LOV *f);
void allouerBloc(LOV *f);
void turn_to_string(char chaine[], int n, int longueur);
void recuperer_chaine(LOV *f,int n , int *i, int *j, char chaine[],Buffer *buf);
void ecrire_chaine(LOV *f,int n , int *i, int *j, char chaine[],int *cpt,Buffer *buf);
void recherche(LOV *f,int cle,int *trouv,int *i, int *j );
void insertion(LOV *f, int cle, char *info) ;
void suppression_logique(LOV *f, int cle);
void afficher_bloc(LOV *f,int i);
void afficher_f(LOV *f);
void recherche_min(LOV *f,int MIN, int *i, int *j,int *val);
int max_f(LOV *f);
void reordonner_f(LOV *f1,char *nom);
void creation_f(LOV *f,int n);
void afficher_entete(LOV *f);
 */