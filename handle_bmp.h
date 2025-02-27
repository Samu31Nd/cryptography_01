#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOT "./"
#define _C_ "_c."
#define _R_ "_r."

#define RED "\x1b[31m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

typedef struct {
  char bm[2];
  int size;
  int reservated;
  int offset;
  int sizeMetadata;
  int height, width;
  short int no_planes;
  short int deep_color;
  int compression_type;
  int size_struct;
  int pxmh;
  int pxmv;
  int used_colors;
  int important_colors;
} headerBMP;

typedef struct {
  char **R;
  char **G;
  char **B;
} bmp_image;

typedef struct {
  headerBMP h;
  bmp_image i;
} BMP_file;

void freeBMP(BMP_file*);
void printHeader(BMP_file*);
void freeRGB(bmp_image *, int);
BMP_file *readBMP_file(const char*);
int initializeRGB(bmp_image*, int, int);
char *getNewName(const char*, const char*);
FILE *createNewFile(const char*, const char*);
void writeFile(FILE*, BMP_file*, const int, const int, const int);
