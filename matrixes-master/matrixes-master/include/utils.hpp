#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H
#include <cstdlib>
#include <string>

using namespace std;
namespace UtilsLibrary
{
 int getch();
string get_user_input(std::string message, bool maskared, int max_length);
}
#endif
