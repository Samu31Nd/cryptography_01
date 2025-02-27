#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <dirent.h>
#include "handle_bmp.h"
#include "utils.h"
#include "functions.h"
#define ext ".bmp"
#define maxFiles 5

void menu(void);

int main (int argc, char const *argv[]) {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(1);
    init_colors();

    menu();
    endwin();
    return 0;
}

void menu (void) {
    int ch = 0, pos = 0;
    int R,G,B;
    int fileCount = 0;
    char despR[4] = {0};
    char despG[4] = {0};
    char despB[4] = {0};
    char *files[maxFiles];

    getFiles(files, &fileCount);
    int selectedFile = selectMenu(files, fileCount);
    int selection = cyphDesyphMenu();

    clear();

    if (selection == 1) {                       
        R = shiftValues("R", despR, pos, ch, 4);
        G = shiftValues("G", despG, pos, ch, 3);
        B = shiftValues("B", despB, pos, ch, 5);
        if (R == 0 || G == 0 || B == 0) {
            errorScreen();
            return;
        }
        encryptBMPFile(files[selectedFile], R, G, B);
    } else if (selection == 2) {                
        R = shiftValues("R", despR, pos, ch, 3);
        G = shiftValues("G", despG, pos, ch, 4);
        B = shiftValues("B", despB, pos, ch, 5);
        if (R == 0 || G == 0 || B == 0) {
            errorScreen();
            return;
        }
        decryptBMPFile(files[selectedFile], R, G, B);
    }

    clear();

    attron(COLOR_PAIR(3));
    box(stdscr, 0, 0);
    printEndscreen();
    attroff(COLOR_PAIR(3));
    refresh();

    for (int i = 0; i < fileCount; i++)
        free(files[i]);
    getch();
    return;
}
