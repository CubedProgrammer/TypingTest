#include<stdio.h>
#include"load.h"
int main(int argl, char *argv[])
{
	int succ = findload();
	if(succ == 0)
	{
		fputs("Starting in 5", stdout);
	}
	else
		fputs("Could not load file containing the words, check if the file ttdict.txt exists in the working directory, and that you have permission to read it.\n", stderr);
	return succ;
}
