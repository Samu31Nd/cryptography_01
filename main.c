#include "handle_bmp.h"
#include "utils.h"

#define DESP_R 100
#define DESP_G 200
#define DESP_B 50
#define FILE_NAME "plantita.bmp"

int main(int argc, char *argv[]) {
  BMP_file *bmp = readBMP_file(FILE_NAME);
  printHeader(bmp);
  //NOW SAVE FILE

  FILE *newF = createNewFile(FILE_NAME);
  //WRITING HEADER
  writeFile(newF, bmp, DESP_R, DESP_G, DESP_B);
  fclose(newF);
  freeBMP(bmp);
  return 0;
}