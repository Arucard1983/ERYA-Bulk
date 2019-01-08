/***************************************************************
 * Name:      PlotLibrary.h
 * Purpose:   wxMathPlot derived classes for graphical plotting
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef __PlotLibrary__
#define __PlotLibrary__

#include "ERYAPIXE.h"
#include "DatabaseLibrary.h"
#include "ParserLibrary.h"
#include "LinearAlgebra.h"

// Handle a pair of STL-vectores to make a xy-plot between Energy and Sigma
class ElementEnergySigmaData : public mpFXYVector
{
std::vector<double> EnergyData;
std::vector<double> SigmaData;
public:
ElementEnergySigmaData() {};
ElementEnergySigmaData(wxString LabelEnergySigma):mpFXYVector(LabelEnergySigma, mpALIGN_LEFT) {};
void ExportGridVector(wxGrid* EnergySigmaTable, std::vector<double>& EnergyData, std::vector<double>& SigmaData, double& GetEnergyMin, double& GetEnergyMax, double& GetSigmaMin, double& GetSigmaMax);
void ExportDetectorVector(DetectorParameters EnergySigmaTable, std::vector<double>& EnergyData, std::vector<double>& SigmaData, double& GetEnergyMin, double& GetEnergyMax, double& GetSigmaMin, double& GetSigmaMax);
std::vector<double> GetEnergyData() {return EnergyData;};
std::vector<double> GetSigmaData() {return SigmaData;};
};

// Plots polygonal lines
class PolygonalLineData: public mpFXYVector
{
std::vector<double> ElementNumber;
std::vector<double> PolygonValue;
public:
PolygonalLineData() {};
PolygonalLineData(wxString LabelEnergySigma):mpFXYVector(LabelEnergySigma, mpALIGN_LEFT) {};
void ExportArrayVector(wxArrayString TableData, std::vector<double>& ElementPosition, std::vector<double>& ElementValue, double& GetMaximumValue, double& GetMinimumValue);
void ExportArrayVectorRenorm(wxArrayString TableData, std::vector<double>& ElementPosition, std::vector<double>& ElementValue, double& GetMaximumValue, double& GetMinimumValue);
};

class PlotFunction
{
 AlgebraicFunction CurrentFunction;
 DetectorParameters CurrentDetector;
 public:
 PlotFunction(AlgebraicFunction f, DetectorParameters d){ CurrentFunction = f; CurrentDetector = d;};
 void GetFunctionGraph(mpWindow* &pf);
};


class PlotPolygonal
{
 std::vector<wxArrayString> CurrentTableData;
 wxArrayString CurrentTableLabels;
 wxString axisX, axisY;
 bool Renorm;
 public:
 PlotPolygonal(wxString Xlabel, wxString Ylabel, bool Renormalize){CurrentTableData.clear(); axisX = Xlabel; axisY = Ylabel; Renorm = Renormalize;};
 void Add(wxArrayString data, wxString label){CurrentTableData.push_back(data); CurrentTableLabels.Add(label); return;};
 void GeneratePlotPolygonal(mpWindow* &PlotPolygonal);
};

#endif // __PlotLibrary__
