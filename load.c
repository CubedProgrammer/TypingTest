#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#ifdef _WIN32
#else
#include<sys/stat.h>
#endif
int loaddict(const char *fname, char ***dictp, size_t len)
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
				size_t lns = 0;
				for(const char *it = cont; it != cont + fsz; ++it)
					lns += *it == '\n';
				size_t totlen = fsz - lns;
				lns += cont[fsz - 1] != '\n';
				char **dict = malloc(totlen * sizeof(**dict) + lns * sizeof(*dict));
				if(dict)
				{
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
						cont += dictsz - fsz;
						fseek(fh, 0, SEEK_SET);
						fread(cont , 1, fsz, fh);
						size_t pushcnt = 0;
						for(int64_t i = fsz - 1; i >= 0; --i)
						{
							if(cont[i] == '\n')
								++pushcnt;
							else if(pushcnt > 0)
								cont[i + pushcnt] = cont[i];
						}
					}
					else
						succ = -1;
				}
				fclose(fh);
			}
			else
				succ = -1;
		}
		else
			succ = -1;
	}
	return succ;
}

int findload(void)
{
	return 0;
}
