#include <iostream>
#include <cstring>
#include <stdlib.h>

void error(const String str){
  perror(str);
  exit(0);
}

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

  struct sockaddr_in addr;
  int listenFd, commFd;
  String buffs = "";
  char buff[100];
  size_t res = 0;//result of different funcs

  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(22000);
  
  bind(listenFd, (struct sockaddr *) addr, sizeof(addr));
  listen(listenFd, 10);

  commFd = accept(listenFd, (struct sockaddr *) addr, sizeof(addr));
  while(1)
  {
    buffs = "";
    bzero(buff, 100);
    do {
      res =  recv(commFd, buff, 100, MSG_PEEK);
      if (res > 0) {
	buffs += buff;
	memset(buff, 0, 100);
      }else if(res == 0){
      }else{
	error("recv");
      }
    }while(res < 100);
    std::cout << buffs << endl;
    res = send(commFd, buffs, strlen(buffs), NULL);
    if (res < 0)
      error("send");
  }
  
  printf("hello world %d\n", value);
  return 0;
}
