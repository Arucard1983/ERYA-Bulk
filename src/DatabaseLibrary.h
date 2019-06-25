/***************************************************************
 * Name:      DatabaseLibrary.h
 * Purpose:   ERYA-PIXE Database classes implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef DATABASELIBRARY_H_INCLUDED
#define DATABASELIBRARY_H_INCLUDED
#include "ERYAPIXE.h"
#include <cmath>


class ElementDatabase;
class ElementSRIM;
WX_DECLARE_OBJARRAY(ElementDatabase, ElementDatabaseList);
WX_DECLARE_OBJARRAY(ElementSRIM, ElementSRIMList);

class ElementDatabase
{
   private:
   wxString dataEditElement;
   wxString dataEditGamma;
   wxString dataEditNumber;
   wxString dataEditAbundance;
   wxString dataEditAtomic;
   wxString dataEditIsotopic;
   wxArrayString dataEnergy;
   wxArrayString dataEnergyError;
   wxArrayString dataSigma;
   wxArrayString dataSigmaError;
   wxString infoElement;
   public:
   ElementDatabase() {};
   ElementDatabase(wxString Name, wxString Gamma, wxString Number, wxString Abundance, wxString Atomic, wxString Isotopic, wxArrayString Energy, wxArrayString EnergyError, wxArrayString Sigma, wxArrayString SigmaError, wxString Info);
   ElementDatabase(wxTextCtrl* EditElement, wxTextCtrl* EditGamma, wxTextCtrl* EditNumber, wxTextCtrl* EditAbundance, wxTextCtrl* EditAtomic, wxTextCtrl* EditIsotopic, wxString EditInfo, wxGrid* DataEditor);
   bool SetElement(wxString NewName){ dataEditElement = NewName; return true;};
   bool CheckElement();
   wxString GetElement(){return dataEditElement;};
   wxString GetGamma(){return dataEditGamma;};
   wxString GetNumber(){return dataEditNumber;};
   wxString GetAbundance(){return dataEditAbundance;};
   wxString GetAtomic(){return dataEditAtomic;};
   wxString GetIsotopic(){return dataEditIsotopic;};
   wxArrayString GetEnergy(){return dataEnergy;};
   wxArrayString GetEnergyError(){return dataEnergyError;};
   wxArrayString GetSigma(){return dataSigma;};
   wxArrayString GetSigmaError(){return dataSigmaError;};
   void SetInfo(wxString f){infoElement = f; return;};
   wxString GetInfo(){return infoElement;};
   bool GetAllElementInfo(wxTextCtrl* &SetElement, wxTextCtrl* &SetGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxString &SetInfo, wxGrid* &SetEnergySigmaErrorTable);
};

class ElementDatabaseArray : public ElementDatabaseList
{
  private:
  int CharToNumber(wxString c);
  double StringToNumber(wxString s);
  bool SwapStrings(wxString a, wxString b);
  wxString infoDatabase;
  public:
  ElementDatabaseArray(){};
  bool NewElement(ElementDatabase CandidateElement, bool Overwrite);
  int FindElement(wxString GetElementName, wxString GetGammaPeak);
  bool RemoveElement(wxString GetElement, wxString GetGammaPeak, wxChoice* &ListElement, wxChoice* &ListGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxGrid* &SetEnergySigma, mpWindow* &SetEnergyPlot);
  bool FindGetAllElementInfo(wxString GetElement, wxString GetGammaPeak, wxChoice* &ListElement, wxChoice* &ListGammaPeak, wxTextCtrl* &SetNumber, wxTextCtrl* &SetAbundance, wxTextCtrl* &SetMass, wxTextCtrl* &SetIsotopic, wxGrid* &SetEnergySigma, mpWindow* &SetEnergyPlot);
  bool RebuildElementGammaMenu(wxChoice* &ElementList, wxChoice* &GammaPeakList, int SelectedElementIndex);
  bool ResetElementGammaMenu(wxChoice* &ElementList, wxChoice* &GammaPeakList){ return  this->RebuildElementGammaMenu(ElementList, GammaPeakList, -1); };
  wxArrayString GetAllListElements();
  wxArrayString GetAllListGammaPeaks(wxString SearchElement);
  void SetInfo(wxString f){infoDatabase = f; return;};
  wxString GetInfo(){return infoDatabase;};
  bool SortElementDatabase();
};

// Handle the main detector model parameters
class DetectorParameters
{
  private:
   wxString DetectorFunction;
   wxArrayString DetectorEnergy;
   wxArrayString DetectorEfficiency;
   wxString infoDetector;
  public:
   DetectorParameters(){};
   DetectorParameters(wxTextCtrl* textFunctionEficiency, wxGrid* tableConstantEficiency, wxString &info);
   wxString GetDetectorFunction(){return DetectorFunction;};
   wxArrayString GetDetectorEnergy(){return DetectorEnergy;};
   wxArrayString GetDetectorEfficiency(){return DetectorEfficiency;};
   void SetDetectorFunction(wxString f){ DetectorFunction = f; return;};
   void AddEnergyEfficiencyData(wxString Energy, wxString Efficiency){ DetectorEnergy.Add(Energy); DetectorEfficiency.Add(Efficiency); return;};
   bool GetDisplay(wxTextCtrl* &SetDetectorFunction, wxGrid* &DetectorTable);
   void SetInfo(wxString f){infoDetector = f; return;};
   wxString GetInfo(){return infoDetector;};
   void Clear();
};

// Handle the SRIM Table Data
class ElementSRIM
{
 private:
 int Atomic;
 wxArrayString ElementEnergy;
 wxArrayString ElementStoppingPower;
 public:
 ElementSRIM(){};
 ElementSRIM(int Number, wxGrid* tableSRIMData);
 ElementSRIM(int Number, wxArrayString Energy, wxArrayString StoppingPower){Atomic = Number, ElementEnergy = Energy; ElementStoppingPower = StoppingPower;};
 int GetElementAtomic(){return Atomic;};
 wxArrayString GetElementEnergy(){return ElementEnergy;};
 wxArrayString GetElementStoppingPower(){return ElementStoppingPower;};
 bool GetDisplay(wxGrid* &tableSRIMData);
};

class ElementSRIMArray : public ElementSRIMList
{
 private:
 bool SortArray();
 bool IfExist(int Number);
 public:
 ElementSRIMArray(){};
 bool GetDisplay(int Number, wxGrid* &tableSRIMData);
 bool Append(int Number, wxGrid* tableSRIMData);
 bool Replace(int Number, wxGrid* tableSRIMData);
 bool Replace(int Number, wxArrayString Energy, wxArrayString StoppingPower);
 bool Delete(int Number);
 int GetElementAdress(int Number);
};


// Handle the Ziegler parameters
class ZieglerParameters
{
  private:
   wxArrayString ZieglerElements;
   wxArrayString ZieglerValues1;
   wxArrayString ZieglerValues2;
   wxArrayString ZieglerValues3;
   wxArrayString ZieglerValues4;
   wxArrayString ZieglerValues5;
   wxArrayString ZieglerValues6;
   wxArrayString ZieglerValues7;
   wxArrayString ZieglerValues8;
   wxArrayString ZieglerValues9;
   wxArrayString ZieglerValues10;
   wxArrayString ZieglerValues11;
   wxArrayString ZieglerValues12;
   wxArrayString ZieglerMass;
   wxString ZieglerVersion;
   wxString ZieglerFunction;
   wxString infoZiegler;
  public:
   ZieglerParameters(){};
   ZieglerParameters( wxChoice* choiceZieglerVersion, wxGrid* tableZieglerParameters, wxString &info);
   ZieglerParameters( wxTextCtrl* textZieglerFunction, wxChoice* choiceZieglerVersion, wxGrid* tableZieglerParameters, wxString &info);
   wxArrayString GetZieglerElements(){return ZieglerElements;};
   wxArrayString GetZieglerValues1(){return ZieglerValues1;};
   wxArrayString GetZieglerValues2(){return ZieglerValues2;};
   wxArrayString GetZieglerValues3(){return ZieglerValues3;};
   wxArrayString GetZieglerValues4(){return ZieglerValues4;};
   wxArrayString GetZieglerValues5(){return ZieglerValues5;};
   wxArrayString GetZieglerValues6(){return ZieglerValues6;};
   wxArrayString GetZieglerValues7(){return ZieglerValues7;};
   wxArrayString GetZieglerValues8(){return ZieglerValues8;};
   wxArrayString GetZieglerValues9(){return ZieglerValues9;};
   wxArrayString GetZieglerValues10(){return ZieglerValues10;};
   wxArrayString GetZieglerValues11(){return ZieglerValues11;};
   wxArrayString GetZieglerValues12(){return ZieglerValues12;};
   wxArrayString GetZieglerMass(){return ZieglerMass;};
   wxString GetZieglerVersion(){return ZieglerVersion;};
   void SetZieglerVersion(wxString v){ ZieglerVersion = v;};
   void AddZieglerParameterData(wxString e, wxString v1, wxString v2, wxString v3, wxString v4, wxString v5, wxString v6, wxString v7, wxString v8, wxString v9, wxString v10, wxString v11, wxString v12, wxString m);
   bool GetDisplay(wxChoice* &CurrentZieglerVersion, wxGrid* &ZieglerTable);
   bool GetDisplay(wxTextCtrl* &textZieglerFunction, wxChoice* &CurrentZieglerVersion, wxGrid* &ZieglerTable);
   bool FixElementNames();
   void Clear();
   void SetZieglerFunction(wxString f){ ZieglerFunction = f; return;};
   wxString GetZieglerFunction(){return ZieglerFunction;};
   void SetInfo(wxString f){infoZiegler = f; return;};
   wxString GetInfo(){return infoZiegler;};
};


#endif // DATABASELIBRARY_H_INCLUDED
