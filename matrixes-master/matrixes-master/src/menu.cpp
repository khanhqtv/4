#include <iostream>
#include <LibConstants.hpp>
using namespace std;
void print_title(int matrix_id){
  if(matrix_id <= 0){
  cout << " ======================" << endl;
  cout << "     == OPCIONES ==" << endl;
  cout << " ======================" << endl << endl;
  } else {
    cout << " ======================" << endl;
    cout << "     == MATRIZ::" << matrix_id << " ==" << endl;
    cout << " ======================" << endl << endl;
  }
}

void print_menu(int matrix_id){
  print_title(matrix_id);

  cout << " " << MenuValuesConstants::MostrarMenu << ".-  Mostar Menú" << endl;
  cout << " " << MenuValuesConstants::MostrarMatriz << ".-  Mostar Matriz" << endl;
  cout << " " << MenuValuesConstants::BuscarMatriz << ".-  Buscar Matrices Guardadas" << endl;
  cout << " " << MenuValuesConstants::NuevaMatriz << ".-  Nueva Matriz" << endl;
  cout << " " << MenuValuesConstants::SeleccionarMatriz << ".-  Seleccionar Matriz" << endl;
  cout << " " << MenuValuesConstants::Historico << ".-  Mostrar Histórico de Movimientos" << endl;
  cout << " " << MenuValuesConstants::Salir << ".-  Salir" << endl;
  if(matrix_id > 0){
    cout << " " << MenuValuesConstants::ModifCelda << ".- Modificar Celda" << endl;
    cout << " " << MenuValuesConstants::SumaMatrices << ".- Sumar valores de dos Matrices" << endl;
    cout << " " << MenuValuesConstants::ModifFila << ".- Modificar Fila" << endl;
    cout << " " << MenuValuesConstants::IntFilas << ".- Intercambiar Filas" << endl;
      cout << " " << MenuValuesConstants::ModifColumna << ".- Modificar Columna" << endl;
    cout << " " << MenuValuesConstants::OrdenarMatriz << ".- Ordenar Matriz" << endl;
     
  }
  cout << endl;
}


