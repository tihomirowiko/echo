#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class User{
 public:
  User();
  User(std::string name);
  User(std::string name, std::string address);
  void Show();

  std::string Name;
  struct sockaddr_in Addr;
};
