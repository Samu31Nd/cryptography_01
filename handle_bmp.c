#include "handle_bmp.h"
#include "utils.h"

int initializeRGB(bmp_image *i, int w, int h) {
    i->R = (char **)malloc(sizeof(char *) * h);
    i->G = (char **)malloc(sizeof(char *) * h);
    i->B = (char **)malloc(sizeof(char *) * h);

    if (!i->R || !i->G || !i->B) {
        freeRGB(i, h);
        return -1;
    }

    for (int j = 0; j < h; j++) {
        i->R[j] = (char *)malloc(sizeof(char) * w);
        i->G[j] = (char *)malloc(sizeof(char) * w);
        i->B[j] = (char *)malloc(sizeof(char) * w);

        if (!i->R[j] || !i->G[j] || !i->B[j]) {
            freeRGB(i, j);
            return -1;
        }
    }
    return 0; // Éxito
}

BMP_file *readBMP_file(const char *filename)
{
    FILE *f;
    if ((f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, RED "Error al abrir el archivo BMP: " RESET "No se encontro el archivo " YELLOW"[%s]"RESET"\n",filename);
        exit(-1);
    }
    BMP_file *bmp = (BMP_file *)malloc(sizeof(BMP_file));
    // METADATA READ
    fseek(f, 0, SEEK_SET);
    fread(&bmp->h.bm, sizeof(char), 2, f);
    fread(&bmp->h.size, sizeof(int), 1, f);
    fread(&bmp->h.reservated, sizeof(int), 1, f);
    fread(&bmp->h.offset, sizeof(int), 1, f);
    fread(&bmp->h.sizeMetadata, sizeof(int), 1, f);
    fread(&bmp->h.height, sizeof(int), 1, f);
    fread(&bmp->h.width, sizeof(int), 1, f);
    fread(&bmp->h.no_planes, sizeof(short int), 1, f);
    fread(&bmp->h.deep_color, sizeof(short int), 1, f);
    fread(&bmp->h.compression_type, sizeof(int), 1, f);
    fread(&bmp->h.size_struct, sizeof(int), 1, f);
    fread(&bmp->h.pxmh, sizeof(int), 1, f);
    fread(&bmp->h.pxmv, sizeof(int), 1, f);
    fread(&bmp->h.used_colors, sizeof(int), 1, f);
    fread(&bmp->h.important_colors, sizeof(int), 1, f);

    if ( (initializeRGB(&bmp->i, bmp->h.width, bmp->h.height)) != 0){
        fprintf(stderr, RED "Error al usar malloc: " RESET "No se pudo asignar memoria.\n");
        exit(-1);
    }

    for (int i = 0; i < bmp->h.height; i++)
    {
        for (int j = 0; j < bmp->h.width; j++)
        {
            fread(&bmp->i.B[i][j], sizeof(char), 1, f);
            fread(&bmp->i.G[i][j], sizeof(char), 1, f);
            fread(&bmp->i.R[i][j], sizeof(char), 1, f);
        }
    }
    fclose(f);
    return bmp;
}

void printHeader(BMP_file *bmp)
{
    printf("\n-------------=="YELLOW " HEADER " RESET "==-------------\n");
    printf(YELLOW "  BM: "RESET "%c%c\n", bmp->h.bm[0], bmp->h.bm[1]);
    printf(YELLOW "  Size: "RESET "%d\n", bmp->h.size);
    printf(YELLOW "  Reservado: "RESET "%d\n", bmp->h.reservated);
    printf(YELLOW "  Offset: "RESET "%d\n", bmp->h.offset);
    printf(YELLOW "  Size Metadata: "RESET "%d\n", bmp->h.sizeMetadata);
    printf(YELLOW "  Size (height, width): "RESET "(%d,%d)\n", bmp->h.height, bmp->h.width);
    printf(YELLOW "  Numero planos: "RESET "%d\n", bmp->h.no_planes);
    printf(YELLOW "  Profundidad color: "RESET "%d\n", bmp->h.deep_color);
    printf(YELLOW "  Tamaño struct: "RESET "%d\n", bmp->h.size_struct);
    printf(YELLOW "  Resolucion: "RESET "(%d,%d)\n", bmp->h.pxmv, bmp->h.pxmh);
    printf(YELLOW "  Colores usados: "RESET "%d\n", bmp->h.used_colors);
    printf(YELLOW "  Colores importantes: "RESET "%d\n", bmp->h.important_colors);
    printf("--------------------------------------\n");
}

void freeBMP(BMP_file *bmp)
{
    freeRGB(&bmp->i, bmp->h.height);
    free(bmp);
}

void freeRGB(bmp_image *i, int h) {
    if (i->R) {
        for (int j = 0; j < h; j++) free(i->R[j]);
        free(i->R);
    }
    if (i->G) {
        for (int j = 0; j < h; j++) free(i->G[j]);
        free(i->G);
    }
    if (i->B) {
        for (int j = 0; j < h; j++) free(i->B[j]);
        free(i->B);
    }
}

void writeFile(FILE *newF, BMP_file *bmp, const int DESP_R, const int DESP_G, const int DESP_B)
{
    fseek(newF, 0, SEEK_SET);
    fwrite(&bmp->h.bm, sizeof(char), 2, newF);
    fwrite(&bmp->h.size, sizeof(int), 1, newF);
    fwrite(&bmp->h.reservated, sizeof(int), 1, newF);
    fwrite(&bmp->h.offset, sizeof(int), 1, newF);
    fwrite(&bmp->h.sizeMetadata, sizeof(int), 1, newF);
    fwrite(&bmp->h.height, sizeof(int), 1, newF);
    fwrite(&bmp->h.width, sizeof(int), 1, newF);
    fwrite(&bmp->h.no_planes, sizeof(short int), 1, newF);
    fwrite(&bmp->h.deep_color, sizeof(short int), 1, newF);
    fwrite(&bmp->h.compression_type, sizeof(int), 1, newF);
    fwrite(&bmp->h.size_struct, sizeof(int), 1, newF);
    fwrite(&bmp->h.pxmh, sizeof(int), 1, newF);
    fwrite(&bmp->h.pxmv, sizeof(int), 1, newF);
    fwrite(&bmp->h.used_colors, sizeof(int), 1, newF);
    fwrite(&bmp->h.important_colors, sizeof(int), 1, newF);
    unsigned char newR, newG, newB;
    for (int i = 0; i < bmp->h.height; i++)
    {
        for (int j = 0; j < bmp->h.width; j++)
        {
            newB = (bmp->i.B[i][j] + DESP_B) % 255;
            if (newB < 0) newB += 255;
            newG = (bmp->i.G[i][j] + DESP_G) % 255;
            if (newG < 0) newG += 255;
            newR = (bmp->i.R[i][j] + DESP_R) % 255;
            if (newR < 0) newR += 255;
            fwrite(&newB, sizeof(char), 1, newF);
            fwrite(&newG, sizeof(char), 1, newF);
            fwrite(&newR, sizeof(char), 1, newF);
        }
    }
}

FILE *createNewFile(const char *file_name, const char* type_file)
{
    struct stat st = {0};
    if (stat("./new_files", &st) == -1)
    {
        mkdir("./new_files", 0700);
    }

    // CREATING FILE
    FILE *newF;
    char *new_name = getNewName(file_name, type_file);

    if ((newF = fopen(new_name, "w")) == NULL)
    {
        printf("[NAME: %s]\n", new_name);
        fprintf(stderr, RED "Error al intentar crear el archivo BMP: " YELLOW"[%s]"RESET"\n",new_name);
        fclose(newF);
        exit(-1);
    }
    free(new_name);
    return newF;
}

char *getNewName(const char* file_name, const char *end_file){
    int newSizeWord = strlen(file_name) + 2; //_c or _r
    char *full_new_name = (char*)malloc(sizeof(char)*newSizeWord);
    for(int i = 0; i < newSizeWord; i++)
        full_new_name[i] = '\0';
    char **splittedName = split(file_name, '.');
    strcat(full_new_name, splittedName[0]);
    strcat(full_new_name, end_file);
    strcat(full_new_name, splittedName[1]);

    freeSplitted(splittedName);
    return full_new_name;
}