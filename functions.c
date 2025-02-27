#include "handle_bmp.h"
#include "utils.h"
#include "functions.h"
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define ext ".bmp"
#define maxFiles 5

errcode decryptBMPFile(char* file_name, int DESP_R, int DESP_G, int DESP_B){
    BMP_file *bmp = readBMP_file(file_name);
    printHeader(bmp);
    //NOW SAVE FILE
  
    FILE *newF = createNewFile(file_name,_R_);
    //WRITING HEADER
    writeFile(newF, bmp, DESP_R*-1, DESP_G*-1, DESP_B*-1);
    fclose(newF);
    freeBMP(bmp);
    return _NO_ERR;
}
  
errcode encryptBMPFile(char* file_name, int DESP_R, int DESP_G, int DESP_B){
    BMP_file *bmp = readBMP_file(file_name);
    printHeader(bmp);
    //NOW SAVE FILE
  
    FILE *newF = createNewFile(file_name,_C_);
    //WRITING HEADER
    writeFile(newF, bmp, DESP_R, DESP_G, DESP_B);
    fclose(newF);
    freeBMP(bmp);
    return _NO_ERR;
}

void draw_box(int y, int x, int color, const char* label) { //Marco
  attron(COLOR_PAIR(color));
  mvprintw(y - 1, x - 1, "+------+");
  mvprintw(y, x - 1, "| %s |", label);
  mvprintw(y + 1, x - 1, "+------+");
  attroff(COLOR_PAIR(color));
}

void init_colors(void) {
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // Título
  init_pair(2, COLOR_WHITE, COLOR_YELLOW);  // Fondo
  init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Sin Error | Verde
  init_pair(4, COLOR_RED, COLOR_BLACK);   // Error | Rojo
  init_pair(5, COLOR_BLUE, COLOR_BLACK); // Azul
}

int writeText (char desp[], int pos, int ch) {
  int despl;
  while ((ch = getch()) != '\n') {
      if (ch == KEY_BACKSPACE || ch == 127) { //Borra
          if (pos > 0) {
              pos--;
              desp[pos] = '\0';
              mvaddch(16, 36 + pos, ' ');
              move(16, 36 + pos);
          }
      } else if (pos < 4 - 1) {              //Escribe
          desp[pos++] = ch;
          addch(ch);
      }
      refresh();
  }
  despl = atoi(desp);
  return despl;
}

void titles(int y, int x, const char* text, int colorPair) {
  attron(COLOR_PAIR(colorPair));
  mvprintw(y, x,"%s", text);
  attroff(COLOR_PAIR(colorPair));
}

void printBox (void) {
  attron(COLOR_PAIR(1));
  mvprintw(15, 31, "+-----------+");
  mvprintw(16, 31, "|           |");
  mvprintw(17, 31, "+-----------+");
  attroff(COLOR_PAIR(1));
  return;
}

void printTitle(void) {
  attron(COLOR_PAIR(1));
  box(stdscr, 0, 0);
  mvprintw(9,  39, "  ____  __  __ _____   ");
  mvprintw(10, 39, " |  _ \\|  \\/  |  __ \\ ");
  mvprintw(11, 39, " | |_) | \\  / | |__) | ");
  mvprintw(12, 39, " |  _ <| |\\/| |  ___/ ");
  mvprintw(13, 39, " | |_) | |  | | | ");
  mvprintw(14, 39, " |____/|_|  |_|_|");
  mvprintw(16, 39, "  Image Modifier! ");
  mvprintw(18, 41,"Selecciona el archivo");
  mvprintw(19, 41,"que deseas modificar ");
  attroff(COLOR_PAIR(1));
  refresh();
  return;
}

void printTitleR(void) {
  attron(COLOR_PAIR(4));
  mvprintw(2, 28, "  ____  "); 
  mvprintw(3, 28, " |  _ \\ "); 
  mvprintw(4, 28, " | |_) "); 
  mvprintw(5, 28, " |  _ < "); 
  mvprintw(6, 28, " | |_) "); 
  mvprintw(7, 28, " |____/ "); 
  attroff(COLOR_PAIR(4));

  attron(COLOR_PAIR(1));
  mvprintw(2, 35, " __  __ _____  "); 
  mvprintw(3, 35, "|  \\/  |  __ \\ "); 
  mvprintw(4, 35, "| \\  / | |__) |"); 
  mvprintw(5, 35, "| |\\/| |  ___/"); 
  mvprintw(6, 35, "| |  | | |");
  mvprintw(7, 35, "|_|  |_|_|");
  mvprintw(9, 29, "  Image Modifier! ");
  attroff(COLOR_PAIR(1));
  refresh();
  return;
}

void printTitleG(void) {
  attron(COLOR_PAIR(1));
  mvprintw(2, 28, "  ____  "); 
  mvprintw(3, 28, " |  _ \\ "); 
  mvprintw(4, 28, " | |_) "); 
  mvprintw(5, 28, " |  _ < "); 
  mvprintw(6, 28, " | |_) "); 
  mvprintw(7, 28, " |____/ "); 
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(3));
  mvprintw(2, 35, " __  __ "); 
  mvprintw(3, 35, "|  \\/  | "); 
  mvprintw(4, 35, "| \\  / |"); 
  mvprintw(5, 35, "| |\\/| |"); 
  mvprintw(6, 35, "| |  | |");
  mvprintw(7, 35, "|_|  |_|");
  attroff(COLOR_PAIR(3));

  attron(COLOR_PAIR(1));
  mvprintw(2, 43, "_____  "); 
  mvprintw(3, 43, "  __ \\ "); 
  mvprintw(4, 43, " |__) |"); 
  mvprintw(5, 43, "  ___/"); 
  mvprintw(6, 43, " |");
  mvprintw(7, 43, "_|");
  mvprintw(9, 29, "  Image Modifier! ");
  attroff(COLOR_PAIR(1));
  refresh();
  return;
}

void printTitleB(void) {
  attron(COLOR_PAIR(1));
  mvprintw(2, 28, "  ____  "); 
  mvprintw(3, 28, " |  _ \\ "); 
  mvprintw(4, 28, " | |_) "); 
  mvprintw(5, 28, " |  _ < "); 
  mvprintw(6, 28, " | |_) "); 
  mvprintw(7, 28, " |____/ "); 
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(1));
  mvprintw(2, 35, " __  __"); 
  mvprintw(3, 35, "|  \\/  "); 
  mvprintw(4, 35, "| \\  /"); 
  mvprintw(5, 35, "| |\\/|"); 
  mvprintw(6, 35, "| |  |");
  mvprintw(7, 35, "|_|  |_");
  mvprintw(9, 29, "  Image Modifier! ");
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(5));
  mvprintw(2, 42, " _____  "); 
  mvprintw(3, 42, "|  __ \\ "); 
  mvprintw(4, 42, "| |__) |"); 
  mvprintw(5, 42, "|  ___/"); 
  mvprintw(6, 42, "| |");
  mvprintw(7, 42, "|_|");
  attroff(COLOR_PAIR(5));
  refresh();
  return;
}

void printEndscreen(void) {
  attron(COLOR_PAIR(3));
  mvprintw(6, 27, " _____                   ");
  mvprintw(7, 27, "|  __ \\                 ");
  mvprintw(8, 27, "| |  | | ___  _ __   ___");
  mvprintw(9, 27, "| |  | |/ _ \\| '_ \\ / _ \\");
  mvprintw(10, 27, "| |__| | (_) | | | |  __/");
  mvprintw(11, 27, "|_____/ \\___/|_| |_|\\___|");
  mvprintw(13, 23, "  Se ha creado una nueva carpeta");
  mvprintw(14, 26, " con tu imágen modificada! ");
  attroff(COLOR_PAIR(3));
  mvprintw(16, 23, "Presiona cualquier tecla para salir");
}

int selectMenu(char *files[], int fileCount) {
  int highlight = 0;
  int ch;
  while (1) {
      clear();
      printTitle();
      attron(COLOR_PAIR(1));
      for (int i = 0; i < fileCount; i++) {
          if (i == highlight)
              attron(A_REVERSE);
          mvprintw(10 + 2*i, 10, "%s", files[i]);
          if (i == highlight)
              attroff(A_REVERSE);
      }
      attroff(COLOR_PAIR(1));
      refresh();

      ch = getch();
      switch (ch) {
          case KEY_UP:
              if (highlight > 0) highlight--;
              break;
          case KEY_DOWN:
              if (highlight < fileCount - 1) highlight++;
              break;
          case '\n':
              return highlight;
      }
  }   
}

void getFiles(char *files[], int *fileCount) {
  struct dirent *file;
  DIR *dir = opendir(".");

  if (dir == NULL) {
      perror("Error al abrir directorio");
      exit(1);
  } 

  while ((file = readdir(dir)) != NULL) {
      if (strstr(file->d_name, ext)) {
          files[*fileCount] = strdup(file->d_name);
          (*fileCount)++;
          if (*fileCount >= maxFiles) 
              break;
      }
  }
  return;
}

int shiftValues(char *colorLetter, char desp[], int pos, int ch, int colorPair) {
  int color;
  //clear();
  attron(COLOR_PAIR(colorPair));
  box(stdscr, 0, 0);
  if(strcmp(colorLetter, "R") == 0)
      printTitleR();
  else if(strcmp(colorLetter, "G") == 0)
      printTitleG();
  else if(strcmp(colorLetter, "B") == 0)
      printTitleB();
  mvprintw(13, 17, "Ingresa el desplazamiento deseado en el canal %s", colorLetter);
  printBox();
  attroff(COLOR_PAIR(colorPair));

  refresh();
  move (16, 36);
  color = writeText(desp, pos, ch);
  desp[pos] = '\0';
  if (color < 1 || color > 255)
      return 0;
  return color;
}

int cyphDesyphMenu (void) {
  int highlight = 0;
  int ch;
  const char* options[] = {"Cifrar", "Descifrar"};
  while (1) {
      clear();
      printTitle();
      attron(COLOR_PAIR(1));
      for (int i = 0; i < 2; i++) {
          if (i == highlight)
              attron(A_REVERSE);
          mvprintw(10 + 2*i, 10, "%s", options[i]);
          if (i == highlight)
              attroff(A_REVERSE);
      }
      attroff(COLOR_PAIR(1));
      refresh();

      ch = getch();
      switch (ch) {
          case KEY_UP:
              if (highlight > 0) highlight--;
              break;
          case KEY_DOWN:
              if (highlight < 1) highlight++;
              break;
          case '\n':
              return highlight + 1;
      }
  }   
}

void errorScreen(void) {
  clear();
  attron(COLOR_PAIR(4));
  box(stdscr, 0, 0);
  mvprintw(6, 27, " ______                     ");
  mvprintw(7, 27, "|  ____|                    ");
  mvprintw(8, 27, "| |__   _ __ _ __ ___  _ __");
  mvprintw(9, 27, "|  __| | '__| '__/ _ \\| '__|");
  mvprintw(10, 27,"| |____| |  | | | (_) | |  ");
  mvprintw(11, 27,"|______|_|  |_|  \\___/|_|  ");
  mvprintw(13, 25, " Debes elegir un número entre");
  mvprintw(14, 36, " 1 y 225");
  mvprintw(16, 24, "Presiona cualquier tecla para salir");
  attroff(COLOR_PAIR(4));
  refresh();
  getch();
  return;
}