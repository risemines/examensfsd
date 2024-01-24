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




  jSauv = *j; //pr sauvgrader le j
    extraire(identifiant, 5, j); //extraire l'identfiant du bloc
    efface = buf.tab[*j + 3]; //recupere le champ 'efface'

    while( atoi(identifiant) < cle) //si'il est inferieur avencer
    {
        extraire(taille, 4, j); //extraire la taille
        *j = (*j) + atoi(taille) - 7; //bloc suivant

        if( (*j) >= buf.nb )  //verifier on a depassé le nombre de caratères insérés dans le bloc
        {

            if(buf.suivant != -1) //verifier si on n'est pas a la fin
            {
                *i = buf.suivant; //avencer
                *j = 0; //debut du bloc
                lireDir(f, *i, &buf); //lire le bloc
            }
            else
            {
                break;
            }
        }
        jSauv = *j; //sauvgarder le j
        extraire(identifiant, 5, j); //recuperer l'identifiant
        efface = buf.tab[*j + 3]; //verifier l'existence
    }

    if(cle == atoi(identifiant) && efface == '0')  //si il sont egaux et le bloc existe
    {
        (*trouve) = 1; //on a trouver
    }
    if( atoi(identifiant) < cle) //si on est dans le meme bloc
    {
        extraire(taille, 4, j); //recupere la taille
        *j = (*j) + atoi(taille) - 3; //au enregistrement suivant
        jSauv = *j;
    }
    *j = jSauv; //recuperer j
}


void insertion(LOV_C *f, char etudiant[], int taille) //insertion dun enregistrement
{
    int i, j, trouve, suivant;
    char cle[5];
    int k;
    char* tmpData;
    int tmpTaille;

    strncpy(cle, etudiant, 4);//recuperer l'identifiant
    cle[4] = '\0';

    recherche(f, atoi(cle), &trouve, &i, &j); //verifier s'il l'identifiant existe

    if(trouve == 0) //s'il n'existe pas
    {
        lireDir(f, i, &buf); //lire le bloc


        if(taille <= (MAX_CAR - buf.nb) ) //verifier si il ya d'espace dans le bloc
        {

            tmpTaille = buf.nb - j; //recuperer la taille des donnees
            if(tmpTaille > 0)
            {
                tmpData = malloc(sizeof(char) * tmpTaille); //pr sauvgarder les donnees existante
            }

            for(k = 0; k < tmpTaille; k++) //l'affecter dans la chaine temporairement
            {
                tmpData[k] = buf.tab[j + k];
            }


            for(k = 0; k < taille; k++)//affecter les nv donnees a la position j
            {
                buf.tab[j] = etudiant[k];
                j++;
            }

            for(k = 0; k < tmpTaille; k++)//retourner les donnes dans leur positions apres le nv
            {
                buf.tab[j] = tmpData[k];
                j++;
            }

            buf.nb += taille; //maj le nb de caracteres inseres
            ecrireDir(f, i, &buf); //ecrire le buffer
        }
        else
        {
            tmpTaille = buf.nb - j; //recuperer la taille des donnes restantes
            if(tmpTaille > 0)
            {
                tmpData = malloc(sizeof(char) * tmpTaille); //allocation pr sauvgarder les donnes
            }

            for(k = 0; k < tmpTaille; k++)
            {
                tmpData[k] = buf.tab[j + k]; //l'affecter temporairement dans une chaine
            }

            suivant = buf.suivant;
            buf.suivant = allocBloc(f); //allocation d'un nv bloc
            buf.nb -= tmpTaille; //maj du nb de blocs deplacer les donnes dans un nv bloc
            if(buf.nb > 0)
            {
                ecrireDir(f, i, &buf); //ecrire le bloc

                i = buf.suivant;

                for(k = 0; k < taille; k++) //inserer les donnes
                {
                    buf.tab[k] = etudiant[k];
                }

                buf.nb = taille;
            }
            else //si on doit inserer au debut de bloc
            {
                for(k = 0; k < taille; k++)
                {
                    buf.tab[j + k] = etudiant[k];
                }

                buf.nb = taille;
            }

            if(tmpTaille <= (MAX_CAR - buf.nb)) // si il y d'espace
            {
                j = buf.nb;

                for(k = 0; k < tmpTaille; k++) //inserer les donnes
                {
                    buf.tab[j] = tmpData[k];
                    j++;
                }

                if(tmpTaille > 0)
                {
                    buf.nb += tmpTaille;
                }

                buf.suivant = suivant; //passer au bloc svt
                ecrireDir(f, i, &buf); //ecrire le bloc
            }
            else // pas d'espace
            {
                buf.suivant = allocBloc(f); //allouer un nv bloc
                ecrireDir(f, i, &buf); //ecrire le bloc
                i = buf.suivant; //passer au nv bloc

                for(k = 0; k < tmpTaille; k++) //inserer
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

        aff_entete(f, 2, entete(f, 2) + taille); //maj l'entete
        if(tmpData != NULL)
            free(tmpData); //liberer

        printf("\nL'etudiant a ete insere\n");
    }
    else
    {
        printf("\nL'etudiant existe deja!\n");
    }

}
void lire(char chaine[], int *taille) //lire les donnees
{
    int identifiant, annee, bac;
    char nom[25], prenom[25], specialite[25], universite[930];

    do //boucle pour assurer qu'on entre un identifiant correct
    {
        printf("Entrez un identifiant (<= 9999):");
        scanf("%d", &identifiant);
    }
    while(identifiant > 9999);

    printf("Entrez votre nom:");
    scanf("%s", nom);

    printf("Entrez votre prenom:");
    scanf("%s", prenom);

    do
    {
        printf("Entrez votre annee universitaire(un seul chiffre): ");
        scanf("%d", &annee);
    }
    while(annee < 1 || annee > 5);


    do
    {
        printf("Entrez votre annee de bac: ");
        scanf("%d", &bac);
    }
    while(bac < 1990 || bac > 2023);


    printf("Entrez votre specialite: ");
    scanf("%s", specialite);

    printf("Entrez votre universite: ");
    scanf("%s", universite);
    *taille = 4 + 3 + 1 + 1 + 1 + 4 + strlen(prenom) + 1 + strlen(nom) + 1 + strlen(specialite) + 1 + strlen(universite) + 1; //affecter la taille

    sprintf(chaine, "%04d%03d01%d%d%s$%s$%s$%s#", identifiant, *taille, annee, bac, nom, prenom, specialite, universite); //affecter le tout dans une seule chaine
}

void affichage(LOV_C **fichier, int i) {
	ouvrir(fichier, "Etudiant.bin", 'a');
	lireDir(*fichier, i, &buf);

	int j = 0;

	while(j < buf.nb) {
		printf("Identifiant: ");
		printf("%c%c%c%c\n", buf.tab[j], buf.tab[j+1], buf.tab[j+2], buf.tab[j+3]);
		j += 4;
		printf("Taille: ");
		printf("%c%c%c\n", buf.tab[j], buf.tab[j+1], buf.tab[j+2]);
		j += 3;
		printf("Efface: ");
		printf("%s\n",buf.tab[j] == '0' ? "NON" : "OUI");
		j++;
		printf("Disponible: ");
		printf("%s\n", buf.tab[j] == '0' ? "NON" : "OUI");
		j++;
		printf("Annee universitaire: ");
		switch(buf.tab[j]) {
			case '1':
				printf("Licence 1\n");
				break;
			case '2':
				printf("Licence 2\n");
				break;
			case '3':
				printf("Licence 3\n");
				break;
			case '4':
				printf("Master 1\n");
				break;
            case '5':
				printf("Master 2\n");
				break;
		}

		j++;
		printf("Annee de bac: ");
		printf("%c%c%c%c\n", buf.tab[j], buf.tab[j+1], buf.tab[j+2], buf.tab[j+3]);
		j += 4;
		printf("Nom: ");
		while(buf.tab[j] != '$') {
			printf("%c", buf.tab[j]);
			j++;
		}

		j++;

		printf("\nPrenom: ");
		while(buf.tab[j] != '$') {
			printf("%c", buf.tab[j]);
			j++;
		}

		j++;
		printf("\nSpecialite: ");
		while(buf.tab[j] != '$') {
			printf("%c", buf.tab[j]);
			j++;
		}
		j++;
		printf("\nUniversite: ");
		while(buf.tab[j] != '#') {
			printf("%c", buf.tab[j]);
			j++;
		}
		j++;
		printf("\n-----------------------------------------------------------------------------------------------------------\n");
	}

}

int Aleanombre( int N ) //generer un nombre aleatoirement
{
    return ( rand() % N );
}

void  Aleachaine ( char chaine[], int N ) //generer une chaine aleatoirement
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

void int_vers_string(char chaine[], int n, int longueur) //conversion d'un entier vers une chaine de caracters
{

    int k;
    for(k=longueur-1; k>=0; k--)
    {
        chaine[k]=(n%10)+'0';
        n=n/10;
    }
}

void ecrire_chaine( int n, int *j, char chaine[], Buffer *buf) //affecter la chaine dans le buffer de taille n
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

void Suppressionlogique(LOV_C *f,char c[20]) { //suppression logique d'un bloc

    int trouv,i, j;

    ouvrir(f, "Etudiant.bin", 'A'); // ouvrir le fichier en mode ajout

    recherche(f,c, &trouv, &i, &j); // rechercher l'élément à supprimer

    if (trouv==1) { // si l'élément existe

        j = j + 7; // se positionner après l'élément

        if (j < MAX_CAR) { // si on n'est pas à la fin du fichier

            buf.tab[j] = '1'; // marquer l'élément comme supprimé
            ecrireDir(f, i, &buf); // écrire le changement dans le fichier
            //aff_entete(f,3,Entete(f,3)+nb_taille);
        }

    } else { // si l'élément n'existe pas

        printf("L'élément n'existe pas!\n"); // afficher un message d'erreur

    }

    fermer(f); // fermer le fichier

}

#endif

