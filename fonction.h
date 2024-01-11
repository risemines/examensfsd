//----------------------- focntion de suppression lohgique dans le fichier--------------------------------------------//
void suppression_logique_L7OVC(L7OVC *fichier, int cle)
{
    int i,j,trouv;
    Buffer buf;
    char *chaine=malloc(sizeof(char)*3);
    recherche_L7OVC(fichier,cle,&trouv,&i,&j); // recherche de la cle fdans le fichihre
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