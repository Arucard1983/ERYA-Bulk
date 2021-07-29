/***************************************************************
 * Name:      FileLibrary.h
 * Purpose:   ERYA-PIXE file format for the Results(*.epz), Database (*.epd), Detector (*.epsd) and Ziegler(*.epsz) library
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef FILELIBRARY_H_INCLUDED
#define FILELIBRARY_H_INCLUDED
#include "ERYAPIXE.h"
#include "DatabaseLibrary.h"
#include "ParserLibrary.h"
#include "R33Library.h"
#include "XlsxLibrary.h"


class ERYAPIXEFile
{
 private:
 wxString ERYAPIXEFileName;
 public:
 ERYAPIXEFile(wxString ERYAPIXEFilePath){ ERYAPIXEFileName = ERYAPIXEFilePath;};
 bool ERYAPIXEFileLoad(wxTextCtrl* &MinimumEnergy, wxTextCtrl* &MaximumEnergy, wxTextCtrl* &EnergyStep, wxTextCtrl* &ProfilingStep, wxTextCtrl* &Charge, wxTextCtrl* &Thickness, wxArrayString& choiceElement, wxArrayString& choiceGP, wxArrayString& checkFit, wxArrayString& textZ, wxArrayString& textCP, wxArrayString& textMG, wxArrayString& textSG, wxArrayString& textYS, wxArrayString& textYI, wxArrayString& textYE, wxArrayString& textYF, wxArrayString& textSF, wxArrayString& textSM, wxArrayString& textSE, wxArrayString& TableData, ElementDatabaseArray CurrentDatabase, int& CI, int& MI, int& LT, int& LS, int& LY);
 bool ERYAPIXEFileSave(wxTextCtrl* MinimumEnergy, wxTextCtrl* MaximumEnergy, wxTextCtrl* EnergyStep, wxTextCtrl* ProfilingStep, wxTextCtrl* Charge, wxTextCtrl* Thickness, ArrayElement choiceElement, ArrayGP choiceGP, ArrayFit checkFit, ArrayZ textZ, ArrayCP textCP, ArrayMG textMG, ArraySG textSG, ArrayYS textYS, ArrayYI textYI, ArrayYE textYE, ArrayYF textYF, ArraySF textSF, ArraySM textSM, ArraySE textSE, wxGrid* TableData, int CI, int MI, int LT, int LS, int LY);
};

class DatabaseFile
{
 private:
 wxString DatabaseFileName;
 wxString DatabaseFileVersion;
 ElementDatabaseArray ParsedDatabase;
 bool EnableCompatibilityMode;
 bool ElementBlockParsing(wxArrayString ElementBlock);
 public:
 DatabaseFile(wxString DatabaseFilePath, wxString Version, ElementDatabaseArray CurrentDatabase);
 bool ERYAPIXEDatabaseFileLoad();
 bool ERYAPIXEDatabaseFileSave();
 ElementDatabaseArray GetDatabase(){return ParsedDatabase;};
 };

 class DetectorFile
{
 private:
 wxString DetectorFileName;
 wxString DetectorFileVersion;
 DetectorParameters ParsedParameters;
 bool EnableCompatibilityMode;
 public:
 DetectorFile(wxString DetectorFilePath, wxString Version, DetectorParameters CurrentParameters);
 bool DetectorFileLoad();
 bool DetectorFileSave();
 DetectorParameters GetParameters(){return ParsedParameters;};
};

class ZieglerFile
{
 private:
 wxString ZieglerFileName;
 wxString ZieglerFileVersion;
 ZieglerParameters ParsedParameters;
 ElementSRIMArray ParsedTables;
 int TableMode;
 public:
 ZieglerFile(wxString ZieglerFilePath, wxString Version, ZieglerParameters CurrentParameters, ElementSRIMArray CurrentTables, int Argument);
 bool ZieglerFileLoad();
 bool ZieglerFileSave();
 ZieglerParameters GetParameters(){return ParsedParameters;};
 ElementSRIMArray GetTables(){return ParsedTables;};
};

#endif // FILELIBRARY_H_INCLUDED
