/***************************************************************
 * Name:      PlotLibrary.cpp
 * Purpose:   wxMathPlot derived class implementation for graphical plotting
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2016-10-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

 #include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "PlotLibrary.h"


void ElementEnergySigmaData::ExportGridVector(wxGrid* EnergySigmaTable, std::vector<double>& EnergyData, std::vector<double>& SigmaData, double& GetEnergyMin, double& GetEnergyMax, double& GetSigmaMin, double& GetSigmaMax)
{
 // Load the table to a pair of dynamic vectors
 if (EnergySigmaTable->GetNumberCols() == 2)
 {
   bool endrecord = false;
   for (int k=0; k<EnergySigmaTable->GetNumberRows(); k++)
   {
     wxString temp1 = EnergySigmaTable->GetCellValue(k,0);
     wxString temp2 = EnergySigmaTable->GetCellValue(k,1);
     if (temp1.Len() > 0 && temp2.Len() > 0 && !endrecord)
     {
       double c1,c2;
       if(temp1.ToDouble(&c1) && temp2.ToDouble(&c2))
       {
         EnergyData.push_back(c1);
         SigmaData.push_back(c2);
       }
     }
     else
     {
       endrecord = true;
     }
   }
  if (EnergyData.size() > 0 && SigmaData.size() > 0)
 {
 GetEnergyMax = EnergyData.at(0);
 GetEnergyMin = EnergyData.at(0);
 GetSigmaMax = SigmaData.at(0);
 GetSigmaMin = SigmaData.at(0);
 // Get Energy maximum and minimum
 for (int i = 0; i < EnergyData.size(); i++)
 {
  if (EnergyData.at(i) < GetEnergyMin)
   GetEnergyMin = EnergyData.at(i);
  if (EnergyData.at(i) > GetEnergyMax)
   GetEnergyMax = EnergyData.at(i);
 }
 // Get Cross-Section maximum and minimum
 for (int j = 0; j < SigmaData.size(); j++)
 {
  if (SigmaData.at(j) < GetSigmaMin)
   GetSigmaMin = SigmaData.at(j);
  if (SigmaData.at(j) > GetSigmaMax)
   GetSigmaMax = SigmaData.at(j);
 }
  return;
 }
  else
  {
   GetEnergyMax = 0;
   GetEnergyMin = 1;
   GetSigmaMax = 0;
   GetSigmaMin = 1;
    return;
  }
 }
 else
 {
  EnergyData.clear();
  SigmaData.clear();
  return;
 }
}


void ElementEnergySigmaData::ExportDetectorVector(DetectorParameters EnergySigmaTable, std::vector<double>& EnergyData, std::vector<double>& SigmaData, double& GetEnergyMin, double& GetEnergyMax, double& GetSigmaMin, double& GetSigmaMax)
{
 // Load the table to a pair of dynamic vectors
 if (EnergySigmaTable.GetDetectorEnergy().GetCount() == EnergySigmaTable.GetDetectorEfficiency().GetCount())
 {
   for (int k=0; k<EnergySigmaTable.GetDetectorEnergy().GetCount(); k++)
   {
     wxString temp1 = EnergySigmaTable.GetDetectorEnergy().Item(k);
     wxString temp2 = EnergySigmaTable.GetDetectorEfficiency().Item(k);
     if (temp1.Len() > 0 && temp2.Len() > 0)
     {
       double c1,c2;
       if(temp1.ToDouble(&c1) && temp2.ToDouble(&c2))
       {
         EnergyData.push_back(c1);
         SigmaData.push_back(c2);
       }
     }
   }
  if (EnergyData.size() > 0 && SigmaData.size() > 0)
 {
 GetEnergyMax = EnergyData.at(0);
 GetEnergyMin = EnergyData.at(0);
 GetSigmaMax = SigmaData.at(0);
 GetSigmaMin = SigmaData.at(0);
 // Get Energy maximum and minimum
 for (int i = 0; i < EnergyData.size(); i++)
 {
  if (EnergyData.at(i) < GetEnergyMin)
   GetEnergyMin = EnergyData.at(i);
  if (EnergyData.at(i) > GetEnergyMax)
   GetEnergyMax = EnergyData.at(i);
 }
 // Get Cross-Section maximum and minimum
 for (int j = 0; j < SigmaData.size(); j++)
 {
  if (SigmaData.at(j) < GetSigmaMin)
   GetSigmaMin = SigmaData.at(j);
  if (SigmaData.at(j) > GetSigmaMax)
   GetSigmaMax = SigmaData.at(j);
 }
  return;
 }
  else
  {
   GetEnergyMax = 0;
   GetEnergyMin = 1;
   GetSigmaMax = 0;
   GetSigmaMin = 1;
    return;
  }
 }
 else
 {
  EnergyData.clear();
  SigmaData.clear();
  return;
 }
}




void PolygonalLineData::ExportArrayVector(wxArrayString TableData, std::vector<double>& ElementPosition, std::vector<double>& ElementValue, double& GetMaximumValue, double& GetMinimumValue)
{
 // Load the table to a pair of dynamic vectors
 if (TableData.GetCount() > 0)
 {
   for (int k=0; k<TableData.GetCount(); k++) // Load the original data to the vectors
   {
    double data;
    if(TableData.Item(k).Len() == 0)
    {
      ElementPosition.push_back(k);
      ElementValue.push_back(0.0);
    }
    else
    {
      if(TableData.Item(k).ToDouble(&data))
      {
         ElementPosition.push_back(k);
         ElementValue.push_back(data);
      }
    }
   }
  if (ElementPosition.size() > 0 && ElementValue.size() > 0)
  {
   GetMaximumValue = ElementValue.at(0);
   GetMinimumValue = ElementValue.at(0);
   // Get Vector Maximum and Minimum
   for (int i = 0; i < ElementValue.size(); i++)
   {
    if (ElementValue.at(i) < GetMinimumValue)
    GetMinimumValue = ElementValue.at(i);
    if (ElementValue.at(i) > GetMaximumValue)
    GetMaximumValue = ElementValue.at(i);
   }
   return;
  }
  else
  {
   ElementPosition.clear();
   ElementValue.clear();
   GetMinimumValue = 0;
   GetMaximumValue = 1;
   return;
  }
 }
 else
 {
  ElementPosition.clear();
  ElementValue.clear();
  GetMinimumValue = 0;
  GetMaximumValue = 1;
  return;
 }
}




void PolygonalLineData::ExportArrayVectorRenorm(wxArrayString TableData, std::vector<double>& ElementPosition, std::vector<double>& ElementValue, double& GetMaximumValue, double& GetMinimumValue)
{
 // Load the table to a pair of dynamic vectors
 if (TableData.GetCount() > 0)
 {
   for (int k=0; k<TableData.GetCount(); k++) // Load the original data to the vectors
   {
    double data;
    if(TableData.Item(k).Len() == 0)
    {
      ElementPosition.push_back(k);
      ElementValue.push_back(0.0);
    }
    else
    {
      if(TableData.Item(k).ToDouble(&data))
      {
         ElementPosition.push_back(k);
         ElementValue.push_back(data);
      }
    }
   }
  if (ElementPosition.size() > 0 && ElementValue.size() > 0)
  {
   // Renormalize the data vector
   double ElementValueTotal = 0;
   for (int kk=0; kk < ElementValue.size(); kk++)
   {
    ElementValueTotal = ElementValueTotal + ElementValue.at(kk);
   }
   for (int kk=0; kk < ElementValue.size(); kk++)
   {
     double temp = ElementValue.at(kk);
     ElementValue.at(kk) = temp / ElementValueTotal;
   }
   // Get Vector Maximum and Minimum
   GetMaximumValue = ElementValue.at(0);
   GetMinimumValue = ElementValue.at(0);
   for (int i = 0; i < ElementValue.size(); i++)
   {
    if (ElementValue.at(i) < GetMinimumValue)
    GetMinimumValue = ElementValue.at(i);
    if (ElementValue.at(i) > GetMaximumValue)
    GetMaximumValue = ElementValue.at(i);
   }
   return;
  }
  else
  {
   ElementPosition.clear();
   ElementValue.clear();
   GetMinimumValue = 0;
   GetMaximumValue = 1;
   return;
  }
 }
 else
 {
  ElementPosition.clear();
  ElementValue.clear();
  GetMinimumValue = 0;
  GetMaximumValue = 1;
  return;
 }
}



void PlotFunction::GetFunctionGraph(mpWindow* &pf)
{
  // Prepare data
  std::vector<double> FunctionX,FX;
  std::vector<double> FunctionY,FY;
  double XMin, XMax, YMin, YMax;
  ElementEnergySigmaData *data = new ElementEnergySigmaData(wxT("Detector Interpolation Efficiency"));
  data->ExportDetectorVector(CurrentDetector,FunctionX,FunctionY,XMin,XMax,YMin,YMax);
  data->SetData(FunctionX,FunctionY);
  ElementEnergySigmaData *function = new ElementEnergySigmaData(wxT("Detector Custom Function Efficiency"));
  // Evaluate the custom function points
  for(int i=0;i<FunctionX.size();i++)
  {
    FX.push_back(FunctionX.at(i));
    double temp = CurrentFunction.GetFyxEval(FunctionX.at(i));
    FY.push_back(temp);
  }
  // Update the maximum and mininum.
  for (int i = 0; i < FY.size(); i++)
  {
   if (FY.at(i) < YMin)
    YMin = FY.at(i);
   if (FY.at(i) > YMax)
    YMax = FY.at(i);
  }
  function->SetData(FX,FY);
  // Format the axes and plot
   mpLayer *axeEnergy = new mpScaleX(wxT("Energy"),mpALIGN_BORDER_BOTTOM,false);
   mpLayer *axeEfficiency = new mpScaleY(wxT("Efficiency"),mpALIGN_BORDER_LEFT,false);
   mpLayer *plot = data;
   mpLayer *fplot = function;
   plot->SetContinuity(true);
   fplot->SetContinuity(true);
   wxPen plotcolor(*wxBLUE, 3, wxSOLID);
   wxPen fplotcolor(*wxRED, 3, wxSOLID);
   plot->SetPen(plotcolor);
   fplot->SetPen(fplotcolor);
   axeEnergy->SetDrawOutsideMargins(false);
   axeEfficiency->SetDrawOutsideMargins(false);
   plot->SetDrawOutsideMargins(false);
   fplot->SetDrawOutsideMargins(false);
    // Draw the plot elements on selected frame
   pf->DelAllLayers(false,true);
   pf->AddLayer(axeEnergy);
   pf->AddLayer(axeEfficiency);
   pf->AddLayer(plot);
   pf->AddLayer(fplot);
   mpInfoLegend* pfLegend = new mpInfoLegend(wxRect(40,20,40,20), wxTRANSPARENT_BRUSH);
   pf->AddLayer(pfLegend);
   pf->Fit(XMin,XMax,YMin,YMax*1.10);
   // Update the graphic plot
   return;
}


void PlotPolygonal::GeneratePlotPolygonal(mpWindow* &PlotPolygonal)
{
  // Clean the render, and set the axis labels
 PlotPolygonal->DelAllLayers(false,true);
 mpLayer *axeX = new mpScaleX(axisX,mpALIGN_BORDER_BOTTOM,false);
 mpLayer *axeY = new mpScaleY(axisY,mpALIGN_BORDER_LEFT,false);
 axeX->SetDrawOutsideMargins(false);
 axeY->SetDrawOutsideMargins(false);
 PlotPolygonal->AddLayer(axeX);
 PlotPolygonal->AddLayer(axeY);
 double GlobalMaximumValue, GlobalMinimumValue;
 GlobalMaximumValue = 0;
 GlobalMinimumValue = 0;
 int NumberPlotElements = 0;
 // Render the each plot to the main window
 for(int i=0; i<CurrentTableLabels.GetCount(); i++)
 {
   PolygonalLineData *data = new PolygonalLineData(CurrentTableLabels.Item(i));
   std::vector<double> ElementPosition, ElementValue;
   double MinimumValue, MaximumValue;
   if(Renorm)
     data->ExportArrayVectorRenorm(CurrentTableData.at(i), ElementPosition, ElementValue, MaximumValue, MinimumValue);
   else
     data->ExportArrayVector(CurrentTableData.at(i), ElementPosition, ElementValue, MaximumValue, MinimumValue);
   data->SetData(ElementPosition,ElementValue);
   mpLayer *plot = data;
   plot->SetContinuity(true);
   unsigned char red = 0;
   unsigned char green = 0;
   unsigned char blue = 0;
   if(i%3==0)
     red = 255;
   else if(i%3==1)
     green = 255;
   else if(i%3==2)
     blue = 255;
   else
     red = 255;
   wxPen plotcolor(wxColor(red, green, blue, wxALPHA_OPAQUE), 4, wxSOLID);
   plot->SetPen(plotcolor);
   plot->SetDrawOutsideMargins(false);
   PlotPolygonal->AddLayer(plot);
   if(GlobalMaximumValue < MaximumValue)
    GlobalMaximumValue = MaximumValue;
   if(GlobalMinimumValue > MinimumValue)
    GlobalMinimumValue = MinimumValue;
   if(NumberPlotElements < ElementPosition.size())
    NumberPlotElements = ElementPosition.size();
 }
 // Add the legend and returns
 mpInfoLegend *plotElements = new mpInfoLegend(wxRect(150,5,20,40), wxTRANSPARENT_BRUSH);
 PlotPolygonal->AddLayer(plotElements);
 PlotPolygonal->Fit(-0.001,NumberPlotElements*1.0-1+0.001,GlobalMinimumValue,GlobalMaximumValue*1.1);
 return;
}
