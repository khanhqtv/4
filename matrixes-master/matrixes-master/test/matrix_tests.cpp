
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

#define USER_ID 1

void seleccionar_matriz_id_correcto();
void seleccionar_matriz_id_no_existente();
void seleccionar_matriz_id_negativo();

void crear_matriz();
void crear_matriz_fuera_limites();
void crear_matriz_dimensiones_negativos();

void modificar_fila_valores_correctos();
void modificar_fila_columna_negativa();
void modificar_fila_valor_fuera_limites();

void intercambiar_filas_existentes();
void intercambiar_filas_inexistentes();

int main(void){
  seleccionar_matriz_id_correcto();
  seleccionar_matriz_id_no_existente();
  seleccionar_matriz_id_negativo();
  crear_matriz();
  crear_matriz_fuera_limites();
  crear_matriz_dimensiones_negativos();
  modificar_fila_valores_correctos();
  modificar_fila_columna_negativa();
  modificar_fila_valor_fuera_limites();
  intercambiar_filas_existentes();
  intercambiar_filas_inexistentes();
}

void seleccionar_matriz_id_correcto(){
  int expected = 85;
  int matrix_id = 85;
  int actual = select_matrix(matrix_id, USER_ID);
  
  assert(expected == actual);
}

void seleccionar_matriz_id_no_existente(){
  int expected = -1;
  int matrix_id = -1;
  int actual = actual = select_matrix(matrix_id, USER_ID);
  assert(expected == actual);
}

void seleccionar_matriz_id_negativo(){
  int expected = -1;
  int matrix_id = -33;
  int actual = actual = select_matrix(matrix_id, USER_ID);
  assert(expected == actual);
}

void crear_matriz() {
  int cols = 5;
  int rows = 5;
  int user_id = 1;
  string name = "test";
  int result = save_matrix(cols, rows, USER_ID, name);
  assert(result > 0);
}

void crear_matriz_fuera_limites(){
  int cols = 55;
  int rows = 55;
  int user_id = 1;
  string name = "test";
  int result = save_matrix(cols, rows, user_id, name);
  assert(result == -1);
}

void crear_matriz_dimensiones_negativos(){
  int cols =-5;
  int rows = -5;
  int user_id = 1;
  string name = "test";
  int result = save_matrix(cols, rows, user_id, name);
  assert(result == -1);
}

void modificar_fila_valores_correctos(){
  int user_id = 1;
  int matrix_id = 85;
  int cols = 3;
  int selected_row = 0;
  float value = 10.0;
  
  for(int pos = 0; pos < cols; pos++){
    assert(update_cell(matrix_id, pos, selected_row, value++));
  }
  value = 10.0;
  Matrix<float> matrix = get_matrix_by_ID(matrix_id, USER_ID);
  
  for(int pos = 0; pos < matrix.getCols(); pos++){
    assert(matrix.operator()(selected_row, pos) == value);
    value++;
  }
}

void modificar_fila_columna_negativa(){
  int user_id = 1;
  int matrix_id = 85;
  int cols = -3;
  int selected_row = 0;
  float value = 10.0;
  
  for(int pos = 0; pos < cols; pos++){
    assert(update_cell(matrix_id, pos, selected_row, value++) == false);
  }

  value = 10.0;
  Matrix<float> matrix = get_matrix_by_ID(matrix_id, USER_ID);
  
  for(int pos = 0; pos < matrix.getCols(); pos++){
    assert(matrix.operator()(selected_row, pos) == value);
    value++;
  }
}

void modificar_fila_valor_fuera_limites(){
  int user_id = 1;
  int matrix_id = 85;
  int cols = 3;
  int selected_row = 220;
  float value = 10.0;
  
  for(int pos = 0; pos < cols; pos++){
    assert(update_cell(matrix_id, pos, selected_row, value++) == false);
  }
}

void intercambiar_filas_existentes(){
  int r1 = 0;
  int r2 = 1;
  int matrix_id = 85;

  Matrix<float> matrix = get_matrix_by_ID(matrix_id, USER_ID);
  std::vector<float> v1(matrix.getCols()); 
  std::vector<float> v2(matrix.getCols());

  for(int pos = 0; pos < matrix.getCols(); pos++){
    v1[pos] =  matrix.operator()(r1, pos);
    v2[pos] =  matrix.operator()(r2, pos);
  }
  bool resultado = exchange_rows(r1, r2, matrix_id, USER_ID);
  assert(resultado); 

  for(int pos = 0; pos < matrix.getCols(); pos++){
    assert(matrix.operator()(r1, pos) == v2[pos]);
    assert(matrix.operator()(r2, pos) == v1[pos]);
  }  
}

void intercambiar_filas_inexistentes(){
  int r1 = 4;
  int r2 = 5;
  int matrix_id = 85;

  Matrix<float> matrix = get_matrix_by_ID(matrix_id, USER_ID);
  std::vector<float> v1(matrix.getCols()); 
  std::vector<float> v2(matrix.getCols());

  for(int pos = 0; pos < matrix.getCols(); pos++){
    v1[pos] =  matrix.operator()(r1, pos);
    v2[pos] =  matrix.operator()(r2, pos);
  }
  bool resultado = exchange_rows(r1, r2, matrix_id, USER_ID);
  assert(!resultado);
}

