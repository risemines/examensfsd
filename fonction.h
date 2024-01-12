#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonction.h"




//-------------------------aff_entete----------------------------//
void aff_entete(LOV *fichier,int i , int valeur)
{

    switch(i)
    {
        case 1:
        {
            fichier->entete.nbbloc=valeur; // nombre total de bloc alloués dans le fichier
        }break;
         case 2:
        {
            fichier->entete.tete=valeur;  // numero du bloc representatnt la tete du fichier
        }break;
         case 3:
        {
            fichier->entete.queue=valeur;  // numero du bloc representatnt la tete duf icheir
        }break;
         
         case 4:
        {
            fichier->entete.nb_car_sup=valeur;  // nombre de caractères suprimé depuis la
                                                  //création du ficher afin de lancer la réorganiosation
        }break;

    };
}

//-------------------------------------permet d'obtenir les elements de l'entete du ficher------------------------------//
int entete(LOV *fichier, int i)
{

    switch(i)
    {
        case 1:
        {
            return(fichier->entete.nbbloc);
        }break;
         case 2:
        {
            return(fichier->entete.tete);
        }break;
         case 3:
        {
            return(fichier->entete.queue);
        }break;
        
         case 4:
        {
            return(fichier->entete.nb_car_sup);
        }break;

    };
}
//----------------------liredir-------------------//
void liredir(LOV *fichier, int i , Buffer *buf)
{

 fseek(fichier->fich,(sizeof(Entete)+sizeof(Tbloc)*(i-1)),SEEK_SET); // positionnement au debut du bloc numero i
 fread(buf,sizeof(Buffer),1,fichier->fich);                         //lecture d'un bloc de caractère correspondant a la taille du bloc dans le buffer
 rewind(fichier->fich);                                            // repositionnement endebut de fichier

}
//-------------------------ecriredir------------------------------------------//
void ecriredir(LOV *fichier, int i, Buffer *buf)
{

     fseek(fichier->fich,sizeof(Entete)+sizeof(Tbloc)*(i-1),SEEK_SET); // positionnement au debut du bloc numero i
     fwrite(buf,sizeof(Buffer),1,fichier->fich);                       //ecriture du contenu du buffer dans le bloc numero i du fichier

}







//----------------------- focntion de suppression lohgique dans le fichier--------------------------------------------//
void suppression_logique_LOV(LOV *fichier, int cle)
{
    int i,j,trouv;
    Buffer buf;
    char *chaine=malloc(sizeof(char)*3);
    recherche_LOV(fichier,cle,&trouv,&i,&j); // recherche de la cle fdans le fichihre
    if(trouv==1)                                // si la cmle a etet trouvee
    {
        liredir(fichier,i,&buf);   // lecture du bloc dans lequel on a trouvé l'info
        recuperer_chaine(fichier,3,&i,&j,chaine,&buf); // recuperation de la chaine correpondant a la taille de l'info
        buf.tab[j]='v';          // mise du champs efface a vrai
        ecriredir(fichier,i,&buf);                  // reecriture du bloc
        aff_entete(fichier,5,entete(fichier,5)+atoi(chaine)+9); // mise az jour du nombre de caractère supprimes
        printf("\nsuppression logique reussie\n");
    }
    else
    {
        printf("\n   suppression impossible clé inexistante\n");
    }
}
void manuel()
 {
   puts("\n \t \t \t \t \t\t BIENVENUE DANS NOTRE PROGRAMME") ;
   puts("\n  \t \t \t \t  la cle doit etre un entier positif sur 5 positions ") ;
   puts("\n\t \t \t \t vous devez creer votre fichier avant de commencer le traitement ") ;
   puts("\n \t \t \t \t on vous propose 2 types de suppression physique :") ;
   puts("\n \t \t \t \t 1- suppression physique d'un seul element celle ci generera un autre fichier") ;
   puts("\n \t \t \t \t 2- supprssion physique de tous les elements supprimes logiquement") ;
   puts("\n\t \t \t \t nous vous recomondons la seconde methode pour diminuer le nombre d'accee au disque") ;
}