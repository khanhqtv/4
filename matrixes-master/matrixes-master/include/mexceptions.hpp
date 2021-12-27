#include <iostream>
#include <exception>

using namespace std;

class LockedUserException: public exception{
public:
  virtual const char* message() const throw(){
    return "Internal Error: Contact Administrators";
  }
};
