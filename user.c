#include <arpa/inet.h>
#include <iostream>
#include "user.h"


const unsigned int family = AF_INET;

User::User(): FD(0), Name("User")
{
  Addr.sin_family = family;
  Addr.sin_addr.s_addr = htons(INADDR_ANY);
  Addr.sin_port = htons(22000);
}

User::User(long fd, std::string name): FD(fd), Name(name)
{
  Addr.sin_family = family;
  Addr.sin_addr.s_addr = htons(INADDR_ANY);
  Addr.sin_port = htons(22000);
}

User::User(long fd, std::string name, std::string address): FD(fd), Name(name)
{
  Addr.sin_family = family;
  int temp = address.find(":");//count symbols before :
  inet_pton(family, address.substr(0, temp).c_str(), &(Addr.sin_addr));
  Addr.sin_port = htons(std::stoi(address.substr(temp + 1, 10)));
}

User::User(long fd, std::string name, struct sockaddr_in addr): FD(fd), Name(name)
{
  Addr.sin_family = addr.sin_family;
  Addr.sin_addr.s_addr = addr.sin_addr.s_addr;
  Addr.sin_port = addr.sin_port;
}

void User::Show() const
{
  std::cout << "Fd " << FD << " Name " << Name << "\n";
  std::cout << "Ip address " << inet_ntoa(Addr.sin_addr)
	    << ":" <<  ntohs(Addr.sin_port) << "\n";
}

void User::SetName(std::string name)
{
  Name = name;
}

std::string User::GetName() const
{
  return Name;
}

std::string &  User::GetName()
{
  return Name;
}

struct sockaddr_in User::GetAddr() const
{
  return Addr;
}

struct sockaddr_in & User::GetAddr()
{
  return Addr;
}

long User::GetFD() const
{
  return FD;
}

long & User::GetFD()
{
  return FD;
}

bool User::operator<(User &rhs)
{
  return FD < rhs.GetFD();
}

bool User::operator<(const User &rhs) const
{
  return FD < rhs.GetFD();
}
