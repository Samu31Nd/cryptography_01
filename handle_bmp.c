#include "handle_bmp.h"
#include "utils.h"

void initializeRGB(bmp_image *i, int w, int h)
{
    char **R = (char **)malloc(sizeof(char *) * h);
    char **G = (char **)malloc(sizeof(char *) * h);
    char **B = (char **)malloc(sizeof(char *) * h);
    for (int i = 0; i < h; i++)
    {
        R[i] = (char *)malloc(sizeof(char) * w);
        G[i] = (char *)malloc(sizeof(char) * w);
        B[i] = (char *)malloc(sizeof(char) * w);
    }
    i->R = R;
    i->G = G;
    i->B = B;
}

/*
 * filename must be like "./filename.bmp"
 */
BMP_file *readBMP_file(const char *filename)
{
    FILE *f;
    if ((f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo!");
        fclose(f);
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

    initializeRGB(&bmp->i, bmp->h.width, bmp->h.height);

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
    printf("BM: %c%c\n", bmp->h.bm[0], bmp->h.bm[1]);
    printf("Size: %d\n", bmp->h.size);
    printf("Reservado: %d\n", bmp->h.reservated);
    printf("Offset: %d\n", bmp->h.offset);
    printf("Size Metadata: %d\n", bmp->h.sizeMetadata);
    printf("Size (height, width): (%d,%d)\n", bmp->h.height, bmp->h.width);
    printf("Numero planos: %d\n", bmp->h.no_planes);
    printf("Profundidad color: %d\n", bmp->h.deep_color);
    printf("Tamaño struct: %d\n", bmp->h.size_struct);
    printf("Resolucion: (%d,%d)\n", bmp->h.pxmv, bmp->h.pxmh);
    printf("Colores usados: %d\n", bmp->h.used_colors);
    printf("Colores importantes: %d\n", bmp->h.important_colors);
}

void freeBMP(BMP_file *bmp)
{
    printf("Erasing memory...\n");
    for (int j = 0; j < bmp->h.height; j++)
    {
        free(bmp->i.R[j]);
        free(bmp->i.G[j]);
        free(bmp->i.B[j]);
    }
    free(bmp->i.R);
    free(bmp->i.G);
    free(bmp->i.B);
    free(bmp);
    printf("Erased!\n");
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
            newG = (bmp->i.G[i][j] + DESP_G) % 255;
            newR = (bmp->i.R[i][j] + DESP_R) % 255;
            fwrite(&newB, sizeof(char), 1, newF);
            fwrite(&newG, sizeof(char), 1, newF);
            fwrite(&newR, sizeof(char), 1, newF);
        }
    }
}

FILE *createNewFile(const char *file_name)
{
    struct stat st = {0};
    if (stat("./new_files", &st) == -1)
    {
        mkdir("./new_files", 0700);
    }

    // CREATING FILE
    FILE *newF;
    char *new_name = getNewEncryptedName(file_name);

    if ((newF = fopen(new_name, "w")) == NULL)
    {
        printf("[NAME: %s]\n", new_name);
        fprintf(stderr, "Error al abrir el archivo!");
        fclose(newF);
        exit(-1);
    }
    free(new_name);
    return newF;
}

char *getNewEncryptedName(const char* file_name){
    int newSizeWord = strlen(file_name) + 2 + strlen(NEW_FILES_DIR); //_c
    char *full_new_name = (char*)malloc(sizeof(char)*newSizeWord);
    for(int i = 0; i < newSizeWord; i++)
        full_new_name[i] = '\0';
    char **splittedName = split(file_name, '.');
    strcat(full_new_name, NEW_FILES_DIR);
    strcat(full_new_name, splittedName[0]);
    strcat(full_new_name, "_c.");
    strcat(full_new_name, splittedName[1]);

    freeSplitted(splittedName);
    return full_new_name;
}

char *getNewDecryptedName(const char* file_name){
    int newSizeWord = strlen(file_name) + 2 + strlen(NEW_FILES_DIR); //_c
    char *full_new_name = (char*)malloc(sizeof(char)*newSizeWord);
    for(int i = 0; i < newSizeWord; i++)
        full_new_name[i] = '\0';
    char **splittedName = split(file_name, '.');
    strcat(full_new_name, NEW_FILES_DIR);
    strcat(full_new_name, splittedName[0]);
    strcat(full_new_name, "_c.");
    strcat(full_new_name, splittedName[1]);

    freeSplitted(splittedName);
    return full_new_name;
}