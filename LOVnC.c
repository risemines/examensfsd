#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


int main()
{
    int choix;
    int fin = 0;
    LOV f;
    FILE* test;
    while(fin == 0)
    {
        system("cls");
        printf("\n\n\t\t\t\tBienvenue dans notre programme\n\n\n");
        printf("\t\t\t1-Creation d'un etudiant\n");
        printf("\t\t\t2-Recherche d'un etudiant\n");
        printf("\t\t\t3-Insertion d'un nouveau etudiant\n");
        printf("\t\t\t4-Supprimer un etudiant (Suppresion logique)\n");
        printf("\t\t\t5-Quitter\n\n");
        printf("\t\t\tVotre choix : ");
        scanf("%d",&choix);

        switch(choix)
        {
            case 1:
                {
                    system("cls");
                    int nb_etudiant, matricule;
                    printf("\n\n\t\t\tVeuillez entrer le nombre d'etudiants : ");
                    scanf("%d",&nb_etudiant);
                    printf("\n\n\t\t\tVeuillez entrer le matricule du 1er etudiant : ");
                    scanf("%d", &matricule);
                    creation(&f,"Etudiants.bin",nb_etudiant, matricule);
                    printf("\n\t\t\tContenu de l'entete : ");
                    printf("\n\t\t\tAdresse du premier bloc : %d",Entete(&f,1));
                    printf("\n\t\t\tNombre de caracteres inseres : %d",Entete(&f,2));
                    printf("\n\t\t\tNombre de caracteres supprimes : %d",Entete(&f,3));
                    printf("\n\t\t\tNombre total de blocs : %d",Entete(&f,4));
                    printf("\n\t\t\tAdresse de la queue de la liste : %d",Entete(&f,5));
                    printf("\n\t\t\tOperation effectuee avec succes\n");
                    break;
                }
            case 2:
                {
                    system("cls");

                    //Tester si le fichier existe
                    if(test = fopen("Etudiants.bin","rb"))
                    {
                        fclose(test);

                        int matricule,i,j,trouv;
                        int nb_taille;
                        Buffer buf;
                        char taille[10];
                        char eff;
                        char ch_mat[10];
                        char nom[14];
                        char annee;
                        char prenom[10];
                        char* universite;
                        printf("\n\n\t\t\tVeuillez entrer le matricule de l'etudiant que vous recherchiez : ");
                        scanf("%d",&matricule);
                        recherche(&f,"Etudiants.bin",matricule,&i,&j,&trouv);
                        if(trouv == 0)
                        {
                            printf("\n\t\t\tLe matricule que vouz avez entre n'existe pas\n");
                            printf("\n\t\t\tLe matricule doit se trouve dans le bloc %d a la position %d si on veut l'inserer\n",i,j);
                        }
                        else
                        {
                            printf("\n\t\t\tL'etudiant se trouve dans le bloc %d a la position %d\n",i,j);
                            Ouvrir(&f,"Etudiants.bin",'A');
                            LireDir(&f,i,&buf);
                            RecupChamp(&f,10,&buf,&i,&j,taille);
                            nb_taille = atoi(taille);
                            universite = (char*)malloc(sizeof(char)*(nb_taille-10-36+1));
                            RecupChamp(&f,1,&buf,&i,&j,&eff);
                            RecupChamp(&f,10,&buf,&i,&j,ch_mat);
                            RecupChamp(&f,14,&buf,&i,&j,nom);
                            RecupChamp(&f,1,&buf,&i,&j,&annee);
                            RecupChamp(&f,10,&buf,&i,&j,prenom);
                            RecupChaine(&f,nb_taille-10-36,&buf,&i,&j,universite);
                            printf("\n\t\t\tInformations sur cet etudiant : ");
                            printf("\n\t\t\tTaille : %s",taille);
                            printf("\n\t\t\tEfface : %c",eff);
                            printf("\n\t\t\tMatricule : %s",ch_mat);
                            printf("\n\t\t\tNom : %s",nom);
                            printf("\n\t\t\tPrenom: %s",prenom);
                            printf("\n\t\t\tAnnee : %c",annee);
                            printf("\n\t\t\tUniversite : %s\n",universite);
                        }
                    }
                    else
                    {
                        printf("\n\n\t\t\tLe fichier n'existe pas, veuillez d'abord le creer\n");
                    }

                    break;
                }
            case 3:
                {
                    system("cls");
                    int matricule;
                    if(test = fopen("Etudiants.bin","rb"))
                    {
                        fclose(test);
                    }
                    else
                    {
                        Ouvrir(&f,"Etudiants.bin",'N');
                        Fermer(&f);
                    }
                    printf("\n\n\t\t\tVeuillez entrer le matricule de l'etudiant que vous voulez inserer : ");
                    scanf("%d",&matricule);
                    insertion(&f,"Etudiants.bin",matricule,lectured(matricule));
                    printf("\n\t\t\tContenu de l'entete : ");
                    printf("\n\t\t\tAdresse du premier bloc : %d",Entete(&f,1));
                    printf("\n\t\t\tNombre de caracteres inseres : %d",Entete(&f,2));
                    printf("\n\t\t\tNombre de caracteres supprimes : %d",Entete(&f,3));
                    printf("\n\t\t\tNombre total de blocs : %d",Entete(&f,4));
                    printf("\n\t\t\tAdresse de la queue de la liste : %d",Entete(&f,5));
                    printf("\n\t\t\tOperation effectuee avec succes\n");
                    break;
                }
            case 4:
                {
                    system("cls");
                    int matricule;
                    if(test = fopen("Etudiants.bin","rb"))
                    {
                        fclose(test);
                        printf("\n\n\t\t\tVeuillez entrer le matricule de l'etudiant que vous voulez supprimer : ");
                        scanf("%d",&matricule);
                        SuppressionLogique(&f,"Etudiants.bin",matricule);
                        printf("\n\t\t\tContenu de l'entete : ");
                        printf("\n\t\t\tAdresse du premier bloc : %d",Entete(&f,1));
                        printf("\n\t\t\tNombre de caracteres inseres : %d",Entete(&f,2));
                        printf("\n\t\t\tNombre de caracteres supprimes : %d",Entete(&f,3));
                        printf("\n\t\t\tNombre total de blocs : %d",Entete(&f,4));
                        printf("\n\t\t\tAdresse de la queue de la liste : %d",Entete(&f,5));
                        printf("\n\t\t\tOperation effectuee avec succes\n");
                    }
                    else
                    {
                        printf("\n\n\t\t\tLe fichier n'existe pas, veuillez d'abord le creer au choix 1\n");
                    }

                    break;
                }
            default:
                {
                    fin = 1;
                    break;
                }
        }
        if(fin == 0)
        {
            printf("\n\t\t\tVoulez vous quitter?\n\t\t\t1-Oui 2-Non\n\t\t\tChoix : ");
            scanf("%d",&choix);
            if(choix == 1)
                fin = 1;
        }
    }
    return 0;
}
