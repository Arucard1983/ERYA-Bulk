/***************************************************************
 * Name:      R33Library.h
 * Purpose:   IBANDL file format (*.r33), ITN text file format (*.dat|*.txt), LabView DataLog binary file (*.txt:DTLG) library
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef R33LIBRARY_H_INCLUDED
#define R33LIBRARY_H_INCLUDED
#include "ERYAPIXE.h"
#include "ParserLibrary.h"
#include "DatabaseLibrary.h"
#include <cmath>

class R33File
{
 private:
 wxString R33FileName;
 public:
 R33File(wxString IBANDLFilePath){ R33FileName = IBANDLFilePath; };
 bool IBANDLFileLoad(wxGrid* &tableDataEditor, wxTextCtrl* &textEditElement, wxTextCtrl* &textEditGamma, wxTextCtrl* &textEditNumber, wxTextCtrl* &textEditAbundance, wxTextCtrl* &textEditAtomic, wxTextCtrl* &textEditIsotopic, wxString &infoRemark);
 bool IBANDLFileSave(wxGrid *tableDataEditor, wxTextCtrl* textEditElement, wxTextCtrl* textEditGamma, wxTextCtrl* textEditNumber, wxTextCtrl* textEditAbundance, wxTextCtrl* textEditAtomic, wxTextCtrl* textEditIsotopic, wxString infoRemark);
};

class ITNFile
{
 private:
 wxString ITNFileName;
 int SelectUnits,SelectAngles;
 bool IgnoreParsingErrors;
 public:
 ITNFile(wxString ITNFilePath){ ITNFileName = ITNFilePath; SelectUnits = 0; SelectAngles = 0; IgnoreParsingErrors = false;};
 ITNFile(wxString ITNFilePath, int SigmaUnit, int SigmaAngle, bool IgnoreErrors){ ITNFileName = ITNFilePath; SelectUnits = SigmaUnit; SelectAngles = SigmaAngle; IgnoreParsingErrors = IgnoreErrors;};
 bool ITNFileLoad(wxGrid* &tableDataEditor, wxTextCtrl* &textEditElement, wxTextCtrl* &textEditGamma, wxTextCtrl* &textEditNumber, wxTextCtrl* &textEditAbundance, wxTextCtrl* &textEditAtomic, wxTextCtrl* &textEditIsotopic, wxString &infoRemark);
 bool ITNFileSave(wxGrid *tableDataEditor, wxTextCtrl* textEditElement, wxTextCtrl* textEditGamma, wxTextCtrl* textEditNumber, wxTextCtrl* textEditAbundance, wxTextCtrl* textEditAtomic, wxTextCtrl* textEditIsotopic, wxString infoRemark);
};

class SRIMFile
{
 private:
 wxString SRIMFileName;
 int AtomicElement;
 wxArrayString ElementEnergy;
 wxArrayString ElementStoppingPower;
 public:
 SRIMFile(wxString SRIMFilePath){SRIMFileName = SRIMFilePath;};
 bool IsSRIMFile();
 bool SRIMImport();
 int GetAtomic(){return AtomicElement;};
 wxArrayString GetEnergy(){return ElementEnergy;};
 wxArrayString GetStoppingPower(){return ElementStoppingPower;};
};

// Code ported from the original MATLAB avaliable from:
// https://github.com/fieldtrip/fieldtrip/blob/master/private/read_labview_dtlg.m
// The main variables from the original source, in the same order, are fid, header, version, nd, p, ld and descriptor.

class LabViewFile
{
 private:
 wxString LabViewFileName;
 wxString LabViewFileHeader;
 unsigned int LabViewFileVersion3,LabViewFileVersion2,LabViewFileVersion1,LabViewFileVersion0;
 unsigned int NumberDescriptor;
 unsigned int FileOffsetPointer;
 unsigned int FileOffsetAdress;
 wxString FileDescriptor;
 public:
 LabViewFile(){};
 unsigned int CharToInt32(unsigned char* x);
 unsigned int CharToInt16(unsigned char* x);
 double CharToDouble(unsigned char* x);
 void Initialization(wxString LabViewFilePath);
 bool IsLabViewFile();
 wxString GetLabViewFileName(){ return LabViewFileName;};
 int GetFileOffsetAdress(){ return FileOffsetAdress;}
};

class LabViewZiegler : public LabViewFile
{
  private:
  int ZieglerNumberRows;
  int ZieglerNumberColumns;
  public:
  LabViewZiegler(wxString LabViewFilePath){this->Initialization(LabViewFilePath);};
  bool IsLabViewZiegler();
  ZieglerParameters GetZieglerParametersFromLabView();
};

class LabViewElements : public LabViewFile
{
  private:
  int ElementsNumberRecords;
  public:
  LabViewElements(wxString LabViewFilePath){this->Initialization(LabViewFilePath);};
  bool IsLabViewElements();
  ElementDatabaseArray GetElementsFromLabView();
};

#endif // R33LIBRARY_H_INCLUDED
