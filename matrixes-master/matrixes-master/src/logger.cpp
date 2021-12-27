#include <logger.hpp>
#include <sys/stat.h>
#include <cstring>
#include <stdio.h>
#include <sodium.h>
#include <pqxx/pqxx>
#include <string>
#include <fstream>
#include "disk_usage.cpp"


#define MAXLEN 64
#define URL "./logs/"

using namespace UtilsLibrary;

string get_content_hash(string data);

string get_file_content(string url){ 
  std::ifstream ifs(url.c_str());
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
		      (std::istreambuf_iterator<char>()    ) );
 return content;
}
void update_log_hash(string hash, string log_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c); 
  txn.exec(
	   "UPDATE logs "
	   "SET hash = " + txn.quote(hash) +
	   "WHERE log_name = " + txn.quote(log_name));

  txn.commit();  
}

string get_log_hash(std::string log_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c);

  pqxx::result r = txn.exec(
			    "SELECT hash " 
			    "FROM logs " 			   
			    "WHERE log_name=" + txn.quote(log_name));
  string hash = "-1";
  if(r.size() == 1){
    hash = r[0][0].as<string>();
  }
  return hash;
}

int mkpath(std::string s,mode_t mode)
{
  size_t pre=0,pos;
  std::string dir;
  int mdret;

  if(s[s.size()-1]!='/'){
    // force trailing / so we can handle everything in loop
    s+='/';
  }

  while((pos=s.find_first_of('/',pre))!=std::string::npos){
    dir=s.substr(0,pos++);
    pre=pos;
    if(dir.size()==0) continue; // if leading / first time is 0 length
    if((mdret=mkdir(dir.c_str(),mode)) && errno!=EEXIST){
      return mdret;
    }
  }
  return mdret;
}
void insert_log_hash(string hash, string log_name){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c); 
  txn.exec(
	   "INSERT INTO logs "
	   " VALUES (" + txn.quote(log_name) + ", "// + txn.quote("sad") +
	   + txn.quote(hash) + ")");//"'hashsss')");

  txn.commit();  
}

void log(string data){ 
  std::ofstream outfile;
  int mkdirretval = mkpath(URL, 0755);
  if(DiskUsage::is_disk_space_available(URL)){ 
    string date = UtilsLibrary:: get_current_date_as_string();
    string file_url = URL + date;
    outfile.open(file_url.c_str(), std::ios_base::app);
 
    if(outfile.is_open()){
      string previous_hash = get_log_hash(file_url);    
      string original_content = get_file_content(file_url);
    
      bool modified_log = previous_hash != get_content_hash(original_content);
    
      if(modified_log && previous_hash != "-1"){
	cerr << "Fallo de seguridad. Integridad de fichero comprometida.";
      } else {
	string info = "[" + UtilsLibrary::get_current_time_as_string() + "] :: " ;
	info += data;
	outfile << info << endl; 
	outfile.close();
    
	if(previous_hash  == "-1"){
	  insert_log_hash(get_content_hash(get_file_content(file_url)), file_url);    
	} else { 
	  update_log_hash(get_content_hash(get_file_content(file_url)), file_url);
	}
      }
    } else {
      cerr << "No se ha podido escribir en el log" << endl;
    }  
  }
}

void log_error(string data){
  string error = "ERROR :: " + data;
  log(error);
}

string get_content_hash(string data){
  unsigned char hash[crypto_generichash_BYTES];
  unsigned char *data_p = new unsigned char[data.length()+1];
  string result = "";
  // try{
    //data_p = (const unsigned char *)data.c_str();
    std::copy(data.begin(), data.end(), data_p);
    data_p[data.size()] = '\0';
    crypto_generichash(hash, sizeof hash,
		       data_p, data.length()+1,
		       NULL, 0);
    ostringstream convert; 
    for (size_t j = 0; j <sizeof hash; ++j) {
      convert << ((unsigned int) hash[j]);
    }
    data_p = NULL;
    delete[] data_p;
    result = convert.str(); 
    // } catch (const std::exception &e) {
    //if(data_p){
    // data_p = NULL;
    // delete[] data_p;
    //}  
    //}
  return result;
}

void print_logger_content(string file){
  string line;
  cout << endl << "Log: " << (URL + file).c_str() << endl;
  ifstream myfile ((URL + file).c_str());
  if (myfile.is_open())
  {
    while ( getline (myfile,line) ) {
      cout << line << '\n';
    }
    myfile.close();
  }

  else cerr << "Unable to open file"; 

}

