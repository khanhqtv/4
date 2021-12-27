#ifndef LOGIN_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define LOGIN_H

#include <cstdlib>
#include <string>

int login();
int is_granted(std::string user, std::string mPass);
void increment_attempt_per_user(std::string user_name);
void reset_attempts(std::string user_name);
bool is_user_locked(std::string user_name);
#endif
