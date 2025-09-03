#include <stdio.h>
#include <stdlib.h>

int main(int cArgs, char* vArgs[]) {
  if (cArgs != 5) {
    fprintf(stderr, "Error: incorrect number of arguments provided");
  }
  char* filename = vArgs[1];
  int nFrames = atoi(vArgs[2]);
  char* algorithm = vArgs[3];
  char* mode = vArgs[4];

  int diskReads = 0;
  int diskWrites = 0;
  char* frames[nFrames];
  return 0;
}
