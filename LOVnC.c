#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main()
{
    LOV_C *fichier;
    srand(time(0));
    int choix;
    int numeroBloc;
    int trouve, i, j;
    int cle;
    char Etudiant[1000];
    int taille;
    int fin=0;
    int nbloc;
    while(fin == 0)
    {
        printf("MENU:\n");
    	printf("1) Creation du fichier 'Etudiant.bin'\n");
    	printf("2) Rechercher un etudiant dans le fichier 'Etudiant.bin'\n");
    	printf("3) Inserer un etudiant dans le fichier 'Etudiant.bin'\n");
    	printf("4) Afficher un bloc dans le fichier 'Etudiant.bin'\n");
        printf("Veuillez entrer votre choix: ");
    	scanf("%d", &choix);

switch(choix) {
    		case 1:{
    			printf("creation du fichier 'Etudiant.bin'\n");
    			creation(&fichier);
    			printf("\nCreation avec succes\n");
    			break;}
    		case 2:{
                printf("entrer un identifiant:\n");
    			scanf("%d", &cle);
    			ouvrir(&fichier, "Etudiant.bin", 'A');
    			recherche(fichier, cle, &trouve, &i, &j);
    			fermer(fichier);
    			printf("TROUVE: ");
    			printf("%s\n", trouve == 1 ? "OUI" : "NON");
    			if(trouve==1){
    			printf("Numero du bloc: ");
    			printf("%d\n",i);
    			printf("Position dans le bloc: ");
    			printf("%d\n",j);}
    			else{
                printf("L'etudiant que vous cherchiez n'existe pas, veuillez l'inserer\n");
    			}
    			break;}
    		case 3:{
                lire(Etudiant, &taille);
    			ouvrir(&fichier, "Etudiant.bin", 'A');
    			insertion(fichier, Etudiant, taille);
    			fermer(fichier);
                break;}
            case 4:
                    printf("entrez le bloc que vous voulez afficher:\n");
                    scanf("%d", &nbloc);
                    affichage(&fichier, nbloc);
    			break;
            default:
                {
                    fin = 1;
                    break;
                }
}

        if(fin == 0)
        {
            printf("\n\t\t\tVoulez vous quitter?\n\t\t\t1-oui 2-non\n\t\t\tChoix : ");
            scanf("%d",&choix);
            if(choix == 1)
                fin = 1;
        }
    }
    return 0;
}
// LE LIEN DU GIT
//https://github.com/risemines/examensfsd.git
