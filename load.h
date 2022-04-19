#ifndef LOAD_H_
#define LOAD_H_
#include<stddef.h>
extern char **cptt____dict;
extern size_t cptt____dictlen;
int findload(void);
int loaddict(const char *fname, char ***dictp, size_t *len);
#endif
