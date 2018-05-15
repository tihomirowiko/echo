#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int value = 0;
  if (argc > 2)
  {
    printf("too many parameters\n");
    return 0;
  }
  else if (argc == 2)
  {
    value = atoi(argv[1]);
  }
  
  printf("hello world %d\n", value);
  return 0;
}
