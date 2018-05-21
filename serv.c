#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <netdb.h>

void error(const std::string str){
  perror(str.c_str());
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
    value = std::atoi(argv[1]);
  }

  struct sockaddr_in addr;
  int listenFd, commFd;
  std::string buffs = "";
  char buff[100];
  size_t res = 0;//result of different funcs

  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(22000);
  
  res = bind(listenFd, (struct sockaddr *) &addr, sizeof(addr));
  if (res < 0) error("bind");
  listen(listenFd, 10);

  socklen_t sizeAddr = sizeof(addr);
  commFd = accept(listenFd, (struct sockaddr *) &addr, &sizeAddr);
  if (commFd < 0) error("accept");
  while(1)
  {
    buffs = "";
    memset(buff, 0, 100);
    do {
      res =  recv(commFd, buff, 100, 0);
      if (res > 0) {
	buffs += buff;
	memset(buff, 0, 100);
	std::cout << "serv " << res << std::endl;
      }else if(res == 0){
      }else{
	error("recv");
      }
    }while(res == 100);
    if (buffs != "")
      std::cout << "Serv: " << buffs << std::endl;
    res = send(commFd, buffs.c_str(), buffs.size(), 0);
    if (res < 0)
      error("send");
  }
  
  printf("hello world %d\n", value);
  return 0;
}
