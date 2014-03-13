/**Copyright (c) 2011-2014, Taiki
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of Mavy nor the names of its
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

int worker(char * basePath, char * archiveName, bool askConfirm, bool verbose)
{
    FILE* config = NULL;
	uint nbElem, length, i;
	uint64_t lengthInput = strlen(basePath);
	bool keepRunning = false, jpgOnly;
	char path[2][lengthInput + MAX_PATH], *localString;
	RPS *basePtr = malloc(sizeof(RPS)), *curPtr = basePtr;	//We have a small logic issue that will make us allocate one extra entry, but no leak, so not critical
	

    //Initialisateurs pour le renommage de chapitre
    struct dirent *ent;
    DIR *dir;

	do
	{
		dir = opendir (basePath);
		if (dir != NULL)
		{
			nbElem = 0;
			jpgOnly = true;

			while((ent = readdir (dir)) != NULL)
			{
				if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
					continue;
				
				if(curPtr == NULL)
				{
					printf("Memory error, abort D:");
					exit(EXIT_FAILURE);
				}

				localString = curPtr->name;					//This prevent the CPU to dereference the ptr each time
				strncpy(localString, ent->d_name, MAX_PATH);
				
				for(length = 0; localString[length]; length++) //On compte le nombre de caractère et on en profite pour virer la casse
				{
					if(localString[length] >= 'A' && localString[length] <= 'Z')
						localString[length] = localString[length] - ('A' - 'a');
				}
				
				if((length < 4 || localString[length-4] != '.') && (length < 5 || localString[length-5] != '.'))	//Check ext (4 = x.jpg/png, 5 = x.jpeg)
					continue;
				
				if(localString[length-4] == '.')
				{
					if(!strcmp(&localString[length-3], "png"))
					{
						curPtr->isPng = true;
						jpgOnly = false;
					}
					else if(!strcmp(&localString[length-3], "jpg"))
						curPtr->isPng = false;
					else
						continue;
				}
				else
				{
					if(!strcmp(&localString[length-4], "jpeg"))
						curPtr->isPng = false;
					else
						continue;
				}
				
				curPtr->next = malloc(sizeof(RPS));
				curPtr = curPtr->next;
				nbElem++;
			}
			if(curPtr != NULL)
				curPtr->next = NULL;
						
			closedir (dir);
			
			if (nbElem)
			{
				RPS* buf[nbElem];
				for (i = 0, curPtr = basePtr; curPtr->next != NULL; curPtr = curPtr->next, i++)
					buf[i] = curPtr;
				
				printf("%p - %lu - %s\n", buf[0], sizeof(buf[0]), buf[0]->name);
				qsort(buf, i, sizeof(RPS*), compare);
				
				for (i = 0, basePtr = curPtr; i < nbElem; curPtr = curPtr->next, i++)
				{
					curPtr = buf[i];
					puts(curPtr->name);
				}
				puts("Ok");
				exit(0);
				
				curPtr = buf[i];
				curPtr->next = NULL;
				
				if(verbose)
				{
					puts("Fichiers classe:\n");
					for(curPtr = basePtr,  i = 0; curPtr; i++, curPtr = curPtr->next)
						printf("%d. %s\n", i+1, curPtr->name);
					
					if(askConfirm)
					{
						puts("\n\nConfirmez vous? 1. Oui, 2. Non\n");
						
						char c = getCh();
						
						if(c == '2')
						{
							puts("Tapez enter quand les modifications seront termines\n");
							getchar();
							putc('\n', stdout);
							keepRunning = true;
						}
						else
							keepRunning = false;
					}
					else
						puts("\n\n");
				}
				
				for(i = 0, curPtr = basePtr; curPtr; i++, curPtr = curPtr->next)
				{
					snprintf(path[0], sizeof(path), "%s/%s", basePath, curPtr->name);
					
					if(curPtr->isPng)
						snprintf(path[1], sizeof(path), "%s/%d.png", basePath, i);
					else
						snprintf(path[1], sizeof(path), "%s/%d.jpg", basePath, i);
					
					rename(path[0], path[1]);
					strncpy(curPtr->name, &path[1][lengthInput+1], sizeof(curPtr->name));
				}
			}
		}
		else
			nbElem = jpgOnly = 0;
		
	} while (askConfirm && keepRunning);
	
	if(!nbElem)
	{
		puts("Aucune page valide trouvée :/\n");
		return 0;
	}
	
	snprintf(path[0], sizeof(path[0]), "%s/config.dat", basePath);
	config = fopen(path[0], "w+");
	
	if(config == NULL)
	{
		puts("Failed at open config file");
		return 0;
	}
	
	if(jpgOnly)
        fprintf(config, "%d", nbElem-1);

	else
	{
		fprintf(config, "%d\nN", nbElem);
		for(curPtr = basePtr, i = 0; curPtr; curPtr = curPtr->next, i++)
			fprintf(config, "\n%d %s", i+1, curPtr->name);
	}
	
	fclose(config);

    
	if(verbose)
	{
		puts("Renommage termine. Compression prete au lancement.\n");
		printf("L'archive sera nomee %s. Remplacez Pr par \n", archiveName);
	}
	
	for (curPtr = basePtr; curPtr->next; curPtr = curPtr->next)	//On va à la fin de la liste
	{
		puts(curPtr->name);
	}
	puts("\n\n");
	
	curPtr->next = malloc(sizeof(RPS));								//On ajoute le config.dat à la liste
	if(curPtr->next == NULL)
	{
		printf("Memory error, abort D:");
		exit(EXIT_FAILURE);
	}
	
	curPtr = curPtr->next;
	strncpy(curPtr->name, "config.dat", sizeof(curPtr->name));

	snprintf(path[0], sizeof(path[0]), "%s/%s", basePath, archiveName);	//On modifie le path de l'archive
	zip(basePath, basePtr, path[0]);
	
	void* buf;
	for (curPtr = basePtr; curPtr; curPtr = buf)
	{
		buf = curPtr->next;
		free(curPtr);
	}
	
	printf("\nTermine.");
	return 0;
}

