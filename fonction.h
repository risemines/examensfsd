#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CAR 1024
#ifndef HEADER_H
#define HEADER_H

typedef struct TblocLOV_C
{
    char tab[MAX_CAR];
    int suivant;
    int nb;     // NOMBRE DE CARATèRES INSéRéS DANS LE BLOC
} TblocLOV_C;

typedef struct Ouvrage
{
    char    Identifiant[4];
    char    Taille[3];
    char    Efface[1];
    char    Disponible[1];  // GENERER ALEATOIREMENT SOIT 1 SOIT 0
    char    Type[1];        // GENERER ALEATOIREMENT DEPUIS UN ENSEMBLE TQ: '1'-LIVRE '2'-MEMOIRE/PFE '3'-ARTICL '4'-PéRIODIQUE
    char    Anne[4];        // GENERER ALEATOIREMENT ENTRE 1950 ET 2021
    /*LES CHAMPS VARIABLES */
    char    Titre[25];      // ALEATOIREMENT (10~25)
    char    Auteur[25];     // ALEATOIREMENT (10~25)
    char    Cote[25];       //ALEATOIREMENT (10~25)
    char    Resume[930];    // {(MAX_CAR -4-3-1-1-4-1-25-25-25 -5'SUPPLIMENTAIRES) POUR RESUMER= 930 }
} Ouvrage;


//LES CHAMPS QUI N'ONT PAS DE TAILLE FIXE SONT SéPARéS PAR LE CARACTèRE '$'

typedef struct Entete
{
    int tete;       //NUMERO DU PREMIER BLOC
    int inseres;    //NOMBRES GLOBAL DE CARATèRES INSéRéS
    int supprimes;  //NOMBRES GLOBAL DE CARATèRE SUPPRIMéS
    int nbBloc;     //NOMBRE DE BLOCS
} Entete;

typedef struct LOV_C
{
    FILE* fichier;
    Entete entete;
} LOV_C;

typedef TblocLOV_C Buffer;
Buffer buf;

void ouvrir     (LOV_C **f, char nom[], char mode );
void fermer     (LOV_C *f                         );
int entete      (LOV_C *f, int i                  );
void aff_entete (LOV_C *f, int i, int valeur      );
void lireDir    (LOV_C *f, int i, Buffer *buf     );
void ecrireDir  (LOV_C *f, int i, Buffer *buf     );
int  allocBloc  (LOV_C *f                         );


void ouvrir(LOV_C **f, char nom[], char mode)
{
    *f = malloc(sizeof(LOV_C));

    if(mode == 'a' || mode == 'A')
    {
        (*f)->fichier = fopen(nom, "rb+");

        if((*f)->fichier != NULL)
        {
            fread(&((*f)->entete), sizeof(Entete), 1, (*f)->fichier);
        }
    }
    else if(mode == 'n' || mode == 'N')
    {
        (*f)->fichier = fopen(nom, "wb+");

        if((*f)->fichier != NULL)
        {
            (*f)->entete.tete = 1;
            (*f)->entete.inseres = 0;
            (*f)->entete.supprimes = 0;
            (*f)->entete.nbBloc = 1;
        }
    }
}


void fermer(LOV_C *f)
{
    rewind(f->fichier);
    fwrite(&(f->entete), sizeof(Entete), 1, f->fichier);
    fclose(f->fichier);
    free(f);
}


int entete(LOV_C *f, int i)
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


void aff_entete(LOV_C *f, int i, int valeur)
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


void lireDir(LOV_C *f, int i, Buffer *buf)
{
    fseek(f->fichier, sizeof(Entete) + sizeof(TblocLOV_C)*(i-1), SEEK_SET);
    fread(buf, sizeof(TblocLOV_C), 1, f->fichier);
}


void ecrireDir(LOV_C *f, int i, Buffer *buf)
{
    fseek(f->fichier, sizeof(Entete) + sizeof(TblocLOV_C)*(i-1), SEEK_SET);
    fwrite(buf, sizeof(TblocLOV_C), 1, f->fichier);
}


int allocBloc(LOV_C *f)
{
    aff_entete(f, 4, entete(f, 4) + 1);
    return entete(f, 4);
}

void creation_table_identifiantes(int *taille,int tmp_table[])
{
    int i,j,k,temp;
    for (i=0; i<(*taille); i++) //boucle de generation
    {
        tmp_table[i]=Aleanombre(9999);  //le nombre maximum d'un identifiant est 9999
    }
    for (i=0; i<(*taille); i++)//boucle de la supprition des reputitions
    {

        for (j=i+1; j<(*taille);)
        {
            if (tmp_table[i]==tmp_table[j])
            {
                for (k=j; k<(*taille); k++)
                {
                    tmp_table[k]=tmp_table[k+1];
                }
                (*taille)--;
            }
            else
            {
                j++;
            }
        }
    }
    for (i=0; i<(*taille); i++)//boucle du triage
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
/* *****************- MODULE DE CREATION DU FICHIER  LOV~C -******************/
void creation_fichier_LOV_C(LOV_C **fichier)
{
    /*---------------------------*/
    int  nb_ouvrages_au_debut;
    int cle_entier,somme_des_tailles_entier,disponible_entier,type_entier,annee_publication_entier;
    int taille_chaine_titre,taille_chaine_auteur,taille_chaine_cote,taille_chaine_resume;
    char cle_char[4],somme_des_tailles_char[3],disponible_char[1],type_char[1],annee_publication_char[4];
    int nb_globale_inseres=0,nbblocs=0,j=0,compteur_nb_caracteres_chaque_bloc=0;
    Ouvrage Ouvrage_crier; //L'ARTUCLE QUE ON UTILISE TEMPOREREMENT
    int i;   //COMPTEUR POUR ARRETER LA BOUCLE
    int I0,I1; //ADRESSE BLOC
    int taille_table;
    int tmp_table[1024];//car il
    taille_table = 5;//Aleanombre(60 );// ENTRE 1 ET 60 ARTUCLE AU DEBUT i.e: COMBIEN PEUT ON ECRIRE EN DEBUT
    creation_table_identifiantes(&taille_table,tmp_table);
    nb_ouvrages_au_debut =taille_table;
    ouvrir(fichier,"Ouvrages.bin",'n');
    I0= entete(*fichier,1);
    for(    i=0 ;   i < nb_ouvrages_au_debut   ;   i++ )
    {
        cle_entier=tmp_table[i];
        disponible_entier=Aleanombre(2);
        type_entier= 1 + Aleanombre(4) ; //SOIT 1 2 3 4
        annee_publication_entier= 1950 + Aleanombre(71);  // 2021-1950 = 71   PUISQUE L'AMPLUTIDE D'INTERVALE [1950;2021]
        taille_chaine_titre=10+Aleanombre(15);  // MIN=10 CARACTERES , MAX=25 CARACTERES
        taille_chaine_auteur=10+Aleanombre(15); // MIN=10 CARACTERES , MAX=25 CARACTERES  POUR DES RAISONS D'AFFICHAGE
        taille_chaine_cote=10+Aleanombre(15);   // MIN=10 CARACTERES , MAX=25 CARACTERES
        taille_chaine_resume=5+Aleanombre(905); // MIN=10 CARACTERES , MAX=1000 CARACTERES
        somme_des_tailles_entier=4+3+1+1+1+4+taille_chaine_titre+1+taille_chaine_auteur+1+taille_chaine_cote+1+taille_chaine_resume+1;
        //SOMME DU TAILLE DES CHAMPS:IDENTIFIANT+TAILLE+EFFACE+DISPONIBLE+TYPE+ANNE+TITRE+$+AUTEUR+$+COTE+$+RESUME
        int_vers_string(cle_char,cle_entier,4);
        int_vers_string(annee_publication_char,annee_publication_entier,4);
           printf("\nis it working? cle :%s ",cle_char );
        int_vers_string(somme_des_tailles_char,somme_des_tailles_entier,3);
        int_vers_string(disponible_char,disponible_entier,1);
        int_vers_string(type_char,type_entier,1);
        /*AFFECTATION EN UN OUVRAGE */
        strcpy(Ouvrage_crier.Identifiant, cle_char);
        strcpy(Ouvrage_crier.Taille, somme_des_tailles_char);
        strcpy(Ouvrage_crier.Efface, "0");
        strcpy(Ouvrage_crier.Disponible, disponible_char  );
        strcpy(Ouvrage_crier.Type, type_char);
        strcpy(Ouvrage_crier.Anne, annee_publication_char );
        //printf("\ncle :%s ",Ouvrage_crier.Anne );
        char titre_chaine[taille_chaine_titre];
        Aleachaine(titre_chaine, taille_chaine_titre);
        strcpy(Ouvrage_crier.Titre, titre_chaine);

        char auteur_chaine[taille_chaine_auteur];
        Aleachaine(auteur_chaine, taille_chaine_auteur);
        strcpy(Ouvrage_crier.Auteur, auteur_chaine );

        char cote_chaine[taille_chaine_cote];
        Aleachaine(cote_chaine, taille_chaine_cote);
        strcpy(Ouvrage_crier.Cote, cote_chaine );

        char resume_chaine[taille_chaine_resume];
        Aleachaine(resume_chaine, taille_chaine_resume);
        strcpy(Ouvrage_crier.Resume, resume_chaine );
        compteur_nb_caracteres_chaque_bloc = somme_des_tailles_entier + compteur_nb_caracteres_chaque_bloc;
        if (compteur_nb_caracteres_chaque_bloc<=MAX_CAR)
        {
                    printf("is it working?");
            ecrire_chaine( 4, &j, Ouvrage_crier.Identifiant, &buf);
            ecrire_chaine( 3, &j,Ouvrage_crier.Taille, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Efface, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Disponible, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Type, &buf);
            ecrire_chaine( 4, &j,Ouvrage_crier.Anne, &buf);
            ecrire_chaine( taille_chaine_titre, &j,Ouvrage_crier.Titre, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_auteur, &j,Ouvrage_crier.Auteur, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_cote, &j,Ouvrage_crier.Cote, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_resume, &j,Ouvrage_crier.Resume, &buf);
            ecrire_chaine( 1, &j,"#", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            buf.nb =compteur_nb_caracteres_chaque_bloc;
        }
        else
        {
            I1 = allocBloc(*fichier);
            buf.suivant= I1;
            ecrireDir(*fichier,I0,&buf);
               printf("bloc crier ok \n");//TEST
            nb_globale_inseres = buf.nb + nb_globale_inseres;
            nbblocs ++;
            I0=I1;
            compteur_nb_caracteres_chaque_bloc=somme_des_tailles_entier;//POUR NE PAS ECRAZER
            j=0;
               printf("%d\n",compteur_nb_caracteres_chaque_bloc );
            ecrire_chaine( 4, &j, Ouvrage_crier.Identifiant, &buf);
            ecrire_chaine( 3, &j,Ouvrage_crier.Taille, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Efface, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Disponible, &buf);
            ecrire_chaine( 1, &j,Ouvrage_crier.Type, &buf);
            ecrire_chaine( 4, &j,Ouvrage_crier.Anne, &buf);
            ecrire_chaine( taille_chaine_titre, &j,Ouvrage_crier.Titre, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_auteur, &j,Ouvrage_crier.Auteur, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_cote, &j,Ouvrage_crier.Cote, &buf);
            ecrire_chaine( 1, &j,"$", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            ecrire_chaine( taille_chaine_resume, &j,Ouvrage_crier.Resume, &buf);
            ecrire_chaine( 1, &j,"#", &buf);   //SEPARATEUR ENTRE LES CHAMPS VARIABLES
            buf.nb =compteur_nb_caracteres_chaque_bloc;
            buf.suivant= -1;
        }
    }
    ecrireDir(*fichier, I0,&buf);
    nb_globale_inseres = buf.nb + nb_globale_inseres;
    nbblocs ++;
    aff_entete(*fichier, 2, nb_globale_inseres);
    aff_entete(*fichier, 3, 0);
    aff_entete(*fichier, 4, nbblocs);
    fermer(*fichier);
}


void extraire(char chaine[], int n, int* i)
{
    for(int j = 0; j < (n-1); j++)
    {
        chaine[j] = buf.tab[(*i)];
        (*i)++;
    }

    chaine[n-1] = '\0';
}


void recherche(LOV_C *f, int cle, int* trouve, int* i, int* j)
{
    *trouve = 0;
    *i = entete(f, 1);
    *j = 0;
    int jSauv;
    char taille[4];
    char identifiant[5];
    char efface;

    lireDir(f, *i, &buf);
    jSauv = *j;
    extraire(identifiant, 5, j);
    efface = buf.tab[*j + 3];

    while( atoi(identifiant) < cle)
    {
        extraire(taille, 4, j);
        *j = (*j) + atoi(taille) - 7;

        if( (*j) >= buf.nb )  //si on a depassé le nombre de caratères insérés dans le bloc
        {

            if(buf.suivant != -1)
            {
                *i = buf.suivant;
                *j = 0;
                lireDir(f, *i, &buf);
            }
            else
            {
                break;
            }
        }
        jSauv = *j;
        extraire(identifiant, 5, j);
        efface = buf.tab[*j + 3];
    }

    if(cle == atoi(identifiant) && efface == '0')
    {
        (*trouve) = 1;
    }
    if( atoi(identifiant) < cle)
    {
        extraire(taille, 4, j);
        *j = (*j) + atoi(taille) - 3;
        jSauv = *j;
    }
    *j = jSauv;
}


void insertion(LOV_C *f, char ouvrage[], int taille)
{
    int i, j, trouve, suivant;
    char cle[5];
    int k;
    char* tmpData;
    int tmpTaille;

    strncpy(cle, ouvrage, 4);//RECUPERER LA CLé
    cle[4] = '\0';

    recherche(f, atoi(cle), &trouve, &i, &j);

    if(trouve == 0)  //L'OUVRAGE N'EXISTE PAS DONC ON PEUT L'INSERER
    {
        lireDir(f, i, &buf);


        if(taille <= (MAX_CAR - buf.nb) )  //IL Y A ASSEZ D'ESPACE DANS LE BLOC
        {

            tmpTaille = buf.nb - j;
            if(tmpTaille > 0)
            {
                tmpData = malloc(sizeof(char) * tmpTaille);//RESERVER UNE CHAINE TEMPORAIRE POUR SAUVEGARDER LES DONNéES EXISTANTES
            }

            for(k = 0; k < tmpTaille; k++)  //SAUVEGARDER LES DONNéES EXISTANTES DANS UNE CHAINE TEMPORAIRE
            {
                tmpData[k] = buf.tab[j + k];
            }


            for(k = 0; k < taille; k++)  //INSERER LE NOUVEAU OUVRAGE
            {
                buf.tab[j] = ouvrage[k];
                j++;
            }

            for(k = 0; k < tmpTaille; k++)
            {
                buf.tab[j] = tmpData[k];
                j++;
            }

            buf.nb += taille;
            ecrireDir(f, i, &buf);
        }
        else  //il n'y a pas assez d'espace dans le bloc
        {
            tmpTaille = buf.nb - j;
            if(tmpTaille > 0)
            {
                tmpData = malloc(sizeof(char) * tmpTaille);//RESERVER UNE CHAINE TEMPORAIRE POUR SAUVEGARDER LES DONNéES EXISTANTES
            }

            for(k = 0; k < tmpTaille; k++)  //SAUVEGARDER LES DONNéES EXISTANTES DANS UNE CHAINE TEMPORAIRE
            {
                tmpData[k] = buf.tab[j + k];
            }

            suivant = buf.suivant;
            buf.suivant = allocBloc(f);//ALLOUER UN NOUVEAU BLOC ET FAIRE LE CHAINAGE
            buf.nb -= tmpTaille;//CAR ON VA DEPLACER LES DONNéES EXISTANTS DANS UN NOUVEAU BLOC
            if(buf.nb > 0)
            {
                ecrireDir(f, i, &buf);//ECRIRE L'ANCIEN BLOC

                i = buf.suivant;

                for(k = 0; k < taille; k++)
                {
                    buf.tab[k] = ouvrage[k];
                }

                buf.nb = taille;
            }
            else  //DANS LE CAS Où LE NOUVEAU OUVRAGE DOIT éTRE INSéRé AU DEBUT
            {
                for(k = 0; k < taille; k++)
                {
                    buf.tab[j + k] = ouvrage[k];
                }

                buf.nb = taille;
            }

            if(tmpTaille <= (MAX_CAR - buf.nb))  //S'IL Y A ASSEZ D'ESPACE DANS LE NOUVEAU BLOC POUR METTRE LES DONNéES DéJA EXISTANTES DANS LE BLOC PRECEDANT
            {
                j = buf.nb;

                for(k = 0; k < tmpTaille; k++)  //ECRIRE LES DONNéES EXISTANTES APRèS LE NOUVEAU OUVRAGE
                {
                    buf.tab[j] = tmpData[k];
                    j++;
                }

                if(tmpTaille > 0)
                {
                    buf.nb += tmpTaille;
                }

                buf.suivant = suivant;//FAIRE LE CHAINAGE
                ecrireDir(f, i, &buf);
            }
            else  //S'IL N'Y A PAS ASSEZ D'ESPACE DANS LE NOUVEAU BLOC POUR ECRIRE LES DONNéES EXISTANTES
            {
                buf.suivant = allocBloc(f);//ON ALLOUE UN AUTRE BLOC ET ON FAIT LE CHAINAGE
                ecrireDir(f, i, &buf);
                i = buf.suivant;

                for(k = 0; k < tmpTaille; k++)
                {
                    buf.tab[k] = tmpData[k];
                }

                if(tmpTaille > 0)
                {
                    buf.nb = tmpTaille;
                }

                buf.suivant = suivant;
                ecrireDir(f, i, &buf);
            }
        }

        aff_entete(f, 2, entete(f, 2) + taille);
        if(tmpData != NULL)
            free(tmpData);//LIBERER LA CHAINE TEMPORAIRE

        printf("L'ouvrage a ete insere\n");
    }
    else
    {
        printf("L'ouvrage existe deja!\n");
    }

}
void lireOuvrage(char chaine[], int *taille)
{
    int identifiant, type, annee;
    char titre[25], auteur[25], cote[25], resume[930];

    do
    {
        printf("ENTREZ L'IDENTIFIANT (<= 9999):");
        scanf("%d", &identifiant);
    }
    while(identifiant > 9999);

    printf("ENTREZ LE TITRE DE L'OUVRAGE:");
    scanf("%s", titre);

    printf("ENTREZ LE(S) AUTEUR(S):");
    scanf("%s", auteur);


    do
    {
        printf("LE TYPE DE L'OUVRAGE:\n");
        printf("1-TEXT IMPRIME\n");
        printf("2-DOCUMENT ELECTRONIQUE\n");
        printf("3-ARTICLE\n");
        printf("4-PERIODIQUE\n");
        printf("ENTREZ LE NUMERO CORRESPONDANT AU TYPE:");
        scanf("%d", &type);
    }
    while(type < 1 || type > 4);


    do
    {
        printf("ENTREZ L'ANNEE DE PUBLICATION DE L'OUVRAGE (COMPRIS ENTRE 1950 ET 2021):");
        scanf("%d", &annee);
    }
    while(annee < 1950 || annee > 2021);


    printf("ENTREZ LA COTE DE L'OUVRAGE:");
    scanf("%s", cote);

    printf("ENTREZ LE RESUME DE L'OUVRAGE:");
    scanf("%s", resume);

    //taille = identifiant(4) + taille(3) + effacé(1) + disponible(1) + type(1) + annee(4) + titre + $ + auteur + $ + cote + $ + resume + $ + #
    *taille = 4 + 3 + 1 + 1 + 1 + 4 + strlen(auteur) + 1 + strlen(titre) + 1 + strlen(cote) + 1 + strlen(resume) + 1;

    sprintf(chaine, "%04d%03d01%d%d%s$%s$%s$%s#", identifiant, *taille, type, annee, titre, auteur, cote, resume);
}

void affichage_fichier_LOV_C(LOV_C **fichier)
{
    int I0; //ADRESSE BLOC
    int cmpt=1,j=0;
    ouvrir(fichier,"Ouvrages.bin",'A');
    I0 = entete(*fichier,1);

    while (I0 != -1)
    {
        lireDir(*fichier, I0, &buf);
        printf("\nBLOC Ouvrages.bin : %3d\n",cmpt);

        for (j=0; j<buf.nb ; j++)
        {
            printf("%c",buf.tab[j]);
        }
        I0 = buf.suivant;
        cmpt++;
    }
    fermer(*fichier);
}
int Aleanombre( int N )
{
    return ( rand() % N );
}
/* *****************- MODULE MOT ALEATOIRE -******************/
void  Aleachaine ( char chaine[], int N )
{
    int k;
    char Chr1[26] = "abcdefghijklmnopqrstuvwxyz";
    char Chr2[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (k=0; k<N; k++)
    {
        switch ( rand() % 2 )
        {
        case 0 :
            chaine[k] = Chr1[rand() % 26];
            break ;
        case 1 :
            chaine[k] = Chr2[rand() % 26];
            break ;
        }
    }
}
/* *****************- MODULE UN NOMBRE EN UNE CHAINE SUR UNE LONGEUR DETERMINER -******************/
void int_vers_string(char chaine[], int n, int longueur)
{

    int k;
    for(k=longueur-1; k>=0; k--)
    {
        chaine[k]=(n%10)+'0';          // EXTRACTION CHIFFRE PAR CHIFFRE
        n=n/10;                        // CHIFFRE SUIVANT
    }
}
/* *****************- MODULE POUR ECRIRE LA CHAINE DE CARACTERES EN BUFFER -******************/
void ecrire_chaine( int n, int *j, char chaine[], Buffer *buf)
{
    int k;
    for(k=0; k<n; k++)
    {
        if((*j)<=MAX_CAR)
        {
            buf->tab[*j]=chaine[k];
            (*j)++;
        }
    }
}
#endif


       
