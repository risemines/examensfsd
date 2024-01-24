#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CAR 1024
#ifndef HEADER_H
#define HEADER_H

typedef struct Tbloc //declaration du bloc
{
    char tab[MAX_CAR]; //tableau pr les informations entrees
    int suivant;  //suivant de chaque bloc
    int nb;     // nb de caracteres inseres
} Tbloc;

typedef struct Etudiant  //enregistrement des informations
{
    char    Identifiant[4];  //aussi la cle entre 0000 et 9999
    char    Taille[3];     //la taille de l'enregistrement
    char    Efface[1];      //un champ pr indiquer si le bloc est supprime ou pas
    char    Disponible[1];  //indique la disponibilite du bloc
    char    Annee[1];        // l'annee universitaire peut etre: 1,2,3,4 ou 5
    char    Bac[4];        // l'annee de l'obtention du bac
    char    Nom[25];      // champ variable entre 10 et 25 caracters
    char    Prenom[25];     // champ variable entre 10 et 25 caracteres
    char    Specialite[25];       //champ variable entre 10 et 25 caracteres
    char    Universite[930];    // 1024-(la somme de la taille des autres champs)
} Etudiant;

typedef struct Entete  //declaration de l'entete
{
    int tete;       //indique le premier bloc
    int inseres;    //nb de caracteres inseres
    int supprimes;  //nb de caracteres supprimes
    int nbBloc;     //nb de bloc
} Entete;

typedef struct LOV_C //declaration du fichier
{
    FILE* fichier;
    Entete entete;
} LOV_C;

typedef Tbloc Buffer; // declarartion du buffer de type Tbloc
Buffer buf;

void ouvrir     (LOV_C **f, char nom[], char mode );
void fermer     (LOV_C *f                         );
int entete      (LOV_C *f, int i                  );
void aff_entete (LOV_C *f, int i, int valeur      );
void lireDir    (LOV_C *f, int i, Buffer *buf     );
void ecrireDir  (LOV_C *f, int i, Buffer *buf     );
int  allocBloc  (LOV_C *f                         );


void ouvrir(LOV_C **f, char nom[], char mode) //permet l'overture du fichier
{
    *f = malloc(sizeof(LOV_C));  //allocation
    if(mode == 'a' || mode == 'A')  //ouvrir un ancien fichier
    {
        (*f)->fichier = fopen(nom, "rb+");  //ouvrir en mode lecture/ecriture

        if((*f)->fichier != NULL) //si le fichier existe
        {
            fread(&((*f)->entete), sizeof(Entete), 1, (*f)->fichier);
        }
    }
    else if(mode == 'n' || mode == 'N') //ouvrir un nouveau fichier
    {
        (*f)->fichier = fopen(nom, "wb+"); //ouvrir en mode ecriture
        if((*f)->fichier != NULL) //si le fichier existe
        {
            (*f)->entete.tete = 1; //initialiser les valeurs de l'entete
            (*f)->entete.inseres = 0;
            (*f)->entete.supprimes = 0;
            (*f)->entete.nbBloc = 1;
        }
    }
}


void fermer(LOV_C *f) //permet la fermeture du fichier
{
    rewind(f->fichier); //se positionner au debut du fichier
    fwrite(&(f->entete), sizeof(Entete), 1, f->fichier); //sauvgarde de l'entete
    fclose(f->fichier);//fermer le fichier
    free(f);//librer l'espace
}


int entete(LOV_C *f, int i) //retourne la valeur de l'entete selon le i
{
    if(i == 1)
        return f->entete.tete;
    else if(i == 2)
        return f->entete.inseres;
    else if(i == 3)
        return f->entete.supprimes;
    else if(i == 4)
        return f->entete.nbBloc;
    else
        return -1;
}


void aff_entete(LOV_C *f, int i, int valeur) //affecter une valeur dans l'entete
{
    if(i == 1)
        f->entete.tete = valeur;
    else if(i == 2)
        f->entete.inseres = valeur;
    else if(i == 3)
        f->entete.supprimes = valeur;
    else if(i == 4)
        f->entete.nbBloc = valeur;
}


void lireDir(LOV_C *f, int i, Buffer *buf) //lire directory/lire le bloc a partir du buffer et la position i
{
    fseek(f->fichier, sizeof(Entete) + sizeof(Tbloc)*(i-1), SEEK_SET); //positionner le curseur au debut du fichier
    fread(buf, sizeof(Tbloc), 1, f->fichier); //lire le bloc
}


void ecrireDir(LOV_C *f, int i, Buffer *buf) //ecrire le bloc de l'adresse i
{
    fseek(f->fichier, sizeof(Entete) + sizeof(Tbloc)*(i-1), SEEK_SET); //positionne le curseur au debut du fichier
    fwrite(buf, sizeof(Tbloc), 1, f->fichier); //ecrire le bloc
}


int allocBloc(LOV_C *f) //allocation d'un bloc
{
    aff_entete(f, 4, entete(f, 4) + 1); //incrementer le nb de blocs
    return entete(f, 4);
}

void generer(int *taille,int tmp_table[]) //a la creation du fichier cette procedure genere aleatoirement des identifiants
{
    int i,j,k,temp; //variables pr les boucles
    for (i=0; i<(*taille); i++) //boucle pr la generation aleatoire entre 0000 et 9999
    {
        tmp_table[i]=Aleanombre(9999);
    }
    for (i=0; i<(*taille); i++) // boucle pr eviter les repetitions
    {

        for (j=i+1; j<(*taille);)
        {
            if (tmp_table[i]==tmp_table[j]) //si il y a une repetition
            {
                for (k=j; k<(*taille); k++)
                {
                    tmp_table[k]=tmp_table[k+1];
                }
                (*taille)--; //diminuer la taille
            }
            else
            {
                j++; //avencer a la case suivante
            }
        }
    }
    for (i=0; i<(*taille); i++) //boucle pr trier les identifiants
    {

        for (j=0; j<(*taille); j++)
        {

            if (tmp_table[j]>tmp_table[j+1])
            {
                temp=tmp_table[j];
                tmp_table[j]=tmp_table[j+1];
                tmp_table[j+1]=temp;
            }
        }
    }
}

void creation(LOV_C **fichier) //creation du fichier
{
    int  nb_etudiants_au_debut; //nb des etudiants crees aleatoirement
    int cle_entier,somme_des_tailles_entier,disponible_entier,annee_entier,bac_entier; //les champs en entier entres
    int taille_chaine_nom,taille_chaine_prenom,taille_chaine_spec,taille_chaine_uni; //les champs entres comme chaine de caracteres
    char cle_char[4],somme_des_tailles_char[3],disponible_char[1],annee_char[1],bac_char[4];
    int nb_inseres=0,nbblocs=0,j=0,nb_caracteres=0; //pr mettre a jour l'entete
    Etudiant Etudiant;
    int i;
    int k,l;
    int taille_table; //taille de table des identfiant generes alea
    int tmp_table[1024]; //la taille de la table
    taille_table = 6;
    generer(&taille_table,tmp_table); //generer les identifiants alea
    nb_etudiants_au_debut =taille_table;
    ouvrir(fichier,"Etudiant.bin",'n'); //ouvrir le nv fichier
    k= entete(*fichier,1); //k pointe sur le 1er bloc
    for(i=0 ;i < nb_etudiants_au_debut;i++ ) //boucle pr remplir les informations alea
    {
        cle_entier=tmp_table[i]; //affecter a chaque l'identifiant genere
        disponible_entier=Aleanombre(2); //generer alea
        annee_entier= 1 + Aleanombre(5) ;
        bac_entier= 1990 + Aleanombre(34);
        taille_chaine_nom=10+Aleanombre(15);
        taille_chaine_prenom=10+Aleanombre(15);
        taille_chaine_spec=10+Aleanombre(15);
        taille_chaine_uni=5+Aleanombre(905);

        somme_des_tailles_entier=4+3+1+1+1+4+taille_chaine_nom+1+taille_chaine_prenom+1+taille_chaine_spec+1+taille_chaine_uni+1; //+1 pr les separateurs
        int_vers_string(cle_char,cle_entier,4); //conversion du entier a chaine de cara
        printf("\ncle :%s ",cle_char ); //afficher l'identifiant a chaque fois
        int_vers_string(bac_char,bac_entier,4); //conversion de tout les champs entier en une chaine de caracteres
        int_vers_string(somme_des_tailles_char,somme_des_tailles_entier,3);
        int_vers_string(disponible_char,disponible_entier,1);
        int_vers_string(annee_char,annee_entier,1);



        strcpy(Etudiant.Identifiant, cle_char);
        strcpy(Etudiant.Taille, somme_des_tailles_char); //copier les champs genere vers le bloc
        strcpy(Etudiant.Efface, "0");
        strcpy(Etudiant.Disponible, disponible_char  );
        strcpy(Etudiant.Annee, annee_char);
        strcpy(Etudiant.Bac, bac_char );


        char nom_chaine[taille_chaine_nom];       //declaration d'une chaine
        Aleachaine(nom_chaine, taille_chaine_nom); //la generer alea
        strcpy(Etudiant.Nom, nom_chaine); //l'affecter au champ qui convient

        char prenom_chaine[taille_chaine_prenom];  //faire la meme chose avec les autres chaines de caracteres du bloc
        Aleachaine(prenom_chaine, taille_chaine_prenom);
        strcpy(Etudiant.Prenom, prenom_chaine );

        char spec_chaine[taille_chaine_spec];
        Aleachaine(spec_chaine, taille_chaine_spec);
        strcpy(Etudiant.Specialite, spec_chaine );

        char uni_chaine[taille_chaine_uni];
        Aleachaine(uni_chaine, taille_chaine_uni);
        strcpy(Etudiant.Universite, uni_chaine );

        nb_caracteres= somme_des_tailles_entier + nb_caracteres;//sauvgarde de nb de caracteres inseres
        if (nb_caracteres<=MAX_CAR) //verifier si il ya d'espace dans le bloc et enregistrer dans le buffer
        {
            ecrire_chaine(4,&j,Etudiant.Identifiant,&buf); //sauvgarder les champs dans le buffer
            ecrire_chaine(3,&j,Etudiant.Taille,&buf);
            ecrire_chaine(1,&j,Etudiant.Efface,&buf);
            ecrire_chaine(1,&j,Etudiant.Disponible,&buf);
            ecrire_chaine(1,&j,Etudiant.Annee,&buf);
            ecrire_chaine(4,&j,Etudiant.Bac,&buf);
            ecrire_chaine(taille_chaine_nom,&j,Etudiant.Nom,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_prenom,&j,Etudiant.Prenom,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_spec,&j,Etudiant.Specialite,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_uni,&j,Etudiant.Universite,&buf);
            ecrire_chaine(1,&j,"#",&buf);
            buf.nb =nb_caracteres; //maj le nb de caracteres
        }
        else //si'il y a plus d'espace
        {
            l = allocBloc(*fichier);//allouer un nv bloc
            buf.suivant= l; //passer au nv bloc
            ecrireDir(*fichier,k,&buf); //ecrire le bloc
            nb_inseres = buf.nb + nb_inseres; //maj le nb de caracteres inseres
            nbblocs ++; //incrementer le nb de blocs
            k=l; //affecter la position du nv bloc au k
            nb_caracteres=somme_des_tailles_entier;//sauvgarder le nb de caracters inseres
            j=0; //initialiser le j au debut du bloc
            ecrire_chaine(4,&j,Etudiant.Identifiant,&buf); //affecter les champs au buffer
            ecrire_chaine(3,&j,Etudiant.Taille,&buf);
            ecrire_chaine(1,&j,Etudiant.Efface,&buf);
            ecrire_chaine(1,&j,Etudiant.Disponible,&buf);
            ecrire_chaine(1,&j,Etudiant.Annee,&buf);
            ecrire_chaine(4,&j,Etudiant.Bac,&buf);
            ecrire_chaine(taille_chaine_nom,&j,Etudiant.Nom,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_prenom,&j,Etudiant.Prenom,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_spec,&j,Etudiant.Specialite,&buf);
            ecrire_chaine(1,&j,"$",&buf);
            ecrire_chaine(taille_chaine_uni,&j,Etudiant.Universite,&buf);
            ecrire_chaine(1,&j,"#",&buf);
            buf.nb =nb_caracteres; //affecter le nb de caracteres inseres
            buf.suivant= -1; //pr indiquer le dernier bloc
        }
    }
    ecrireDir(*fichier, k,&buf);//ecrire le bloc
    nb_inseres = buf.nb + nb_inseres;
    nbblocs ++;
    aff_entete(*fichier, 2, nb_inseres); //mettre a jour l'entete
    aff_entete(*fichier, 3, 0);
    aff_entete(*fichier, 4, nbblocs);
    fermer(*fichier);
}


void extraire(char chaine[], int n, int* i) //extraire un champs du buffer dans une chaine de caracteres
{
    for(int j = 0; j < (n-1); j++) //boucle de laffectation caractere par caractere
    {
        chaine[j] = buf.tab[(*i)];
        (*i)++;
    }

    chaine[n-1] = '\0';
}


void recherche(LOV_C *f, int cle, int* trouve, int* i, int* j) //recherche d'un identifiant 'cle'
{
    *trouve = 0;
    *i = entete(f, 1); //premier bloc
    *j = 0;
    int jSauv;
    char taille[4]; //pr recuperer la taille
    char identifiant[5];
    char efface; //verifier sa existence

    lireDir(f, *i, &buf); //lire le bloc
