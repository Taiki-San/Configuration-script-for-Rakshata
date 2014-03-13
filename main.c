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

char REPERTOIREEXECUTION[250];

void printHelp()
{
	puts("Help:\n");
	puts("-help		|		print this help");
	puts("-batch		|		will assume every directory in the current directory contain a chapter, and manage them all at once");
	puts("-prefix X	|		change the default prefix (Pr) to the one provided afterward (X here)");
	puts("-AC		|		will ask a confirmation before renaming files and creating the zipfile");
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
	getcwd(REPERTOIREEXECUTION, sizeof(REPERTOIREEXECUTION));
#else
    strncpy(REPERTOIREEXECUTION, argv[0], sizeof(REPERTOIREEXECUTION));
	for(uint64_t i = strlen(REPERTOIREEXECUTION); REPERTOIREEXECUTION[i] != '/'; REPERTOIREEXECUTION[i--] = 0);
	chdir(REPERTOIREEXECUTION);
#endif

	bool batchMode = false, askConfirm = false;
	char prefixArchive[50] = "Pr";
	
	for(int pos = 1; pos < argc; pos++)
	{
		if(!strcmp(argv[pos], "-batch"))
			batchMode = true;
		
		else if(!strcmp(argv[pos], "-prefix") && pos+1 < argc)
			strncpy(prefixArchive, argv[++pos], 50);
		
		else if(!strcmp(argv[pos], "-AC"))
			askConfirm = true;
		
		else if(!strcmp(argv[pos], "-help"))
		{
			printHelp();
			return 0;
		}
	}

	if(batchMode)
	{
		DIR *directory, *test;			//directory pointer
		struct dirent *entry;			//a single directory entry
		char basePath[0x100];
		char archiveName[0x180];
		
		mkdir("zip_files", 0750);
		directory = opendir(".");
		
		while ((entry = readdir(directory)) != NULL)
		{
			if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..") || !strcmp(entry->d_name, "zip_files"))
				continue;
			
			if((test = opendir(entry->d_name)) != NULL)	//Directory is accessible
			{
				closedir(test);
				strncpy(basePath, entry->d_name, sizeof(basePath));
				snprintf(archiveName, sizeof(archiveName), "../zip_files/%s_Chapitre_%s.zip", prefixArchive, entry->d_name);
				worker(basePath, archiveName, askConfirm, askConfirm);
			}
		}
		closedir(directory);

	}
	else
	{
		char archiveName[0x100];
		snprintf(archiveName, sizeof(archiveName), "%s_Chapitre_X.zip", prefixArchive);
		worker(".", archiveName, askConfirm, askConfirm);
	}

    return 0;
}


