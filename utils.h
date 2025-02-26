#include <stdio.h>
#include <stdlib.h>

#define errcode int
#define _NO_ERR 0 

char **split(const char *, const char);
void freeSplitted(char **);
