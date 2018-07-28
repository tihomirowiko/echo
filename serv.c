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
  int portNum = 0;
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
  std::string buffs = "";
  char buff[100];
  size_t res = 0;//result of different funcs
  int countSock;
  int on = 1;
  
  listenFd = socket(AF_INET, SOCK_STREAM, 0);
  //fcntl(listenFd, F_SETFL, O_NONBLOCK);
  setsockopt(listenFd, SOL_SOCKET,  SO_REUSEADDR,
                   (char *)&on, sizeof(on));
  ioctl(listenFd, FIONBIO, (char *)&on);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htons(INADDR_ANY);
  addr.sin_port = htons(22000);
  
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
    /* for(std::set<long>::iterator i = servSocket.begin(); i != servSocket.end(); i++){ */
    /*   FD_SET(*i, &readSock); */
    /* } */

    for(std::set<User>::iterator i = servUser.begin(); i != servUser.end(); ++i)
      {
	FD_SET(i->GetFD(), &readSock);
      }

    
    countSock = select(listenFd+servSocket.size()+1, &readSock, NULL, NULL, &timeout);
    if(countSock == 0)
      break;
    if(countSock < 0)
      error("select", errno);

    if(FD_ISSET(listenFd, &readSock)){
      commFd = accept(listenFd, (struct sockaddr *)&addr, &sizeAddr);
      if(commFd < 0)
	error("accept", errno);
      else if(servSocket.insert(commFd).second){
	//fcntl(commFd, F_SETFL, O_NONBLOCK);
	ioctl(commFd, FIONBIO, (char *)&on);
	servSocket.insert(commFd);
	User name(commFd, "name", addr);
	servUser.insert(name);
	print("connect", commFd);
	--countSock;
	do {
	  res =  recv(commFd, buff, 100, 0);
	  if (res > 0) {
	    buffs += buff;
	    memset(buff, 0, 100);
	    std::cout << "serv " << res << std::endl;
	  }else if(res == 0){
	    close(commFd);
	    servSocket.erase(commFd);
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
    

    
    for(std::set<long>::iterator i = servSocket.begin(); i != servSocket.end(); ++i){
      if(FD_ISSET(*i, &readSock)){
	commFd = *i;
	do {
	  res =  recv(commFd, buff, 100, 0);
	  if (res > 0) {
	    buffs += buff;
	    memset(buff, 0, 100);
	    std::cout << "serv " << res << std::endl;
	  }else if(res == 0){
	    close(commFd);
	    servSocket.erase(commFd);
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
    //sleep(1);
  }
  
  
  printf("hello world %d\n", portNum);
  return 0;
}
