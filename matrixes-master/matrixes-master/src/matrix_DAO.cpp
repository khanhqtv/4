#include <matrix_DAO.hpp>
#include <pqxx/pqxx>
#include <iostream>

void save_matrix_content(pqxx::transaction_base &txn, int id_matrix, int col_index, int row_index, float value){
  txn.exec(
	   "INSERT INTO matrix_content(id_matrix, col_index, row_index, value) "
	   "VALUES (" +
	   txn.quote(id_matrix) + ", " +
	   txn.quote(col_index) + ", " +
	   txn.quote(row_index) + ", " +
	   txn.quote(value) +
	   ")");
}

void save_matrix_content(int id_matrix, int col_index, int row_index, float value){
  pqxx::connection c("dbname=matrixes user=matrixuser");
  pqxx::work txn(c); 
  save_matrix_content(txn, id_matrix, col_index, row_index, value);
  txn.commit();
}

bool update_matrix_content(int id_matrix, int col_index, int row_index, float value){
  try{
    pqxx::connection c("dbname=matrixes user=matrixuser");
    pqxx::work txn(c);
    txn.exec(
	     "UPDATE matrix_content SET value = " + txn.quote(value) +
	     "WHERE id_matrix = " + txn.quote(id_matrix) +
	     " AND col_index = " + txn.quote(col_index) + 
	     " AND row_index = " + txn.quote(row_index));
    
    txn.commit();
    return true;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
}

int save_matrix(Matrix<float> *matrix){
  int id = -1;
  try{
    pqxx::connection c("dbname=matrixes user=matrixuser");
    pqxx::work txn(c); 
    pqxx::result r = txn.exec(
			      "INSERT INTO matrixes "
			      "(name, owner_id, number_cols, number_rows) VALUES("	   
			      + txn.quote(matrix->getName()) + ", "
			      + txn.quote(matrix->getOwnerID()) + ", "
			      + txn.quote(matrix->getCols()) + ", "
			      + txn.quote(matrix->getRows()) +	   
			      ") returning id");
    id = r[0][0].as<int>();
    for(int r = 0; r < matrix->getRows();r++){
      for(int c = 0; c <  matrix->getCols();c++){
      	matrix->assign(r,c, 0.00f);
	save_matrix_content(txn,id, c, r, 0.00f);
      }
    }
    matrix->setID(id);
    txn.commit();
  } catch (const std::exception &e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }
  return id;
}

Matrix<float> get_matrix_by_ID(int matrix_id, int user_id){
  try
    {
      pqxx::connection c("dbname=matrixes user=matrixuser");
      pqxx::work txn(c);

      pqxx::result r = txn.exec(
				"SELECT number_rows, number_cols, owner_id, name, id " 
				"FROM matrixes " 			   
				"WHERE id=" + txn.quote(matrix_id) + 
				" AND owner_id = " + txn.quote(user_id));
 
      if(r.size() == 1){
	Matrix<float> mMatrix(r[0][0].as<int>(),
			      r[0][1].as<int>(),
			      r[0][2].as<int>(),
			      r[0][3].as<string>());
	mMatrix.setID(r[0][4].as<int>());
	pqxx::result content = txn.exec(
				"SELECT col_index, row_index, value " 
				"FROM matrix_content " 			   
				"WHERE id_matrix =" + txn.quote(matrix_id));
	
	for ( pqxx::result::const_iterator row = content.begin(); row != content.end();++row){
	  mMatrix.assign(row[1].as<int>(), row[0].as<int>(), row[2].as<float>());
	}
	return mMatrix;
      }
    } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  Matrix<float> emptyMatrix(0, 0, 0,"");
  return emptyMatrix;
}

std::vector<Matrix<float> > get_matrixes(string nombre, int id, int owner_id){
   pqxx::connection c("dbname=matrixes user=matrixuser");
   pqxx::work txn(c);
   string params = " AND owner_id = " +  txn.quote(owner_id);
   if(strlen(nombre.c_str()) > 0){
     params += " AND name like " + txn.quote('%' + nombre + '%');
   } else if (id > 0){
     params += " AND id = " + txn.quote(id);
   }
   pqxx::result r = txn.exec(
			     "SELECT number_rows, number_cols, owner_id, name, id " 
			     "FROM matrixes " 			   
			     "WHERE 0 = 0" + params);
   std::vector<Matrix<float> > matrixes(r.size());

   int pos = 0;
   for ( pqxx::result::const_iterator row = r.begin(); row != r.end();++row){
     Matrix<float> mMatrix(row[0].as<int>(),
			   row[1].as<int>(),
			   row[2].as<int>(),
			   row[3].as<string>());
     mMatrix.setID(row[4].as<int>());

     matrixes.at(pos)=  mMatrix;
     pos = pos + 1;
   }
   return matrixes;
}
