#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOT "./"
#define NEW_FILES_DIR "./new_files/"

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
FILE *createNewFile(const char*);
BMP_file *readBMP_file(const char*);
char *getNewEncryptedName(const char*);
void initializeRGB(bmp_image*, int, int);
char *getNewDecryptedName(const char* file_name);
void writeFile(FILE*, BMP_file*, const int, const int, const int);
