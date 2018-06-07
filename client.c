#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

void error(const std::string str){
  perror(str.c_str());
  exit(0);
}

int main(int argc, char** argv){
  int portNum = 0;
  if (argc > 2){
    std::cout << "too many parameters\n";
    return 0;
  }else if(argc == 2){
    portNum = std::atoi(argv[1]);
  }else{
    portNum = 22000;
  }

  struct sockaddr_in addr;
  int listenFd, commFd;
  std::string buffs = "";
  char buff[100];
  size_t res = 0;

  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  //fcntl(listenFd, F_SETFL, O_NONBLOCK);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(portNum);
  inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr));
  res = connect(listenFd, (struct sockaddr *) &addr, sizeof(addr));
  if (res < 0)
    error("connect");

  while(1){
    std::cin >> buffs;
    res = send(listenFd, buffs.c_str(), buffs.size(), 0);
    if (res < 0)
      error("send");
    buffs = "";
    do {
      memset(buff, 0, 100);
      res = recv(listenFd, buff, 100, 0);
      if (res > 0) {
	buffs += buff;
      }else if(res == 0){
      }else{
	error("recv");
      }
    }while(res == 100);
    std::cout << "Client: " << buffs << std::endl;
  }
  return 0;
}
