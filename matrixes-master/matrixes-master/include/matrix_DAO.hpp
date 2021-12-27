#ifndef MATRIX_DAO_HPP    
#define MATRIX_DAO_HPP

template <typename T>
int save_matrix(Matrix<T>* matrix);
Matrix<float> get_matrix_by_id(int matrix_id, int user_id);
std::vector<Matrix<float> > get_matrixes(string nombre, int id, int owner_id);
void save_matrix_content(int id_matrix, int col_index, int row_index, float value);
bool update_matrix_content(int id_matrix, int col_index, int row_index, float value);
#endif
