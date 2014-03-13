/**Copyright (c) 2013, Taiki
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.**/

#include "main.h"

int configurationEco()
{
    int i = 1, j = 0, nombrePagePng = 0, verificationExtansion = 0, nombrePage = 0;
    int pagesPNG[500];
    char temp[100], nomPagesRenommage[500][100], path[500];
    FILE* config = NULL;

    for(i = 0; i < 500; nomPagesRenommage[i++][0] = 0);

    getcwd(path, 500);

    //Initialisateurs pour le renommage de chapitre
    struct dirent *ent;
    DIR *dir;

#ifdef FAST
start:
#endif

    dir = opendir (path);

    pagesPNG[0] = -1;

    if (dir != NULL)
    {
        for (i = 0; (ent = readdir (dir)) != NULL; i++)
        {
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            {
                i--;
                continue;
            }
            sprintf (nomPagesRenommage[i], ent->d_name);

            for(j = 0; nomPagesRenommage[i][j] != 0; j++) //On compte le nombre de caractËre et on en profite pour virer la casse
            {
                if(nomPagesRenommage[i][j] >= 'A' && nomPagesRenommage[i][j] <= 'Z')
                    nomPagesRenommage[i][j] = nomPagesRenommage[i][j] - ('A' - 'a');
            }

            for(verificationExtansion = j; j - verificationExtansion <= 5 && nomPagesRenommage[i][verificationExtansion] != '.' && verificationExtansion > 0; verificationExtansion--); //Si pas d'extansion

            if(nomPagesRenommage[i][verificationExtansion] == '.') //VÈrifie que le format est supportÈ
            {
                if(nomPagesRenommage[i][++verificationExtansion] == 'p' || nomPagesRenommage[i][verificationExtansion] == 'j')
                {
                    if(nomPagesRenommage[i][++verificationExtansion] == 'n' || nomPagesRenommage[i][verificationExtansion] == 'p')
                    {
                        if(nomPagesRenommage[i][++verificationExtansion] == 'g')
                        {
                            if(nomPagesRenommage[i][verificationExtansion - 2] == 'p') //On vÈrifie la premiËre lettre de l'extansion
                            {
                                pagesPNG[i]= 1;
                                nombrePagePng = 1;
                                verificationExtansion = -1;
                            }
                            else if (nomPagesRenommage[i][verificationExtansion - 2] == 'j') //On vÈrifie la premiËre lettre de l'extansion
                            {
                                pagesPNG[i]= 0;
                                verificationExtansion = -1;
                            }
                            pagesPNG[i + 1]= -1;
                        }
                    }
                }
                if(verificationExtansion == -1) //Format supportÈ
                    verificationExtansion = j;
                else                            //Sinon
                    verificationExtansion = 0;
            }

            if(nomPagesRenommage[i][0] == '.' || (j - verificationExtansion > 5 || verificationExtansion == 0))
            {
                for(j = 0; j < 100; j++)
                    nomPagesRenommage[i][j] = 0;
                i--;
            }
        }
        closedir (dir);

        qsort(nomPagesRenommage, i, sizeof(nomPagesRenommage[0]), compare);

#ifndef MODE_RAPIDE
        printf("Fichiers classe:\n");
        for(i = 0; nomPagesRenommage[i][0]; i++)
            printf("%d. %s\n", i+1, nomPagesRenommage[i]);

        printf("\n\nConfirmez vous? 1. Oui, 2. Non\n");
#ifndef FAST
        #ifdef _WIN32 //Récupère la lettre sans avoir à presser enter
            char c = 0;
            while(c != '1' && c != '2')
                c = GetCh();
        #else
            system ("/bin/stty raw");
            char c = 0;
            while(c != '0' && c != '1')
                c = getchar();
            system ("/bin/stty cooked");
        #endif

        if(c == '2')
        {
            printf("Tappez enter quand les modifications seront termines\n");
            getchar();
            printf("\n");
            goto start;
        }
#endif
#endif

        for(i = 0; nomPagesRenommage[i][0] != 0; i++)
        {
            crashTemp(temp, 100);
            if(pagesPNG[i]) //Si c'est un png
                sprintf(temp, "%d.png", i);
            else //Si c'est un jpg
                sprintf(temp, "%d.jpg", i);
            rename(nomPagesRenommage[i], temp);
        }
    }

    nombrePage = i;

    if(nombrePage > 5000)
        printf("Trop de page o_o Vous aurez à compresser vous même\n");

    char nomFichiers[5001][100];

    //Ecriture de config.dat
    if(nombrePagePng) //Si il y a des png
    {
        config = fopen("config.dat", "w+");
        fprintf(config, "%d\nN", nombrePage);
        for(i = 0; i < nombrePage; i++)
        {
            if(pagesPNG[i])
            {
                fprintf(config, "\n%d %d.png", i + 1, i);
                sprintf(nomFichiers[i], "%d.png", i);
            }
            else
            {
                fprintf(config, "\n%d %d.jpg", i + 1, i);
                sprintf(nomFichiers[i], "%d.jpg", i);
            }
        }
        fclose(config);
    }
    else
    {
        config = fopen("config.dat", "w+");
        fprintf(config, "%d", nombrePage - 1);
        fclose(config);

        for(i = 0; i < nombrePage; i++)
            sprintf(nomFichiers[i], "%d.jpg", i);

    }

    if(nombrePage <= 5000)
    {
#ifndef MODE_RAPIDE
        printf("Renommage termine. Compression prete au lancement.\n");
        printf("L'archive sera nomee Pr_Chapitre_X.zip. Remplacez Pr par \n");
        printf("le nom du projet et X par le chapitre quand ce sera termine.\n");
#endif

        sprintf(nomFichiers[i++], "config.dat");
        nomFichiers[i][0] = 0;

        zip(nomFichiers, "Pr_Chapitre_X.zip");

        printf("\nTermine.");
    }

#ifndef MODE_RAPIDE
    printf("\n\nDone o/");
    #ifdef _WIN32 //Récupère la lettre sans avoir à presser enter
        GetCh();
    #else
        system ("/bin/stty raw");
        getchar();
        system ("/bin/stty cooked");
    #endif
#endif
    return 0;
}

