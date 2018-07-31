#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <set>
#include <sys/ioctl.h>
#include <errno.h>
#include "user.h"

void error(const std::string str, int codeErr){
  perror(str.c_str());
  std::cout << " errno = " << codeErr << "\n";
  exit(0);
}

template<typename T>
void print(std::string str, T temp){
  std::cout << str << " " << temp << "\n";
}

int main(int argc, char** argv){
  int portNum = 22000;
  if (argc > 2)
  {
    printf("too many parameters\n");
    return 0;
  }
  else if (argc == 2)
  {
    portNum = std::atoi(argv[1]);
  }
  
  fd_set readSock;
  struct timeval timeout;
  std::set<long> servSocket;
  std::set<User> servUser;
  struct sockaddr_in addr;
  long listenFd, commFd;
  long maxFd;
  std::string buffs = "";
  char buff[100];
  size_t res = 0;//result of different funcs
  int countSock;
  int on = 1;
  
  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  setsockopt(listenFd, SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
  ioctl(listenFd, FIONBIO, (char *)&on);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(portNum);
  
  res = bind(listenFd, (struct sockaddr *) &addr, sizeof(addr));
  if (res < 0) error("bind", errno);
  res = listen(listenFd, 10);
  if (res < 0) error("listen", errno);
  
  timeout.tv_sec = 60;
  timeout.tv_usec = 0;
  
  socklen_t sizeAddr = sizeof(addr);
  
  while(1)
  {
    std::cout << "loop\n";
    buffs = "";
    memset(buff, 0, 100);
    timeout.tv_sec = 60;

    FD_ZERO(&readSock);
    FD_SET(listenFd, &readSock);
    maxFd = listenFd;

    for(std::set<User>::iterator i = servUser.begin(); i != servUser.end(); ++i) {
      FD_SET(i->GetFD(), &readSock);
      if(maxFd < i->GetFD()) maxFd = i->GetFD();
    }

    countSock = select(maxFd+1, &readSock, NULL, NULL, &timeout);
    if(countSock == 0){    //timeout
      print("timeout: select is ", countSock);
      break;
    }
    if(countSock < 0) error("select", errno);

    if(FD_ISSET(listenFd, &readSock)){
      commFd = accept(listenFd, (struct sockaddr *)&addr, &sizeAddr);
      if(commFd < 0)
	error("accept", errno);
      else if(servUser.insert(User(commFd, "name", addr)).second){
	ioctl(commFd, FIONBIO, (char *)&on);
	User nameTemp(commFd, "name", addr);
	servUser.insert(nameTemp);
	print("connect", commFd);
	--countSock;
      }
    }

    for(std::set<User>::iterator i = servUser.begin(); i != servUser.end(); ++i){
      if(FD_ISSET(i->GetFD(), &readSock)){
	commFd = i->GetFD();
	do {
	  res =  recv(commFd, buff, 100, 0);
	  if (res > 0) {
	    buffs += buff;
	    memset(buff, 0, 100);
	  }else if(res == 0){
	    close(commFd);
	    print("disconnect ", commFd);
	    i->Show();
	    servUser.erase(*i);
	  }else{
	    error("recv", errno);
	  }
	}while(res == 100);
	if (buffs != "")
	  std::cout << "Serv: " << buffs << std::endl;
	res = send(commFd, buffs.c_str(), buffs.size(), 0);
	if (res < 0)
	  error("send", errno);
      }
    }
  }
  
  
  printf("Bye world %d\n", portNum);
  return 0;
}
