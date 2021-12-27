#include <string>
#include "login.hpp"
#include <iostream>
#include <pqxx/pqxx>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sodium.h>
#include <cstring>
#include "utils.cpp"

using namespace std;


//user  - "password"
//user2 - " password2" 

void update_password_user(const char* user_passw, string user_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c); 
  txn.exec(
	   "UPDATE users "
	   "SET password = " + txn.quote(user_passw) +
	   "WHERE name = " + txn.quote(user_name));

  txn.commit();  
}

bool is_valid_passw(string user_name, string user_passw, string stored_passw){
  int init_sodium = sodium_init();// 0 on Success, -1 on failure, 1 the library is already initialized;
  if(init_sodium == -1){
    cerr << "exception caught: " << "Error initializing Sodium Library" << endl;
    return -1;
  }
  bool result = false;
  char hashed_password[crypto_pwhash_scryptsalsa208sha256_STRBYTES];
  const char* user_p = user_passw.c_str();
  const char* stored_p = stored_passw.c_str();
 
  if (crypto_pwhash_scryptsalsa208sha256_str_verify
      (stored_p, user_p, strlen(user_p)) != 0) {
    result = false;
  } else {
    result = true;
    if (crypto_pwhash_scryptsalsa208sha256_str
	(hashed_password, user_p, strlen(user_p),
	 OPSLIMIT,
	 MEMLIMIT) != 0) {
      cout << "out of memory"<<endl;
      return false;
    }
    update_password_user(hashed_password, user_name);
  }
  return result;
}

int login(){   
  string user = UtilsLibrary::get_user_input( "Please type in your user name: ", false, USER_NAME_MAX_LENGTH);
  string password = UtilsLibrary::get_user_input( "Please type in your password: ", true, USER_PASSW_MAX_LENGTH);  
  return is_granted(user, password);
}

int is_granted(std::string user_name, std::string user_passw){
  if(is_user_locked(user_name)){
    throw LockedUserException();  
  }
  int user_id = -1;
  bool result = false;
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c);

  pqxx::result r = txn.exec(
			    "SELECT id, password " 
			    "FROM users " 			   
			    "WHERE name=" + txn.quote(user_name));
  if(r.size() == 1){
    result = is_valid_passw(user_name, user_passw,r[0][1].as<string>());
    if(result){
      user_id = r[0][0].as<int>();
    }
  }
  if(!result) {
    increment_attempt_per_user(user_name);
  } else {
    reset_attempts(user_name);
  }
  return user_id;
}

void reset_attempts(std::string user_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c);
  txn.exec(
	     "UPDATE users "
	     "SET attempts = 0 "
	     "WHERE name = " + txn.quote(user_name));

  txn.commit();
}

bool is_user_locked(std::string user_name){

  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c);

  pqxx::result r = txn.exec(
			    "SELECT id " 
			    "FROM users " 			   
			    "WHERE name=" + txn.quote(user_name) +		    
			    "AND attempts=attempts_allowed");
 return (r.size() == 1);  
}

void increment_attempt_per_user(std::string user_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c);

  pqxx::result r = txn.exec(
			    "SELECT id " 
			    "FROM users " 			   
			    "WHERE name=" + txn.quote(user_name));
  if(r.size() == 1){
    int user_id = r[0][0].as<int>();
    txn.exec(
	     "UPDATE users "
	     "SET attempts = attempts + 1 "
	     "WHERE id = " + txn.quote(user_id));

    txn.commit();
  } 
}



std::string get_coded_password(std::string mPass){

}
