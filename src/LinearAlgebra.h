/***************************************************************
 * Name:      LinearAlgebra.h
 * Purpose:   ERYA Linear Algebra library
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef LINEARALGEBRA_H_INCLUDED
#define LINEARALGEBRA_H_INCLUDED
#include "ERYAPIXE.h"
#include <cmath>

// Define a dynamic allocable vector for several purposes
class Vector
{
 private:
 double *mainarray;
 int Elements;
 bool status;
 public:
 Vector() {Elements = 0;};
 Vector(int);
 bool GetStatus();
 void SetStatus(bool);
 bool GetOverflow();
 int GetNumberElements();
 double GetValue(int);
 double SetValue(int,double);
 double InfNorm();
 double Maximum();
 double Minimum();
 double EuclideNorm();
 int AtMaximum();
 int AtMinimum();
};

class IntegerVector
{
 private:
 int *mainarray;
 int Elements;
 public:
 IntegerVector() {Elements = 0;};
 IntegerVector(int);
 bool GetOverflow();
 int GetNumberElements();
 int GetValue(int);
 int SetValue(int,double);
};

// Define a dynamic allocable matrix for all purposes
class Matrix
{
  private:
  double **grid;
  int Rows,Columns;
  bool status;
  public:
  Matrix() {Rows = 0; Columns = 0;};
  Matrix(int,int);
  bool GetStatus();
  void SetStatus(bool);
  bool GetOverflow();
  int GetNumberRows();
  int GetNumberColumns();
  double GetValue(int,int);
  double SetValue(int,int,double);
  bool SwapRows(int,int);
  bool ReducePivot(int,int);
};

// Operations with matrices
Matrix MatrixTranspose(Matrix);
Matrix MatrixAddition(Matrix,Matrix);
Matrix MatrixSubtraction(Matrix,Matrix);
Matrix MatrixProduct(Matrix,Matrix);
Matrix MatrixSolve(Matrix,Matrix);
Matrix MatrixIdentity(int);
Matrix MatrixScalarProduct(double,Matrix);

// Operations with vectors
Vector VectorSum (Vector,Vector);
Vector VectorMinus (Vector,Vector);
Matrix GetMatrix (Vector);
Vector GetVector (Matrix);
Vector VectorScalarProduct(double,Vector);
Vector VectorNull(int);


#endif // LINEARALGEBRA_H_INCLUDED
