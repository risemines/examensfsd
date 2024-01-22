#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef HEADER_H
#define HEADER_H


typedef struct donnee donnee;
struct donnee{
 char matricule[10];
 char nom[14];
 char prenom[10];
 char annee;
 char* universite;
 char taille[10]; //sauv taille de l'enreg
 char eff; //chanps pr determiner si l'enreg est supp ou pas
 };

typedef struct Buffer Buffer;
struct Buffer{
 char tab[100]; //contient les enregistrements
 int svt; //champ pr l'adresse du bloc svt
 };
typedef struct EnTete EnTete;
struct EnTete{
 int bl_premier;
 int nb_insert;
 int nb_supp;
 int nb_bloc;
 int bl_dernier;
};

typedef struct LOV LOV;
struct LOV{
EnTete entete;
FILE* fichier;
};


int Entete(LOV* f, int i){ //affiche un champ de l'entete selon le i
switch(i){
 case 1: return f->entete.bl_premier;
 case 2: return f->entete.nb_insert;
 case 3: return f->entete.nb_supp;
 case 4: return f->entete.nb_bloc;
 case 5: return f->entete.bl_dernier;
 }
 }

void aff_entete(LOV* f, int i, int val){ //affectation de val a un champ de l'entete
switch(i){
 case 1:{
   f->entete.bl_premier=val;
   break;}
 case 2:{
   f->entete.nb_insert=val;
   break;}
 case 3:{
   f->entete.nb_supp=val;
   break;}
 case 4:{
   f->entete.nb_bloc=val;
   break;}
 case 5:{
   f->entete.bl_dernier=val;
   break;}
 }
}


void LireDir(LOV* f, int i, Buffer *buf){
 fseek(f->fichier, (sizeof(EnTete)+sizeof(Buffer)*(i-1)), SEEK_SET);//positionner le curseur a l'adresse i
 fread(buf, sizeof(Buffer),1,f->fichier);//lire le bloc
 }


void EcrireDir(LOV* f, int i, Buffer *buf){
 fseek(f->fichier, (sizeof(EnTete)+sizeof(Buffer)*(i-1)), SEEK_SET);//positionner le curseur a l'adresse i
 fwrite(buf, sizeof(Buffer),1,f->fichier);//ecrire le bloc
 }

void ouvrir(LOV* f, char* nom, char mode){
if((mode=='n') || (mode =='N')){ //creer un nv fichier
    f->fichier=fopen(nom, "wb+"); //ouvrir un nv fichier en mode ecriture
    aff_entete(f,1,1);
    aff_entete(f,2,0);
    aff_entete(f,3,0);
    aff_entete(f,4,0);
    aff_entete(f,5,1);
    fwrite(&(f->entete),sizeof(EnTete),1,f->fichier);}
else{
    if((mode=='a')||(mode=='A')){ //ouvrir un ancien fichier
        f->fichier=fopen(nom, "rb+"); //ouvrir en mode lecture/ecriture
        fread(&(f->entete),sizeof(EnTete),1,f->fichier);//lire l'entete et la charger dand l'entete
    }
}
}


void Fermer(LOV* f){
fseek(f->fichier,0,SEEK_SET); //positionner le curseur au debut de f
fwrite(&(f->entete), sizeof(EnTete),1,f->fichier); //sauv de lentete
fclose(f->fichier); //fermer le f
}

void AllocBloc(LOV* f){ //allouer un nb bloc
 Buffer* buf=malloc(sizeof(Buffer));//allouer un espace pr le buf
 LireDir(f,Entete(f,5),buf);//lire le dernier bloc
 buf->svt=Entete(f,4)+1;//affecter svt sur le nv bloc
 int c= Entete(f,5);
 EcrireDir(f,c,&buf); //ecrire le dernier bloc
 strcpy(buf->tab, ""); //initialiser les info
 buf->svt=-1; //initialiser le bloc svt a -1
 c=Entete(f,4)+1;
 EcrireDir(f,c,buf); //ecrire le buf
 aff_entete(f,4,Entete(f,4)+1); //inc le nb total de blocs
 aff_entete(f,5,Entete(f,4)); //maj du dernier bloc;
 }

void initialiserD(donnee* d){
strcpy(d->matricule, "||||||||||||");
strcpy(d->nom, "||||||||||||||");
strcpy(d->prenom, "|||||||||||");
d->taille[0]='0';
 for(int i=1;i<10;i++){
    d->taille[i]='|';
    d->eff='0';
 }
}

char* concat(donnee d){//concatene les champs en une seule chaine
    char* ch;
    ch=(char*)malloc(sizeof(char)*(37+10+strlen(d.universite)));
    int j=0;
    for(int i=0;i<10;i++){
        if(d.taille[i]!='\0')
            ch[j]=d.taille[i];
        else
            ch[j]='|';
        j+=1;
    }
    ch[j]=d.eff; //Le champ effac�
    j+=1;
    for(int i=0;i<10;i++) //Le champ matricule
    {
        if(d.matricule[i]!='\0')
            ch[j]=d.matricule[i];
        else
            ch[j]='|';
        j+=1;
    }
    for(int i=0;i<14;i++) //Le champ nom
    {
        if(d.nom[i]!='\0')
            ch[j]=d.nom[i];
        else
            ch[j]='|';
        j+=1;
    }
    ch[j]=d.annee; //Le champ annee
    j+=1;
    for(int i=0;i<10;i++) //Le champ prenom
    {
        if(d.prenom[i]!='\0')
            ch[j]=d.prenom[i];
        else
            ch[j]='|';
        j+=1;
    }
    for(int i=0;i<strlen(d.universite);i++) //Le champ universite
    {
        ch[j]=d.universite[i];
        j+=1;
    }
    ch[j] = '\0'; //Le dernier carat�re de la chaine
    return ch; //Retourner la chaine conact�n�e
}


void RecupChamp(LOV* f,int n,Buffer* buf,int* i,int* j,char* donnee)//Recup�re le champ de l'enregistrement � partir du fichier
{
    int k = 0;
    while(k<n)
    {
        if(*j>=100) //En cas o� le champ est divis� sur plus d'un bloc
        {
            *i = buf->svt;
            *j = 0; //R�nitiliser le j � 0 (au d�but du nouveau bloc)
            LireDir(f,*i,buf); //Lire le bloc suivant
        }

        if(buf->tab[*j]!='|') //Si le caract�re n'est pas un s�parateur
        {
            donnee[k] = buf->tab[*j];
        }
        else //Sinon mettre des caract�res vides
        {
            donnee[k] = '\0';
        }
        (*j)+=1;
        k+=1;
    }
}

void RecupChaine(LOV* f,int n,Buffer* buf,int* i,int* j,char donnee[])
{
    int k = 0;
    while(k<n)
    {
        if(*j>=100) //En cas où la chaine est divisée sur plus d'un bloc
        {
            *i = buf->svt;
            *j = 0; //Rénitiliser le j à 0 (au début du nouveau bloc)
            LireDir(f,*i,buf);
        }

        donnee[k] = buf->tab[*j];
        (*j)+=1;
        k+=1;
    }
    donnee[k] = '\0';
}


char* lectured(int matricule)
{
    donnee d; //D�clarer d comme enregistrement
    initialiserD(&d); //Initialiser les champs de d
    sprintf(d.matricule,"%d",matricule); //Affectation
    printf("\t\t\tEntrez le nom : ");
    scanf("%s",&d.nom); //Lire le champ nom
    printf("\t\t\tEntrez votre annee L(licence) ou M(master) : ");
    scanf(" %c",&(d.annee)); //Lire le champ annee
    printf("\t\t\tEntrez votre prenom : ");
    scanf("%s",&d.prenom); //Lire le champ prenom
    printf("\t\t\tEntrez votre universite (sans espaces): ");
    d.universite = (char*)malloc(sizeof(char)*250);
    scanf("%s",d.universite); //Lire le champ universite
    sprintf(d.taille,"%d",10+1+35+strlen(d.universite));
    return concat(d); //Retouner les champs concat�n�s
}


void recherche(LOV* f,char* nom,int val,int* i,int* j,int* trouv)
{
    Buffer buf; //Pour lire et �crire les blocs
    int stop; //Pour stopper la boucle dans certaines conditions
    int sauvi; //Pour sauvegarder i
    int sauvj; //Pour sauvegarder j
    char taille[10];
    char matricule[10];
    char eff;
    char* d; //La chaine de carat�res d'un enregistrement apr�s le champ taille, eff et mat
    int nb_taille,nb_mat; //Les champs taille et matricule en entiers

    ouvrir(f,nom,'A'); //Ouvrir le fichier
    *trouv = 0; //Positionner trouv � Faux
    stop = 0; //Positionner stop � Faux
    *i = Entete(f,1);
    *j = 0;
    LireDir(f,*i,&buf); //Lire le premier bloc

    while((*trouv == 0)&&(stop == 0)) //Tant qu'on a pas trouv� le livret et que aucune condition n'a �t� v�rifi�e pour stopper la boucle
    {
        sauvi = *i; //Sauvegarder i
        sauvj = *j; //Sauvegarder j
        RecupChamp(f,10,&buf,i,j,taille); //Recup�rer le champ taille (le i et j sont mis � jour apr�s le champ)
        nb_taille = atoi(taille); //R�cup�rer le champ en tant qu'entier
        RecupChamp(f,1,&buf,i,j,&eff); //R�cup�rer le champ eff
        RecupChamp(f,10,&buf,i,j,matricule); //R�cup�rer le champ num�ro
        nb_mat = atoi(matricule); //R�cup�rer le champ en tant qu'entier
        d = malloc(sizeof(char)*(nb_taille-10-10));
        RecupChaine(f,nb_taille-10-11,&buf,&i,&j,&d); //R�cup�rer le reste de l'enregistrement

        if(nb_mat == val) //Le meme num�ro a �t� trouv�
        {
            if(eff == '0')
                *trouv = 1; //trouv � Vrai
            else
                stop = 1;
            *i = sauvi; //R�cup�rer i avant d'avoir lu cet enregistrement
            *j = sauvj; //R�cup�rer j avant d'avoir lu cet enregistrement
        }
        else
        {
            if(nb_mat>val) //Si la valeur recherch�e est inf�rieure � la valeur lue il ne sert � rien de continuer la recherche donc il faut la stopper
            {
                stop = 1; //stop � Vrai
                *i = sauvi; //R�cup�rer i avant d'avoir lu cet enregistrement
                *j = sauvj; //R�cup�rer j avant d'avoir lu cet enregistrement
            }
        }
       if(buf.svt != -1){ //verifier si c le dernier bloc
        *i=buf.svt; //sinon on avence au bloc suivant
        *j=0; //initialiser lindice du debut du bloc
        LireDir(f, i, &buf); //lire le bloc svt
       }
        free(d); //On lib�re l'espace
    }
    Fermer(f); //Fermer le fichier
}

void creation(LOV* f,char* nom,int nb_etudiant, int matricule)
{
    int i=1; //Se positionner au d�but du fichier
    int j=0;
    Buffer buf;
    ouvrir(f,nom,'N'); //Ouvrir le fichier en mode nouveau
    AllocBloc(f); //Allouer le premier bloc du fichier
    for(int k=0;k<nb_etudiant;k++) //Ins�rer les livrets un par un
    {
        LireDir(f,i,&buf); //Lire le bloc
        char* ch=lectured(matricule);
        int l= 0;
        while(l<strlen(ch)) //Ins�rer la donn�e caract�re par caract�re
        {
            if(j<100) //Si la position est inf�rieure � la taille du bloc
            {
                buf.tab[j] = ch[l]; //Ins�rer le caract�re
                l+=1;
                j+=1;
            }
            else //Si le caract�re doit s'ins�rer dans le bloc suivant
            {
                j=0;
                EcrireDir(f,i,&buf); //On �crit le bloc
                AllocBloc(f); //Alloue un nouveau bloc
                i=Entete(f,5); //Met � jour l'adresse i
                LireDir(f,i,&buf); //Lire le nouveau bloc
            }
        }
        EcrireDir(f,i,&buf); //Ecrit le dernier bloc
        aff_entete(f,2,Entete(f,2)+strlen(ch)); //Met � jour le nombre de caract�res ins�r�s dans l'ent�te
    }
    Fermer(f);
}


void insertion(LOV* f,char* nom,int matricule,char* donnee)
{
    Buffer buf;
    int trouv;
    int stop = 0;
    int i = 1;
    int j = 0;
    int l = 0;
    int rest;
    recherche(f,nom,matricule,&i,&j,&trouv); //On effectue une recherche pour avoir l'adresse i et la position j o� ins�rer
    if(trouv == 0) //Si le num�ro n'existe pas d�j� dans le fichier
    {
        ouvrir(f,nom,'A'); //Ouvrir le fichier en mode ancien
        int sauvtaille = strlen(donnee); //On sauvegarde la taille du nouveau enregistrement
        aff_entete(f,2,Entete(f,2)+sauvtaille); //Mettre � jour le champ (nombre de caract�res ins�r�s) de l'ent�te
        char* d = (char*)malloc((sauvtaille)*sizeof(char)); //Allouer un espace m�moire de la m�me taille que le nouveau enregistrement
        while(stop == 0) //Tant que l'insertion n'est pas finie
        {
            LireDir(f,i,&buf); //Lire le bloc � l'adresse i
            if(j+sauvtaille<=100) //Si il y a assez de place pour l'enregistrement dans le bloc courant
            {
                for(int k=0;k<sauvtaille;k++)
                {
                    d[k] = buf.tab[j+k]; //Sauvegarder les caract�res qui se trouvaient d�j� dans la position o� on veut ins�rer
                    buf.tab[j+k] = donnee[k]; //Ins�rer un carat�re du nouveau enregistrement
                }
                d[sauvtaille] = '\0';
                EcrireDir(f,i,&buf); //Ecrire le bloc
                j+=sauvtaille; //Faire avancer la position
                strcpy(donnee,d); //Copier les caract�res sauvegard�s pr�c�demment dans la donn�e courante qu'on veut ins�rer
            }
            else //Si il n'y a pas assez de place pour l'enregistrement dans le bloc courant
            {
                rest = j+sauvtaille - 100; //Nombre de caract�res qu'on ins�rera dans le bloc suivant
                l = 0;
                while(j<100) //Ins�rer les caract�res jusqu'� la fin du bloc
                {
                    d[l] = buf.tab[j];
                    buf.tab[j] = donnee[l];
                    j++;
                    l++;
                }
                EcrireDir(f,i,&buf); //Ecrire le bloc
                i = buf.svt; //Passer au prochain bloc
                j=0;
                if(i == -1) //Si le bloc courant �tait le dernier bloc
                {
                    AllocBloc(f); //On alloue un nouveau bloc
                    i = Entete(f,5); //Mettre � jour le champ (adresse de la queue) de l'ent�te
                    stop = 1; //On stop l'insertion car on est arriv� au dernier bloc
                }
                LireDir(f,i,&buf); //Lire le nouveau bloc
                for(int k=0;k<rest;k++) //Ins�rer les caract�res dans ce nouveau bloc
                {
                    d[l] = buf.tab[j];
                    buf.tab[j] = donnee[l];
                    j++;
                    l++;
                }
                d[strlen(donnee)] = '\0';
                EcrireDir(f,i,&buf); //Ecrire le bloc
                strcpy(donnee,d); //Copier les caract�res sauvegard�s pr�c�demment dans la donn�e courante qu'on veut ins�rer
            }
            if(buf.svt = -1) //Si on arrive au dernier bloc et � la derni�re position
            {
                stop = 1; //On stoppe l'insertion
            }
        }
        free(d);
        Fermer(f);
    }
    else //Si le num�ro existe d�j� dans le fichier
    {
        printf("\t\t\tCe matricule existe deja\n");
    }
}

void SuppressionLogique(LOV* f,char* nom,int matricule)
{
    int trouv;
    int i,j;
    int nb_taille;
    Buffer buf;
    char taille[10];
    recherche(f,nom,matricule,&i,&j,&trouv); //On effectue d'abord une recherche
    ouvrir(f,nom,'A'); //On ouvre le fichier en mode ancien
    LireDir(f,i,&buf); //On lie le bloc
    RecupChamp(f,10,&buf,&i,&j,taille); //On r�cup�re le champ taille
    nb_taille = atoi(taille); //On le convertit en entier
    if(trouv == 1)
    {
        if(j<100) //la position du champ eff est dans le bloc courant
        {
            buf.tab[j] = '1'; //On met � jour le champ � 1 ce qui signifie que la donn�e a �t� effac�
            EcrireDir(f,i,&buf); //Ecrire le bloc
        }
        else //la position du champ eff est dans le bloc suivant
        {
            i = buf.svt; //On passe au bloc suivant
            if(i == -1) //S'il n'existe pas c'est une erreur de conception
            {
                printf("\t\t\tERREUR: Le champ efface n'existe pas\n");
            }
            else
            {
                LireDir(f,i,&buf); //On lie ce bloc
                buf.tab[100-j] = '1'; //On met � jour le champ � 1 ce qui signifie que la donn�e a �t� effac�
                EcrireDir(f,i,&buf); //On �crit le bloc
            }
        }
        aff_entete(f,3,Entete(f,3)+nb_taille); //On met � jour le champ (nombre de caract�res supprim�s) de l'ent�te
    }
    else 
    {
        printf("\t\t\tLe champs d'etudiant que vous voulez supprimer n'existe pas\n");
    }

    Fermer(f);
}

#endif


