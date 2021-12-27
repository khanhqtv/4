#include <iostream>
#include <cstdlib>
#include <mexceptions.hpp>
//#include "login.cpp"
#include <iomanip>
#include <cmath>
#include <limits>
#include <new>
#include <assert.h>
#include <sodium.h>
#include <cstring>
#include "utils.cpp"
#include "matrix_management.cpp"


int main(void)
{
  int option = 0;
  int matrix_id = 0;
  int user_id = 0;
  cin >> option;
  cin >> matrix_id;
  cin >> user_id;
  int x = Management::run_option(option, matrix_id, user_id);
  cout << "x= " << x << endl;
  assert(x == matrix_id);
}
