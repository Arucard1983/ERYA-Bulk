/***************************************************************
 * Name:      LinearAlgebra.cpp
 * Purpose:   ERYA Linear Algebra implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "LinearAlgebra.h"

 // Vector Constructor
 Vector::Vector(int x)
 {
  if(x>0)
  {
   Elements = x;
   mainarray = new double [Elements];
  }
  else
  {
   Elements = 1;
   mainarray = new double [1];
   mainarray[0] = NAN;
  }
 }

 IntegerVector::IntegerVector(int x)
 {
  if(x>0)
  {
   Elements = x;
   mainarray = new int [Elements];
  }
  else
  {
   Elements = 1;
   mainarray = new int [1];
   mainarray[0] = 0;
  }
 }

// Evaluate vector computation  successfulness
bool Vector::GetStatus()
{
  return status;
}

// Usefull for mark a vectorial computation sucess or failure
void Vector::SetStatus(bool flag)
{
   status = flag;
}

// Check if all vector members are finite. At least one of them will trigger an overflow flag
bool Vector::GetOverflow()
{
 for(int i=0; i<this->GetNumberElements(); i++)
 {
   if(!(std::isfinite(this->GetValue(i))))
    return true;
 }
 return false;
}

bool IntegerVector::GetOverflow()
{
 for(int i=0; i<this->GetNumberElements(); i++)
 {
   if(!(std::isfinite(this->GetValue(i))))
    return true;
 }
 return false;
}

 // Get the Number of Elements in a Vector
int Vector::GetNumberElements()
{
  return Elements;
}

int IntegerVector::GetNumberElements()
{
  return Elements;
}

 // Obtain the numerical value of the selected vector adress
double Vector::GetValue(int nElement)
{
  if(nElement<0 || nElement>=Elements)
   return NAN;
  else
   return mainarray[nElement];
}

int IntegerVector::GetValue(int nElement)
{
  if(nElement<0 || nElement>=Elements)
   return 0;
  else
   return mainarray[nElement];
}

// Write a value to the selected vector adress
double Vector::SetValue(int nElement, double value)
{
   if(nElement<0 || nElement>=Elements)
    {
     return NAN;
    }
    else
    {
    mainarray[nElement] = value;
    return mainarray[nElement];
    }
}

int IntegerVector::SetValue(int nElement, double value)
{
   if(nElement<0 || nElement>=Elements)
    {
     return 0;
    }
    else
    {
    mainarray[nElement] = value;
    return mainarray[nElement];
    }
}


// Calculate the infinite norm
double Vector::InfNorm()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = std::abs(mainarray[0]);
   for (int i=0;i<Elements;i++)
   {
     if (temp < std::abs(mainarray[i]))
     {
       temp = std::abs(mainarray[i]);
     }
   }
   return std::abs(temp);
  }
  else
  {
   return 0;
  }
}

// Calculate the the maximum of a vector
double Vector::Maximum()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = mainarray[0];
   for (int i=0;i<Elements;i++)
   {
     if (temp < mainarray[i])
     {
       temp = mainarray[i];
     }
   }
   return temp;
  }
  else
  {
   return 0;
  }
}

// Calculate the the minimum of a vector
double Vector::Minimum()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = mainarray[0];
   for (int i=0;i<Elements;i++)
   {
     if (temp > mainarray[i])
     {
       temp = mainarray[i];
     }
   }
   return temp;
  }
  else
  {
   return 0;
  }
}

// Return the position of the maximum
int Vector::AtMaximum()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = mainarray[0];
   int position = 0;
   for (int i=0;i<Elements;i++)
   {
     if (temp < mainarray[i])
     {
       temp = mainarray[i];
       position = i;
     }
   }
   return position;
  }
  else
  {
   return 0;
  }
}

// Return the position of the minimum
int Vector::AtMinimum()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = mainarray[0];
   int position = 0;
   for (int i=0;i<Elements;i++)
   {
     if (temp > mainarray[i])
     {
       temp = mainarray[i];
       position = i;
     }
   }
   return position;
  }
  else
  {
   return 0;
  }
}


// Calculate the euclidian norm
double Vector::EuclideNorm()
{
  int vectorsize = Elements;
  if (Elements > 0)
  {
   double temp = 0;
    for (int i=0;i<Elements;i++)
   {
    temp = temp + mainarray[i]*mainarray[i];
   }
   return std::sqrt(temp);
  }
  else
  {
   return 0;
  }
}

// Matrix Constructor
Matrix::Matrix(int x, int y)
{
  if(x>0 && y>0)
  {
   Rows = x;
   Columns = y;
   grid = new double *[Rows];
   for (int i=0; i<Rows; i++)
   {
   grid[i] = new double [Columns];
   }
  }
  else
  {
   Rows = 1;
   Columns = 1;
   grid = new double *[1];
   grid[0] = new double [1];
   grid[0][0] = NAN;
  }
}

// Evaluate matrix computation  successfulness
bool Matrix::GetStatus()
{
  return status;
}

// Usefull for mark a matrix computation sucess or failure
void Matrix::SetStatus(bool flag)
{
   status = flag;
}

// Verifiy if all matrix members are finite numbers
bool Matrix::GetOverflow()
{
 for(int i=0; i<this->GetNumberRows(); i++)
 {
   for(int j=0; j<this->GetNumberColumns(); j++)
   {
    if(!(std::isfinite(this->GetValue(i,j))))
    return true;
   }
 }
 return false;
}

// Get the Number of Rows of a Matrix
int Matrix::GetNumberRows()
{
  return Rows;
}

// Get the Number of Columns of a Matrix
int Matrix::GetNumberColumns()
{
  return Columns;
}

// Obtain the numerical value of the selected matrix adress
double Matrix::GetValue(int nRow, int nColumn)
{
  if( nRow<0 || nColumn<0 || nRow>=Rows || nColumn>=Columns)
   return NAN;
    else
   return grid[nRow][nColumn];
}

// Write a value to the selected matrix adress
double Matrix::SetValue(int nRow, int nColumn, double value)
{
  if( nRow<0 || nColumn<0 || nRow>=Rows || nColumn>=Columns)
    {
     return NAN;
    }
    else
    {
    grid[nRow][nColumn] = value;
    return grid[nRow][nColumn];
    }
}

// Exhange two selected rows, using a temporary vector
bool Matrix::SwapRows(int FirstRow, int SecondRow)
{
  if( FirstRow>=0 && SecondRow>=0 && FirstRow<Rows && SecondRow<Rows)
  {
   double *temp;
   temp = new double [Columns];
   for(int i = 0; i < Columns; i++)
     {
       temp[i] = grid[SecondRow][i];
       grid[SecondRow][i] = grid[FirstRow][i];
       grid[FirstRow][i] = temp[i];
     }
    return true;
  }
  else
  {
   return false;
  }
}

// Reduce the matrix, using a pivot adress and target adress
bool Matrix::ReducePivot(int i, int ni)
{
  if( i>=0 && ni>=0 && i<Rows && ni<Rows)
   {
    if (i < ni)
    {
     double pivot = -1 * grid[ni][i] / grid[i][i];
     for (int j = 0; j < Columns; j++)
     {
      grid[ni][j] = grid[ni][j] + pivot * grid[i][j];
     }
    }
    else if (i == ni)
    {
     for (int j = 0; j < Columns; j++)
     {
      grid[ni][j] = grid[ni][j];
     }
    }
    else
    {
     double pivot = -1 * grid[i][i] / grid[i][ni];
     for (int j = 0; j < Columns; j++)
     {
      grid[i][j] = grid[i][j] + pivot * grid[ni][j];
     }
    }
   return true;
  }
  else
  {
   return false;
  }
}

// Get the Transpose of a matrix
Matrix MatrixTranspose (Matrix A)
{
Matrix C = Matrix(A.GetNumberColumns(),A.GetNumberRows());
for(int i = 0; i < A.GetNumberRows(); i++)
   {
     for(int j = 0; j < A.GetNumberColumns(); j++)
     {
        double temp = A.GetValue(i,j);
        C.SetValue(j,i,temp);
     }
   }
   C.SetStatus(C.GetOverflow());
   return C;
}



// Addition of two compatible matrices (requires the same number of rows and columns)
Matrix MatrixAddition(Matrix A, Matrix B)
{
 Matrix C = Matrix(A.GetNumberRows(),A.GetNumberColumns());
 if(A.GetNumberRows() == B.GetNumberRows() && A.GetNumberColumns() == B.GetNumberColumns())
 {
   for(int i = 0; i < C.GetNumberRows(); i++)
   {
     for(int j = 0; j < C.GetNumberColumns(); j++)
     {
        C.SetValue(i,j, A.GetValue(i,j) + B.GetValue(i,j));
     }
   }
   C.SetStatus(true); // Addition successful
   return C;
 }
  else
  {
    C.SetStatus(false); // Cannot add two different rank matrices
    return C;
  }
}

// Subtraction of two compatible matrices (requires the same number of rows and columns)
Matrix MatrixSubtraction(Matrix A, Matrix B)
{
 Matrix C = Matrix(A.GetNumberRows(),A.GetNumberColumns());
 if(A.GetNumberRows() == B.GetNumberRows() && A.GetNumberColumns() == B.GetNumberColumns())
 {
   for(int i = 0; i < C.GetNumberRows(); i++)
   {
     for(int j = 0; j < C.GetNumberColumns(); j++)
     {
        C.SetValue(i,j, A.GetValue(i,j) - B.GetValue(i,j));
     }
   }
   C.SetStatus(true); // Subtraction successful
   return C;
 }
  else
  {
    C.SetStatus(false); // Cannot subtract two different rank matrices
    return C;
  }
}

// Multiplication of two compatible matrices (The number of columns of left matrix should be equal to the number of rows of the right matrix)
Matrix MatrixProduct(Matrix A, Matrix B)
{
 Matrix C = Matrix(A.GetNumberRows(),B.GetNumberColumns());
 if(B.GetNumberRows() == A.GetNumberColumns())
 {
   for(int i = 0; i < C.GetNumberRows(); i++)
   {
     for(int j = 0; j < C.GetNumberColumns(); j++)
     {
        double sum = 0;
       for(int k = 0; k < A.GetNumberColumns(); k ++)
       {
        sum = sum + A.GetValue(i,k) * B.GetValue(k,j);
       }
       C.SetValue(i,j,sum);
     }
   }
   C.SetStatus(true); // Product successful
   return C;
 }
  else
  {
    C.SetStatus(false); // Invalid Matrix
    return C;
  }
}


// Solve a System of Linear Equations
Matrix MatrixSolve (Matrix A, Matrix B)
{
 Matrix AB = Matrix(A.GetNumberRows(),A.GetNumberColumns() + B.GetNumberColumns());
 Matrix C = Matrix(AB.GetNumberRows(),1);
 // Check compability first
 if (AB.GetNumberRows() == AB.GetNumberColumns() - 1)
 {
  for (int i = 0; i < AB.GetNumberRows(); i++) // Begin matrix and vector merge
  {
    for (int j = 0; j < AB.GetNumberColumns(); j++)
    {
      if(j != AB.GetNumberColumns() - 1)
      {
        AB.SetValue(i,j,A.GetValue(i,j));
      }
      else
      {
        AB.SetValue(i,j,B.GetValue(i,0));
      }
    }
  }  // Finish merging the square matrix and the parameter vector
  // Begin the Gauss Method, using partial pivoting
 for(int k = 0; k < AB.GetNumberRows(); k++)
 {
  for(int l = k + 1; l < AB.GetNumberRows(); l++)
  {
    if(std::abs(AB.GetValue(l,k)) > std::abs(AB.GetValue(k,k)) || AB.GetValue(k,k) == 0.0)
    {
      if(!(AB.SwapRows(k,l)))
      {
         C.SetStatus(false);
         return C;
      }
    }
  }
 if (std::abs(AB.GetValue(k,k)) > 0.0)
 {
    for(int m = k + 1; m < AB.GetNumberRows(); m++)
  {
    if(!(AB.ReducePivot(k,m)))
    {
         C.SetStatus(false);
         return C;
    }
  }
 }
 else
 {
   C.SetStatus(false);
   return C;
 }
}
  // Begin backward substitution
 for(int u = AB.GetNumberRows() - 1; u>=0; u--)
  {
  double temp = 0.0;
 for(int v = u + 1; v<AB.GetNumberRows(); v++)
   {
  temp = temp + C.GetValue(v,0) * AB.GetValue(u,v);
   }
 C.SetValue(u,0,(AB.GetValue(u,AB.GetNumberColumns() - 1) - temp)/AB.GetValue(u,u));
  }
  C.SetStatus(true);
  return C; // If successful, return the solutions
}
 else
 {
  C.SetStatus(false);
  return C;  // Or get a fatal error
 }
}

// Create an identity matrix, also known as Kronecker delta matrix
Matrix MatrixIdentity(int dimension)
{
 Matrix A = Matrix(dimension,dimension);
  for (int i=0; i<A.GetNumberRows(); i++)
 {
   for (int j=0; j<A.GetNumberColumns(); j++)
   {
    if (i == j)
    {
    A.SetValue(i,j,1);
    }
    else
    {
    A.SetValue(i,j,0);
    }
   }
 }
 A.SetStatus(true);
 return A;
}

// Multiply all matrix elements by the selected scalar.
Matrix MatrixScalarProduct(double scalar,Matrix A)
{
 Matrix B = Matrix(A.GetNumberRows(),A.GetNumberColumns());
  for (int i=0; i<A.GetNumberRows(); i++)
 {
   for (int j=0; j<A.GetNumberColumns(); j++)
   {
    B.SetValue(i,j,scalar*A.GetValue(i,j));
   }
 }
 B.SetStatus(true);
 return B;
}

// Sum of two vectores
Vector VectorSum (Vector A, Vector B)
{
Vector C = Vector(A.GetNumberElements());
 if(A.GetNumberElements() == B.GetNumberElements())
 {
   for(int i = 0; i < A.GetNumberElements(); i++)
   {
    C.SetValue(i, A.GetValue(i) + B.GetValue(i));
   }
   C.SetStatus(true); // Addition successful
   return C;
 }
  else
  {
    C.SetStatus(false); // Cannot add two different dimension vectores
    return C;
  }
}

// Subtraction of two vectores
Vector VectorMinus (Vector A, Vector B)
{
Vector C = Vector(A.GetNumberElements());
 if(A.GetNumberElements() == B.GetNumberElements())
 {
   for(int i = 0; i < A.GetNumberElements(); i++)
   {
    C.SetValue(i, A.GetValue(i) - B.GetValue(i));
   }
   C.SetStatus(true); // Subtraction successful
   return C;
 }
  else
  {
    C.SetStatus(false); // Cannot subtract two different dimension vectores
    return C;
  }
}

// Transform a vector in to a single column matrix
Matrix GetMatrix (Vector A)
{
 Matrix C = Matrix(A.GetNumberElements(),1);
 for(int i = 0; i < A.GetNumberElements(); i++)
 {
  C.SetValue(i, 0, A.GetValue(i));
 }
 C.SetStatus(true);
 return C;
}

// Transform a single column matrix to a vector
Vector GetVector (Matrix A)
{
Vector C = Vector(A.GetNumberRows());
 if(A.GetNumberColumns() == 1)
 {
   for(int i = 0; i < A.GetNumberRows(); i++)
   {
    C.SetValue(i, A.GetValue(i,0));
   }
   C.SetStatus(true); // Valid convertion
   return C;
 }
 else
 {
  C.SetStatus(false); // Invalid operation
  return C;
 }
}

// Multiply all vector elements by the selected scalar.
Vector VectorScalarProduct(double scalar,Vector A)
{
 Vector B = Vector(A.GetNumberElements());
   for (int j=0; j<A.GetNumberElements(); j++)
   {
    B.SetValue(j,scalar*A.GetValue(j));
   }
 B.SetStatus(true);
 return B;
}


// Create a null vector with a certain size
Vector VectorNull(int dimension)
{
 Vector B = Vector(dimension);
   for (int j=0; j<B.GetNumberElements(); j++)
   {
    B.SetValue(j,0.0);
   }
 B.SetStatus(true);
 return B;
}

