#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<sys/stat.h>
#endif
#include"load.h"

char **cptt____dict;
size_t cptt____dictlen;

int findload(void)
{
	const char *fpath = "ttdict.txt";
	int succ = loaddict(fpath, &cptt____dict, &cptt____dictlen);
	if(succ)
#ifdef _WIN32
		succ = -1;
#else
	{
		char homedict[2601];
		strcpy(homedict, getenv("HOME"));
		strcat(homedict, "/.ttdict.txt");
		succ = loaddict(homedict, &cptt____dict, &cptt____dictlen);
	}
#endif
	return succ;
}

int loaddict(const char *fname, char ***dictp, size_t *len)
{
#ifdef _WIN32
	WIN32_FIND_DATAA fdat;
	HANDLE hand = FindFirstFileA(fname, &fdat);
	int succ = hand == INVALID_HANDLE_VALUE;
#else
	struct stat fdat;
	int succ = stat(fname, &fdat);
#endif
	if(succ == 0)
	{
#ifdef _WIN32
		size_t fsz = fdat.nFileSizeHigh;
		fsz << 32;
		fsz += fdat.nFileSizeLow;
#else
		size_t fsz = fdat.st_size;
#endif
		char *cont = malloc(fsz);
		if(cont)
		{
			FILE *fh = fopen(fname, "r");
			if(fh)
			{
				setvbuf(fh, NULL, _IONBF, 0);
				fread(cont, 1, fsz, fh);
				size_t lns = 0, totlen = fsz;
				for(const char *it = cont; it != cont + fsz; ++it)
					lns += *it == '\n';
				lns += cont[fsz - 1] != '\n';
				totlen += cont[fsz - 1] != '\n';
				char **dict = malloc(totlen * sizeof(**dict) + lns * sizeof(*dict));
				if(dict)
				{
					size_t cnt = 0;
					char *dictit = (char *)(dict + lns);
					dict[0] = dictit;
					for(const char *it = cont; it != cont + fsz; ++dictit, ++it)
					{
						if(*it == '\n')
						{
							++cnt;
							*dictit = '\0';
							dict[cnt] = dictit + 1;
						}
						else
							*dictit = *it;
					}
					if(cont[fsz - 1] != '\n')
						*dictit = '\0';
				}
				else
				{
					// if there is not enough memory, free cont, it's not really needed anymore
					free(cont);
					size_t dictsz = totlen * sizeof(**dict) + lns * sizeof(*dict);
					dict = malloc(dictsz);
					if(dict)
					{
						cont = (char*)dict;
						cont += lns * sizeof(*dict);
						fseek(fh, 0, SEEK_SET);
						fread(cont, 1, fsz, fh);
						size_t cnt = 0;
						dict[0] = cont;
						for(size_t i = 0; i < fsz; ++i)
						{
							if(cont[i] == '\n')
							{
								cont[i] = '\0';
								++cnt;
								dict[cnt] = cont + i + 1;
							}
						}
						if(cont[fsz - 1] != '\n')
							cont[fsz] = '\0';
					}
					else
						succ = -1;
				}
				fclose(fh);
				*dictp = dict;
				*len = lns;
			}
			else
				succ = -1;
		}
		else
			succ = -1;
	}
	return succ;
}
