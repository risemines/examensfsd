#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int  main(){
  puts("\n \t \t \t \t \t\t BIENVENUE DANS NOTRE PROGRAMME") ;
   puts("\n  \t \t \t \t  la cle doit etre un entier positif sur 5 positions ") ;
   puts("\n\t \t \t \t vous devez creer votre fichier avant de commencer le traitement ") ;
   puts("\n \t \t \t \t on vous propose 2 types de suppression physique :") ;
   puts("\n \t \t \t \t 1- suppression physique d'un seul element celle ci generera un autre fichier") ;
   puts("\n \t \t \t \t 2- supprssion physique de tous les elements supprimes logiquement") ;
   puts("\n\t \t \t \t nous vous recomondons la seconde methode pour diminuer le nombre d'accee au disque") ;
  LOV *f;
  char nom[25];
  printf("entrer un nom pr votre fichier");
  scanf("%s", &nom);
  ouvrir (f, nom);

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
