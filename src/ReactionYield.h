/***************************************************************
 * Name:      ReactionYield.h
 * Purpose:   ERYA Reaction Yield declaration
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef REACTIONYIELD_H_INCLUDED
#define REACTIONYIELD_H_INCLUDED
#include "ERYAPIXE.h"
#include "DatabaseLibrary.h"
#include "LinearAlgebra.h"
#include "ParserLibrary.h"


class Element;
class Compound;
class FittingParameter;
class Yield;
WX_DECLARE_OBJARRAY( Element, ElementArray);
WX_DECLARE_OBJARRAY( Compound, CompoundArray);
WX_DECLARE_OBJARRAY( FittingParameter, FittingParameterArray);
WX_DECLARE_OBJARRAY( Yield, YieldArray);

// Class to handle the detector efficiency in function of their energy.
class Detector
{
private:
Vector Energy;
Vector Sigma;
AlgebraicFunction DetectorFittingFunction;
bool Sucess;
bool DefinedFunction;
public:
Detector(DetectorParameters DetectorSetup);
double GetEval(double arg);
bool CheckDetector() {return Sucess;};
AlgebraicFunction GetFittingFunction(){ return DetectorFittingFunction;};
};


// For each Element declared on main screen, it should generate from the Database classes, an Element object.
class Element
{
private:
wxString Symbol;
bool ElementFit;
double Stoichiometry;
long CompoundGroup;
int ElementID;
double GammaPeak;
long Number;
long ZieglerModel;
double Abundance;
double Atomic;
double Isotopic;
double SigmaCalibrationFactor;
Vector Energy;
Vector EnergyError;
Vector Sigma;
Vector SigmaError;
Vector ZieglerTable;
Vector SRIMEnergy;
Vector SRIMStoppingPower;
AlgebraicFunction CustomZieglerFunction;
bool Sucess;
public:
Element(ElementDatabase ThisElement, ZieglerParameters ThisZiegler, ElementSRIMArray ThisSRIM, wxString StoichiometricGuess, wxString Group, wxString YieldFit, wxString CalibrationFactor, int ElementCounter);
bool IsCorrect(){return Sucess;};
double EvaluateSigma(double AtEnergy);
double EvaluateZiegler(double AtEnergy);
double GetSigmaCalibrationFactor(){return SigmaCalibrationFactor;};
bool GetElementFit() { return ElementFit;};
wxString GetIsotopeSymbol() { return Symbol;};
double GetGammaPeak() { return GammaPeak;};
int GetAtomicNumber() { return Number;};
double GetAbundance() { return Abundance;};
double GetAtomicMass() { return Atomic;};
double GetIsotopicMass() { return Isotopic;};
double GetStoichiometry() { return Stoichiometry;};
double SetStoichiometry(double value) { Stoichiometry = value; return Stoichiometry;};
long GetCompoundGroup() { return CompoundGroup;};
int GetElementID() { return ElementID;};
double GetEnergyMinimum(){return Energy.Minimum();};
double GetEnergyMaximum(){return Energy.Maximum();};
double GetNextEnergyTable(double InitialEnergy);
};

// An auxiliary class for the array of Elements, for global handling.
class ElementExtra : public ElementArray
{
private:
double TotalStoichiometry;
double TotalMass;
int NumberParameters;
public:
ElementExtra(){};
double GetTotalStoichiometry();
double GetTotalMass();
double GetWeightStoichiometry(int i);
double EvaluateCrossSection(int ElementID, double AtEnergy, double DeltaEnergy);
double GetEnergyMinimum(int ElementID){return this->Item(ElementID).GetEnergyMinimum();};
double GetEnergyMaximum(int ElementID){return this->Item(ElementID).GetEnergyMaximum();};
double EvaluateBragg(double AtEnergy);
double GetMolarMass();
int GetNumberFittingParameters();
int GetFittingElementItem(int FindValue);
bool SetStoichiometryFromTo(IntegerVector ElementID, Vector NewValues);
Vector GetTotalFitStoichiometry();
Vector GetAllStoichiometry();
Vector GetAllMass();
bool RenormStoichiometry();
bool RenormStoichiometryTotal();
double GetMassFraction(int ElementID);
};

// And also a class for compounds, which is needed for additional constraints, like the molar mass of a certain chemical compound.

class Compound
{
private:
long CompoundGroupNumber;
bool Sucess;
ElementArray ChemicalFormula;
public:
Compound(Element ThisElement);
bool MergeCompound(Compound Additional);
int GetNumberElements() {return ChemicalFormula.GetCount();};
int GetNumberFitElements();
int GetElementIDAt(int position) {return ChemicalFormula.Item(position).GetElementID();};
bool GetElementFitAt(int position) {return ChemicalFormula.Item(position).GetElementFit();};
bool CheckCompound()  {return Sucess;};
long GetGroupNumber() {return CompoundGroupNumber;};
double GetStoichiometryAt(int position) {return ChemicalFormula.Item(position).GetStoichiometry();};
double SetStoichiometryAt(int position, double value) {return ChemicalFormula.Item(position).SetStoichiometry(value);};
};

// Handle a vector of compounds, specially if the sample had more than one compound.
class CompoundExtra : public CompoundArray
{
private:
bool Sucess;
public:
CompoundExtra(ElementExtra CurrentSelectedElements);
bool CheckCompoundArray(){return Sucess;};
bool FindElement(int ElementID, int& CompoundAdress, int& ElementAdress);
double SetStoichiometryAt(int ElementID, double value);
bool SetStoichiometryFromTo(IntegerVector ElementID, Vector NewValues);
bool RenormStoichiometry();
bool RenormStoichiometryTotal();
};

// Handle a basic Fitting Parameter
class FittingParameter
{
private:
IntegerVector GroupElementsID;
Vector GroupElementsStoichiometry;
int CompoundID;
public:
FittingParameter(IntegerVector GEID, Vector GES, int CID){ GroupElementsID = GEID; GroupElementsStoichiometry = GES; CompoundID = CID;};
bool SetStoichiometry(double newvalue);
Vector GetStoichiometry() {return GroupElementsStoichiometry;};
IntegerVector GetGroupElementsID() {return GroupElementsID;};
int GetNumberElements(){return GroupElementsStoichiometry.GetNumberElements();};
};

// Fitting Parameter array override
class FittingParameterVector : public FittingParameterArray
{
 private:
 int ElementCount();
 public:
 FittingParameterVector(ElementExtra AllSelectedElements, CompoundExtra AllSelectedCompounds);
 int GetNumberFittingParameters(){ return this->GetCount();};
 int GetNumberElements(){ return this->ElementCount();};
 Vector GetCurrentFittingParameters();
 IntegerVector GetElementParameterID();
 IntegerVector GetFittingParameterID();
 bool SetFittingStoichiometry(IntegerVector ElementsID, Vector Data);
 double SetStoichiometryAt(int ElementID, double value);
 Vector GetStoichiometryAt(int ElementID);
 Vector GetFittingStoichiometry();
 Vector GetAllFittingStoichiometry();
 IntegerVector GetElementIDAt(int ElementID);
};

// The atomic class to handle the yield calculations
class Yield
{
private:
double CurrentYield;
int YieldElementPosition;
int YieldCompoundPosition;
public:
Yield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, int ElementPosition, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds);
double FunctionYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds);
int GetElementPosition() {return YieldElementPosition;};
int GetCompoundPosition() {return YieldCompoundPosition;};
double GetYield() {return CurrentYield;};
};

// Yield Fittings require a Yield array overriding
class YieldFitting : public YieldArray
{
private:
wxString ErrorMensage;
Vector OriginalYield;
Vector ExperimentalYield;
Vector FittedYield;
Vector FittedStoichiometric;
Vector MassStoichiometric;
Vector ErrorStoichiometric;
double FitError;
int LMNumberInteractions;
int LMMaxInteractions;
double LMTau;
double LMYield;
double LMStoichiometry;
bool LevenbergMarquardtYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra& ThoseElements, CompoundExtra& ThoseCompounds);
Matrix GradientFunctionYield(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra& ThoseElements, CompoundExtra& ThoseCompounds);
public:
YieldFitting(double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Vector InitialYield, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds, FittingParameterVector ThoseFits, int mi, int lt, int ly, int ls);
double GetFit(int position) {return FittedYield.GetValue(position);};
double GetStoichiometry(int position) {return FittedStoichiometric.GetValue(position);};
double GetMass(int position) {return MassStoichiometric.GetValue(position);};
double GetError(int position) {return ErrorStoichiometric.GetValue(position);};
int GetFittingIteractions(){return LMNumberInteractions;};
double GetYieldAt(int Adress, double Emin, double Emax, double DE, double ProfilingStep, double Charge, double Thickness, Detector ThoseDetector, ElementExtra ThoseElements, CompoundExtra ThoseCompounds);
};

// Base class that handles the interface between the GUI event, and the whole numeric setup
class ReactionYield
{
 private:
 bool ErrorStatus;
 int LMN;
 Matrix ElementsProfiling;
 AlgebraicFunction OptionalDetectorFunction;
 bool EvaluateYield(ElementDatabaseArray MainDatabase, DetectorParameters DetectorSetup, ZieglerParameters ZieglerSetup, ElementSRIMArray SRIMSetup, wxString GetMinimumEnergy, wxString GetMaximumEnergy, wxString GetEnergyStep, wxString GetProfilingStep, wxString GetCharge, wxString GetThickness, wxArrayString GetCalibrationArray, wxArrayString GetGroupArray, wxArrayString GetElementArray, wxArrayString GetFitArray, wxArrayString GetMG, wxArrayString GetSG, wxArrayString& SetYS, wxArrayString GetYE, wxArrayString& SetYF, wxArrayString& SetSF, wxArrayString& SetSM, wxArrayString& SetSE, int mi, int lt, int ly, int ls);
 public:
 ReactionYield(ElementDatabaseArray MainDatabase, DetectorParameters DetectorSetup, ZieglerParameters ZieglerSetup, ElementSRIMArray SRIMSetup, wxString GetMinimumEnergy, wxString GetMaximumEnergy, wxString GetEnergyStep, wxString GetProfilingStep, wxString GetCharge, wxString GetThickness, wxArrayString GetCalibrationArray, wxArrayString GetGroupArray, wxArrayString GetElementArray, wxArrayString GetFitArray, wxArrayString GetMG, wxArrayString GetSG, wxArrayString& SetYS, wxArrayString GetYE, wxArrayString& SetYF, wxArrayString& SetSF, wxArrayString& SetSM, wxArrayString& SetSE, int mi, int lt, int ly, int ls);
 AlgebraicFunction GetDetectorFunction(){return OptionalDetectorFunction;};
 int GetNumberIteractions(){return LMN;};
 bool GetErrorStatus(){return ErrorStatus;};
 Matrix GetElementsProfiling(){return ElementsProfiling;};
 };

#endif // REACTIONYIELD_H_INCLUDED
