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

char REPERTOIREEXECUTION[250];

int main(int argc, char *argv[])
{
#ifdef _WIN32
	getcwd(REPERTOIREEXECUTION, sizeof(REPERTOIREEXECUTION));
#else
    int i = 0;
    sprintf(REPERTOIREEXECUTION, argv[0]);
	for(i = strlen(REPERTOIREEXECUTION); REPERTOIREEXECUTION[i] != '/'; REPERTOIREEXECUTION[i--] = 0);
	chdir(REPERTOIREEXECUTION);
#endif

    printf(" -----------------------------------------\n");
    printf("*                                          *\n");
    printf("*     Bienvenue dans ce script d'import    *\n");
    printf("*        Script concu pour Rakshata        *\n");
    printf("*       Contenu dans le kit d'import       *\n");
    printf("*   Veuillez ne pas distribuer ce script   *\n");
    printf("*              Code par Taiki              *\n");
    printf("*        Libere le 05/03/2013 - 10h        *\n");
    printf("*                                          *\n");
    printf(" ------------------------------------------\n\n\n\n");

#ifdef MODE_RAPIDE
    DIR *directory, *test;           /* pointeur de répertoire */
    struct dirent *entry;     /* représente une entrée dans un répertoire. */
    directory = opendir(REPERTOIREEXECUTION);

    /* On boucle sur les entrées du dossier. */
    while ( (entry = readdir(directory)) != NULL )
    {
        if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")
           || !strcmp(entry->d_name, "zip_files") || !strcmp(entry->d_name, "zip") || !strcmp(entry->d_name, "obj"))
            continue;

        if((test = opendir(entry->d_name)) != NULL)
        {
            closedir(test);
            char temp[1000];
            snprintf(temp, 1000, "%s\\%s", REPERTOIREEXECUTION, entry->d_name);
            chdir(temp);

            configurationEco();
            snprintf(temp, 1000, "FT_Chapitre_%s.zip", entry->d_name);
            rename("Pr_Chapitre_X.zip", temp);

            snprintf(temp, 1000, "move \"%s\\%s\\FT_Chapitre_%s.zip\" \"%s\\zip_files\\\"", REPERTOIREEXECUTION, entry->d_name, entry->d_name, REPERTOIREEXECUTION);
            system(temp);
            chdir(REPERTOIREEXECUTION);

        }
    }
    closedir(directory);
#else
    configurationEco();
#endif
    while(1);
    return 0;
}


