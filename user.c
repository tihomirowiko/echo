#include <arpa/inet.h>
#include <iostream>
#include "user.h"


const unsigned int family = AF_INET;

User::User(): Name("User")
{
  Addr.sin_family = family;
  Addr.sin_addr.s_addr = htons(INADDR_ANY);
  Addr.sin_port = htons(22000);
}

User::User(std::string name): Name(name)
{
  Addr.sin_family = family;
  Addr.sin_addr.s_addr = htons(INADDR_ANY);
  Addr.sin_port = htons(22000);
}

User::User(std::string name, std::string address): Name(name)
{
  Addr.sin_family = family;
  int temp = address.find(":");//count symbols before :
  inet_pton(family, address.substr(0, temp).c_str(), &(Addr.sin_addr));
  Addr.sin_port = htons(std::stoi(address.substr(temp + 1, 10)));
}

void User::Show()
{
std::cout << "Name " << Name << "\n";
std::cout << "Ip address " << inet_ntoa(Addr.sin_addr)
<< ":" <<  ntohs(Addr.sin_port) << "\n";
}
