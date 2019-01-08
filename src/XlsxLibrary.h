/***************************************************************
 * Name:      XlsxLibrary.h
 * Purpose:   Simple Xlsx Writer/Parser port to wxWidgets
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2017-03-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

// Port from original Simple Xlsx Writer website: https://github.com/harvesthq/simple_xlsx_writer
// Research material, which is beyond this simple library, can acess the following website: http://simplexlsx.sourceforge.net/
// A very simple Xlsx reader, can be obtained from here: https://sourceforge.net/p/xlsxparserforja/code/ci/master/tree/xlsxparser/Xlsx.java
// This module only creates new, from scratch, very basic Xlsx spreadsheet files, with only a simple sheet, filled only with strings and numbers, with a lot of hardcoded code.
// The reading part, also only gets strings and numbers.

#ifndef XLSXLIBRARY_H_INCLUDED
#define XLSXLIBRARY_H_INCLUDED
#include "ERYAPIXE.h"
#include <cmath>


class TableNode;
WX_DECLARE_OBJARRAY( TableNode, TableNodeArray);

// It's an element of a matrix block
// The type values are STRING(0), NUMBER(1), BOOLEAN(2), NONE(3)
class TableNode
{
 private:
 wxString TableElement;
 int TableType;
 public:
 TableNode(wxString InputElement);
 TableNode(wxString InputElement, int Type);
 wxString GetElement(){return TableElement;}
 void SetElement(wxString InputElement, int Type){ TableElement = InputElement; TableType = Type; return;};
 int GetType(){return TableType;};
 };

// The matrix block main class, where are stored as a vector. The user can define the expected number of columns and rows,
// where the number of columns are more important than the number of rows. Otherwise, the program
// don't decode the correct position.
class TableMatrix : public TableNodeArray
{
 private:
 int GetNodePosition(int Row, int Column);
 int ExpectedNumberRows;
 int ExpectedNumberColumns;
 public:
 TableMatrix(){ExpectedNumberRows = 0; ExpectedNumberColumns = 0;};
 TableMatrix(int Rows, int Columns){ExpectedNumberRows = Rows; ExpectedNumberColumns = Columns;};
 void SetDimension(int Rows, int Columns);
 void DeclareDimension(int Rows, int Columns){ExpectedNumberRows = Rows; ExpectedNumberColumns = Columns; return;};
 wxArrayString GetAllStrings();
 wxString GetTableMatrixValueAt(int Row, int Column, int& Type);
 bool SetTableMatrixValueAt(int Row, int Column, wxString NewData, int Type);
 void GetRealMatrixSize(int& Row, int& Column);
};

class XlsxFile
{
private:
  int InitialRow;
  int InitialColumn;
  TableMatrix InternalTable;
  wxString CreateXlsxShared();
  wxString CreateXlsxSheet();
  wxString GetXlsxIndex();
  wxString GetXlsxApp();
  wxString GetXlsxCore();
  wxString GetXlsxRels();
  wxString GetXlsxWRels();
  wxString GetXlsxStyles();
  wxString GetXlsxWBook();
  wxString XlsxFileName;
  wxString GetXlsxAdress(int Row, int Column);
  bool SetXlsxAdress(int& Row, int& Column, wxString CellAdress);
  bool GetXlsxMatrixAdress(int& InitialRowValue, int& InitialColumnValue, int& RealNumberRows, int& RealNumberColumns, wxString CellRangeAdress);
  wxString GetLetter(int Value);
  wxString GetLocalTime();
  public:
  XlsxFile(wxString LocalXlsxFileName){ XlsxFileName = LocalXlsxFileName;};
  XlsxFile(wxString LocalXlsxFileName, TableMatrix LocalSpreadsheet, wxString AtCell);
  bool LoadFile();
  bool WriteFile();
  TableMatrix GetTableMatrix(){return InternalTable;};
};

#endif // XLSXLIBRARY_H_INCLUDED
