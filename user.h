#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class User{
 public:
  User();
  User(long fd, std::string name);
  User(long fd, std::string name, std::string address);
  User(long fd, std::string name, struct sockaddr_in addr);
  void Show() const;

  //set
  void SetName(std::string name);

  //get
  std::string GetName() const;
  std::string & GetName();
  struct sockaddr_in GetAddr() const;
  struct sockaddr_in & GetAddr();
  long GetFD() const;
  long & GetFD();

  //operator
  bool operator<(User &rhs);
  bool operator<(const User &rhs) const;
  
  //variables
  long FD;
  std::string Name;
  struct sockaddr_in Addr;
};
