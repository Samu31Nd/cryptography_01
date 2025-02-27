#include "handle_bmp.h"
#include "utils.h"

#define DESP_R 100
#define DESP_G 200
#define DESP_B 50
#define FILE_NAME "plantita.bmp"

errcode encryptBMPFile(char*);
errcode decryptBMPFile(char*);

int main(int argc, char *argv[]) {
  //decryptBMPFile(FILE_NAME);
  encryptBMPFile(FILE_NAME);
  return 0;
}

errcode decryptBMPFile(char* file_name){
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

errcode encryptBMPFile(char* file_name){
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