#ifndef LOAD_H_
#define LOAD_H_
#include<stddef.h>
int findload(void);
int loaddict(const char *fname, char ***dictp, size_t len);
#endif
