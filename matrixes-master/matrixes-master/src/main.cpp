// rememb-o-matic
#include <iostream>
#include <cstdlib>
#include <string>
#include <mexceptions.hpp>
#include "login.cpp"
#include "matrix_management.cpp"
#include <iomanip>
#include <cmath>
#include <limits>
#include <new>
#include <sodium.h>

using namespace std;

int main() {
  int user_id = -1;
  do{
    try{
      user_id = login();
      if(user_id == -1){
	std::cout <<endl;
	std::cout << "Wrong User or Password!. Try it again..."<<endl;
	std::cout <<endl;
      }     
    } catch(LockedUserException &e){
      std::cerr << "exception caught: " << e.message() << endl;    
    }
  } while(user_id == -1);

  if(user_id > -1){
   
    int matrix_id = 0;
    bool mcontinue = true;
    do {
      cout << "run_app" << endl;
      matrix_id = Management::run_app(matrix_id, user_id);
    } while(matrix_id != AppValues::EXIT_CODE);
  }
}
