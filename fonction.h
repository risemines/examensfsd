#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100 //max car dans un bloc


typedef struct Tbloc{
  char tab[MAX];
  int svt; //bloc suivant
  int prd; //bloc precedent
  int nbr; //nb de caracteres inseres dans le bloc
}Tbloc;

typedef Tbloc Buffer;

typedef struct Etudiant{
  char identifiant[4]; //une cle specifique pr chaque bloc
  char taille[3]; //taille du bloc
  char efface[1];
  char nom[25];
  char prenom[25];
  char matricule[12];
  char specialite[30];
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

void ouvrir (LOV **f, char nom[25], char mode);
void fermer(LOV *f);
void lireBloc(LOV *f, int i, Buffer *buf);
void ecrireBloc(LOV *f,int i, Buffer *buf);
int EnTete(LOV *f, int i);
void affecterEntete(LOV *f, int i, int x);
int allouerBloc(LOV *f);
void recherche(LOV *f, int cle, bool *trouve, int *i, int *j);
void suppression_logique(LOV *f, int cle);


void *ouvrir (LOV **f, char nom[25], char mode){ //liste chainee
 (**f)=malloc(sizeof(LOV)); //*f pointe sur la structure LOV
 if(mode=='a' || mode=='A')
 {
    (*f)->fichier=fopen(nom, "rb+");
    if((*f)->fichier != NULL){
        fread(&(*f)->entete, sizeof(EnTete),1, (*f)->fichier);
    }
 }
 else if(mode=='n' || mode=='N'){
 (*f)->fichier = fopen(nom, "wb+"); //creer un nouveau f

 if((*f)->fichier != NULL){ 
  (*f)->entete.insert=0; // initialiser l'entete
  (*f)->entete.nbloc=1;
  (*f)->entete.sup=0;
  (*f)->entete.premier=1;
 }
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

}
 
void ecrireBloc(LOV *f,int i, Buffer *buf){  //ecrire un bloc
  fseek(f->fichier, sizeof(Entete) + sizeof(Tbloc)*(i-1), SEEK_SET); // positionnement au debut du bloc numero i
  fwrite(buf, sizeof(struct Tbloc), 1, f->fichier);  //ecrire un seul bloc de caractère correspondant a la taille du bloc dans le buffer
}

int EnTete(LOV *f, int i){
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
    affecterEntete(f, 4, EnTete(f, 4)+ 1); //incremente le nb de bloc par 1
    return EnTete(f, 4); //retourne le nb de blocs
}

void recuperechaine(char chaine[], int n, int* i)
{
Buffer buf;
    for(int j=0; j<n-1; j++){
        chaine[j]=buf.tab[(*i)];
        (*i)++;
    }
}

//----------------------- fonction de la recherche dans le fichier--------------------------------------------//

void recherche(LOV *f, int cle, bool *trouve, int *i, int *j){
 *trouve=0;
 char identifiant[4];
 char taille[3];
 *i= EnTete(f,1); //indice pointe sur le premier bloc;
 *j=0;
 int j1;
 j1=*j; //sauvgarder le j
 Buffer buf;
 lireBloc(f, *i, &buf); //lire le bloc
 recuperechaine(identifiant, 5, j);
 while(atoi(identifiant)>cle){ //atoi fait la conversion du char a un entier
    recuperechaine(taille, 4, j); // recuperer la taille du bloc
    *j = (*j) + atoi(taille) - 7;//avencer j au prochain bloc
    if((*j)>= buf.nbr) //verifier si on a depasser la taille du bloc
    {
    if(buf.svt != -1){ //verifier si c le dernier bloc
        *i=buf.svt; //sinon on avence au bloc suivant
        *j=0; //initialiser lindice du debut du bloc
        lireBloc(f, *i, &buf); //lire le bloc svt
    }
    else{
        break;
    }
    }
    j1=*j;
    recuperechaine(identifiant, 5, j);
    }
    if(cle==atoi(identifiant)){
        (*trouve)=1;
    }
    (*trouve)=0;
    *j=j1;
}

//----------------------- fonction de suppression logique dans le fichier--------------------------------------------//
void Suppression_logique(LOV *f, int cle, char nomfichier[]) {
    // Déclaration des variables locales
    bool trouve;
    int i, j;
    Buffer buf;
    // Recherche de la chaîne c dans le fichier
    recherche(f, cle, &trouve, &i, &j);

    // Si la chaîne c est trouvée
    if (trouve) {
        // On avance de 8 positions dans le tableau
        *j = *j + 8;

        // Si on n'a pas dépassé la taille du tableau
        if (j < MAX) {
            // On remplace le caractère par '1'
            buf.tab[j] = '1';

            // On écrit le caractère dans le fichier
            ecrireBloc(f, i, &buf);
        }
    }
    // Sinon
    else {
        // On affiche un message d'erreur
        printf("L'élément n'existe pas !\n");
    }
}