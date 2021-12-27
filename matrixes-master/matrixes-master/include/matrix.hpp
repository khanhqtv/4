#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <algorithm>
template <typename T>
class Matrix
{
private:
  int mCols;
  int mRows;
  string mName;
  int mOwnerID;
  int mID;
  //T mGrid[mRows * mCols];
  std::vector<T> mGrid; 

public:
 
  Matrix(int rows, int cols, int ownerID, string name);
  ~Matrix(void);
  T& operator ()(int row, int column);
  float assign (int row, int column, float value);
  int getCols();
  int getRows();
  int getID();
  void setID(int id);
  string getName();
  int getOwnerID();
  //T *getContent();
  //void setContent(T *content);
  void sort();
};

#endif 
