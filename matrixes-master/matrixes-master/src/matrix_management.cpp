#include <matrix_management.hpp>
#include "menu.cpp"
#include <cstring>
#include <stdlib.h>     /* atoi */
#include "matrix.cpp"
#include "matrix_DAO.cpp"
#include <LibConstants.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <errno.h>
#include "logger.cpp"
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <pthread.h>

using namespace std;

float shared_data = 0;
pthread_t tid[2];
pthread_mutex_t lock;

struct arg_struct {
    int selected_id;
    int user_id;
};

void* matrix_sum(void *arg){
  struct arg_struct *args = (struct arg_struct *)arg;
  pthread_mutex_lock(&lock);
  Matrix<float> matrix = get_matrix_by_ID(args->selected_id, args->user_id);
   
  for(int r = 0; r < matrix.getRows();r++){
    for(int c = 0; c <  matrix.getCols();c++){
   
      shared_data += matrix.operator()(r,c);
    }
  }  
  pthread_mutex_unlock(&lock);
  return NULL;
}

namespace Management{

  int run_option(int option, int matrix_id, int user_id){
    int selected_id = matrix_id;
    if(matrix_id <= 0 && option > 6){
      cout << "Opción no válida." << endl;
    } else {  
      switch(option){
      case 0:{
	// Do nothing
      }
	break;
      case 1: 
	{
	  if(matrix_id <= 0){
	    cout << "Primero debes seleccionar una matriz" << endl;
	  } else {
	    cout << "  Mostrando contenido de matriz con id: " << matrix_id << endl;
	    Matrix<float> matrix = get_matrix_by_ID(matrix_id, user_id);
	    for(int r = 0; r < matrix.getRows();r++){
	      cout << "  |";
	      for(int c = 0; c <  matrix.getCols();c++){
		cout << fixed << setprecision(2);
		cout <<  matrix.operator()(r,c) << " | " ;
		}
		    cout << endl;
	    }	   
	  }
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();	  
	}
	break;
  
	case 2 : 
	  { 
	    cout << "  Buscar por:" << endl;
	    cout << "[1] Identificador" << endl;
	    cout << "[2] Nombre" << endl;
	    cout << "[0] Volver menú" << endl;
	    int opt = 0;
	    while(!(cin >> opt) || (opt > 2 || opt < 0)){
	      cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	      cin.clear();
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }
	    string nombre_a_buscar = "";
	    int id_a_buscar = 0;
	    if(opt == 1){
	      cin.ignore(1024, '\n');
	      id_a_buscar = atoi(UtilsLibrary::get_user_input(" Introduce identificador a buscar: ", false, 5).c_str());
	    } else if(opt == 2){
	      cin.ignore(1024, '\n');
	      nombre_a_buscar = UtilsLibrary::get_user_input(" Introduce cadena a buscar: ", false, 20);
	    } else {
	      break;
	    }
	    	    
	    vector<Matrix<float> > result = get_matrixes(nombre_a_buscar, id_a_buscar, user_id);

	    for(int i = 0; i < result.size(); i++) {
	      Matrix<float> n = result.at(i);
	      std::cout << "Matriz con ID: " << n.getID() << " Nombre: "<< n.getName() << endl;
	    }
	   
	    cout << "Tamaño resultado " << result.size();

	    cout<< endl << " Pulse enter para continuar...";
	    cin.get();
	  }
	  break;
  
	case 3: 
	  {
	    string name = UtilsLibrary::get_user_input(" Introduce Nombre Para la Matriz: ", false, 20);
	    cout << " Introduce Número de Columnas: ";
	    int cols = 0;    
	    while(!(cin >> cols) || (cols > AppValues::MAX_COLS)){
	      cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	      cin.clear();
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }
	    cout << " Introduce Número de Filas: ";
	    int rows = 0;
	    while(!(cin >> rows) || (rows > AppValues::MAX_ROWS)){
	      cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	      cin.clear();
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }
	    selected_id = save_matrix(rows, cols, user_id, name);
	    cin.ignore(1024, '\n');
	    if(selected_id > 0){
	      cout << " Matriz creada con ID: " << selected_id << endl;
	      std::ostringstream stream;
	      stream << "Created Matrix with ID: " << selected_id;
	      std::string message = stream.str();
	      log(message);
	    } else {
	      cerr << "Matriz no creada";
	      log_error("No Matriz Creada");
	    }	
	    cout << " Pulse enter para continuar...";
	    cin.get();    
	  }
	  break;  
      case 4: {
	int id = atoi(UtilsLibrary::get_user_input(" Introduce el id de la matriz que quieres seleccionar: ", false, 5).c_str());
	int id_devuelta = select_matrix(id, user_id);
	if(id_devuelta == -1){
	  cerr << "No ha sido posible seleccionar esa matriz" << endl;
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();
	} else {
	  selected_id = id_devuelta;
	}
      }
	break;
      case 5 :
	{
	  string nombre_log = UtilsLibrary::get_user_input(" Introduce nombre del log: ", false, 10);
	  print_logger_content(nombre_log);
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();
	}
	break;
      case 6:
	{
	  selected_id =  AppValues::EXIT_CODE;
	}
	break;
      case 7:
	{
	  Matrix<float> matrix = get_matrix_by_ID(selected_id, user_id);
	  cout << " Introduce Número de Columna: ";
	  int col = 0;    
	  while(!(cin >> col) || (col > matrix.getCols() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  
	  cout << " Introduce Número de Fila: ";
	  int row = 0;
	  while(!(cin >> row) || (row > matrix.getRows() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }

	  cout << " Introduce valor: ";
	  float value = 0;
	  while(!(cin >> value) || value > 100){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	 	  
	  bool resultado = update_matrix_content(selected_id, col, row, value);
	  if(!resultado){
	    std::ostringstream stream;
	    stream << "Error al modificar columna: " << col << ", fila "<< row << " de la matriz " << selected_id << endl;
	    cerr << stream.str() << endl;
	    log_error(stream.str());
	  } else {
	    std::ostringstream stream;
	    stream << "Posición (" << col << ", " << row << ") de matriz "  << selected_id << " modificada con valor " << value;
	    std::string message = stream.str();
	    log(message);
	  }
	  cin.ignore(1024, '\n');
	  cout<< endl << " Pulse enter para continuar...";	  
	  cin.get();
	}
	break;
      case 8:
	{
	  cout << " Introduce id matriz 1: ";
	  int id_1 = 0;    
	  while(!(cin >> id_1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  cout << " Introduce id matriz 2: ";
	  int id_2 = 0;    
	  while(!(cin >> id_2)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  if (pthread_mutex_init(&lock, NULL) != 0){
	    cout << "\n mutex init failed\n" << endl;
	    return selected_id;
	  }
	  int i = 0;
	  int err;
	  //while(i < 2) {
	
	  struct arg_struct args_1;// = malloc(sizeof(struct arg_struct));
	  args_1.selected_id = id_1;
	  args_1.user_id = user_id;
	 
	  struct arg_struct args_2;// = malloc(sizeof(struct arg_struct));
	  args_2.selected_id = id_2;
	  args_2.user_id = user_id;
	
	  pthread_create(&(tid[0]), NULL, &matrix_sum, &args_1);
	 
	  pthread_create(&(tid[1]), NULL, &matrix_sum, &args_2);
	
	  pthread_join(tid[0], NULL);
	 
	  pthread_join(tid[1], NULL);

	  pthread_mutex_destroy(&lock);
	  cout << "Suma total: " << shared_data << endl;
	  shared_data = 0;
	  cin.ignore(1024, '\n');
	  cout<< endl << " Pulse enter para continuar...";	  
	  cin.get();
	}
	break;
      case 9: 
	{	 
	  Matrix<float> matrix = get_matrix_by_ID(selected_id, user_id);
	  cout << " Introduce Número de Fila a modificar: ";
	  int row = 0;
	  while(!(cin >> row) || (row > matrix.getRows() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  for(int pos = 0; pos < matrix.getCols(); pos++){
	    std::ostringstream stream;
	    stream << " -->  Introduce nuevo valor en posición " << pos << ": ";
	    cout << stream.str();
	    float value = 0;
	    while(!(cin >> value) || value > 100){
	      cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	      cin.clear();
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }
	    bool resultado = update_matrix_content(selected_id, pos, row, value);	    
	    if(!resultado){
	      std::ostringstream stream;
	      stream << "Error al modificar columna: " << pos << ", fila " << row << " de la matriz " << selected_id << endl;
	    cerr << stream.str() << endl;
	    log_error(stream.str());
	    break;
	  } else {
	    std::ostringstream stream;
	    stream << "Posición (" << pos << ", " << row << ") de matriz  " << selected_id << " modificada con valor " << value;
	      std::string message = stream.str();
	      log(message);
	    }
	  }
	  cin.ignore(1024, '\n');	  
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();
	}
	break;
      case 10:
	{
	  Matrix<float> matrix = get_matrix_by_ID(selected_id, user_id);
	  cout << " Introduce Número de Fila número 1: ";
	  int r1 = 0;
	  while(!(cin >> r1) || (r1 > matrix.getRows() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  cout << " Introduce Número de Fila número 2: ";
	  int r2 = 0;
	  while(!(cin >> r2) || (r2 > matrix.getRows() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  bool resultado = exchange_rows(r1, r2, selected_id, user_id);

	  if(!resultado){
	    std::ostringstream stream;
	    stream << "Error:  intercambiar filas: " << r1 << ", " << r2 << " de la matriz " << selected_id << endl;
	    cerr << stream.str() << endl;
	    log_error(stream.str());
	  } else {
	    std::ostringstream stream;
	    stream << "Filas (" << r1 << ", " << r2 << ") de matriz  " << selected_id << " intercambiadas" << endl;
	    std::string message = stream.str();
	    log(message);
	  }
	  cin.ignore(1024, '\n');	  
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();
	}
	break;
      case 11: 
	{	 
	  Matrix<float> matrix = get_matrix_by_ID(selected_id, user_id);
	  cout << " Introduce Número de columna a modificar: ";
	  int col = 0;
	  while(!(cin >> col) || (col > matrix.getCols() - 1)){
	    cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	  }
	  for(int pos = 0; pos < matrix.getRows(); pos++){
	    std::ostringstream stream;
	    stream << " -->  Introduce nuevo valor en posición " << pos << ": ";
	    cout << stream.str();
	    float value = 0;
	    while(!(cin >> value) || value > 100){
	      cout << " Formato Incorrecto!! Vuelve a introducirlo : ";
	      cin.clear();
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }
	    bool resultado = update_matrix_content(selected_id, col, pos, value);	    
	    if(!resultado){
	      std::ostringstream stream;
	      stream << "Error al modificar columna: " << col << ", fila " << pos << " de la matriz " << selected_id << endl;
	    cerr << stream.str() << endl;
	    log_error(stream.str());
	  } else {
	    std::ostringstream stream;
	    stream << "Posición (" << col << ", " << pos << ") de matriz  " << selected_id << " modificada con valor " << value;
	      std::string message = stream.str();
	      log(message);
	    }
	  }
	  cin.ignore(1024, '\n');	  
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get();
	}
	break; 
      case 12:
	{
	  Matrix<float> matrix = get_matrix_by_ID(selected_id, user_id);
	  matrix.sort();
	  for(int r = 0; r < matrix.getRows();r++){
	    cout << "  |";
	    for(int c = 0; c <  matrix.getCols();c++){
	      update_matrix_content(selected_id, c, r,  matrix.operator()(r,c));
	      cout << fixed << setprecision(2);
	      cout <<  matrix.operator()(r,c) << " | " ;
	    }
	    cout << endl;
	  }
	  cout<< endl << " Pulse enter para continuar...";
	  cin.get(); 
	}
	break;
      }    
    }
    return selected_id;
  }
  
  int run_app(int matrix_id, int user_id){
      system("clear");
      print_menu(matrix_id);
      int selected_option = -1;
      selected_option = atoi(UtilsLibrary::get_user_input(" INTRODUCE UNA OPCIÓN: ", false, 2).c_str());
      return run_option(selected_option, matrix_id, user_id);
    }
  }

int select_matrix(int matrix_id, int user_id){
  int result = -1;
  Matrix<float> matrix = get_matrix_by_ID(matrix_id, user_id);
  if(matrix.getID() == matrix_id){
   result = matrix.getID();
  }
  return result;
}

int save_matrix(int rows, int cols, int user_id, string name){
  int result = -1;
  if((rows > 0 && rows < 11) && 
     (cols > 0 && cols < 11) && 
     user_id > 0 && sizeof name > 0){
    Matrix<float> mMatrix(rows, cols, user_id, name);
    Matrix<float> *c = &mMatrix;
    result =  save_matrix(c);
  }
  return result;
}

bool update_cell(int selected_id, int col, int row, float value){
  bool result = false;
  if(selected_id > 0 && 
     (col >= 0 && col <= AppValues::MAX_COLS) &&
     (row >= 0 && row <= AppValues::MAX_ROWS)){
    result = update_matrix_content(selected_id, col, row, value);  
  }
  return result;
}

bool exchange_rows(int r1, int r2, int matrix_id, int user_id){
  Matrix<float> matrix = get_matrix_by_ID(matrix_id, user_id);
  bool result = true;
  std::vector<float> v1(matrix.getCols()); 
  std::vector<float> v2(matrix.getCols());

  for(int pos = 0; pos < matrix.getCols(); pos++){
    v1[pos] =  matrix.operator()(r1, pos);
    v2[pos] =  matrix.operator()(r2, pos);
  }
  
  for(int pos = 0; pos < matrix.getCols(); pos++){
    float value1 = v1[pos];
    float value2 = v2[pos];
   
    if(update_cell(matrix_id, pos, r1, value2) && update_cell(matrix_id, pos, r2, value1)){
      continue;
    } else {
      cout << "Error "<< endl;
      result = false;
      break;
    }
  }
  return result;
}
