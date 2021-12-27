#include "utils.hpp"
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <LibConstants.hpp>
#include <ctime>

using namespace std;

namespace UtilsLibrary
{

  int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
  }

  string get_user_input(std::string message, bool maskared, int max_length){
    const char BACKSPACE=127;
    const char RETURN=10;
    unsigned char ch=0;
    string user_value = "";
    cout << message;
    while(((ch=getch()) !='\n') && (ch != EOF)){//!= RETURN){    
      if(ch == BACKSPACE){
	if(user_value.length() != 0){
	  cout << "\b \b";
	  user_value.resize(user_value.length()-1);
	}   
      } else {
	if( strlen(user_value.c_str()) < max_length){
	  user_value += ch;
	  if(maskared) {
	    cout << '*';
	  } else {
	    cout << ch;
	  }
	}
      }    
    }
    cout <<endl;
    return user_value;
  }

  string get_current_date_as_string(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d_%m_%Y",timeinfo);
    std::string str(buffer);
    return str;
  }

  string get_current_time_as_string(){
   time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%I:%M:%S",timeinfo);
    std::string str(buffer);
    return str;
  }
}
