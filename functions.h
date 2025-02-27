#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#define errcode int
#define _NO_ERR 0 

errcode decryptBMPFile(char* file_name, int DESP_R, int DESP_G, int DESP_B);
errcode encryptBMPFile(char* file_name, int DESP_R, int DESP_G, int DESP_B);
void draw_box(int y, int x, int color, const char* label);
void init_colors(void);
int writeText(char desp[], int pos, int ch);
void titles(int y, int x, const char* text, int colorPair);
void printBox (void);
void printTitle(void);
void printTitleR(void);
void printTitleG(void);
void printTitleB(void);
void printEndscreen(void);
int selectMenu(char *files[], int fileCount);
void getFiles(char *files[], int *fileCount);
int shiftValues(char *colorLetter, char desp[], int pos, int ch, int colorPair);
int cyphDesyphMenu(void);
void errorScreen(void);


