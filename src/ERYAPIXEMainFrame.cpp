/***************************************************************
 * Name:      ERYAPIXEMainFrame.cpp
 * Purpose:   ERYA Main Program
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/


#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__


#include "ERYAPIXEMainFrame.h"
#include "ERYAPIXEdialogDatabaseManager.h"
#include "ERYAPIXEdialogDetectorSetup.h"
#include "ERYAPIXEdialogZieglerParameters.h"
#include "ERYAPIXEdialogSetup.h"
#include "ERYAPIXEdialogAdvanced.h"
#include "ERYAPIXEwizardFirstRun.h"
#include "ERYAPIXEdialogERYACalculator.h"

ERYAPIXEMainFrame::ERYAPIXEMainFrame( wxWindow* parent )
:
MainFrame( parent )
{
 if(this->StartUpProgram())
 {
  barMainStatus->SetStatusText(wxT("ERYA-Bulk is ready...") ,0);
 }
 else
 {
  barMainStatus->SetStatusText(wxT("Warning! ERYA Bulk starts in Default Mode") ,0);
 }
}


void ERYAPIXEMainFrame::OnFileNew( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to clear all current contents?"), wxT("New file?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  spinNumberElements->SetValue(1);
  this->GenerateTable(1);
  this->GenerateLog();
  if(OpenDatabase.RebuildElementGammaMenu(choiceElement.Item(0), choiceGP.Item(0), 0))
   {
    choiceElement.Item(0)->SetSelection(0);
    choiceGP.Item(0)->SetSelection(0);
   }
  choiceElement.Item(0)->SetSelection(0);
  choiceGP.Item(0)->SetSelection(0);
  checkFit.Item(0)->SetValue(false);
  textZ.Item(0)->Clear();
  textCP.Item(0)->Clear();
  textMG.Item(0)->Clear();
  textSG.Item(0)->Clear();
  textYS.Item(0)->Clear();
  textYE.Item(0)->Clear();
  textYF.Item(0)->Clear();
  textSF.Item(0)->Clear();
  textSM.Item(0)->Clear();
  textSE.Item(0)->Clear();
  renderYield->DelAllLayers(false,true);
  renderStoichiometry->DelAllLayers(false,true);
  renderError->DelAllLayers(false,true);
  renderDetectorFitting->DelAllLayers(false,true);
  SucefulFit = false;
  barMainStatus->SetStatusText(wxT("ERYA is Ready...") ,0);
  // A little hack to clean the table
  int i = choiceElement.Item(0)->GetSelection();
  int j = choiceGP.Item(0)->GetSelection();
  wxString temp = wxT("Yield ") + choiceElement.Item(0)->GetString(i) + wxT(" (") + choiceGP.Item(0)->GetString(j) + wxT(" keV)");
  tableLogProfiling->SetColLabelValue(1,temp);
 }
}

void ERYAPIXEMainFrame::OnFileOpen( wxCommandEvent& event )
{
wxFileDialog *OpenDialog = new wxFileDialog(this, wxT("Select a ERYA Yield file to open..."), wxEmptyString, wxEmptyString, wxT("ERYA Yield files (*.epz)|*.epz"), wxFD_OPEN, wxDefaultPosition);
 if (OpenDialog->ShowModal() == wxID_OK)
 {
  wxString CurrentERYAPIXEFilePath = OpenDialog->GetPath();
  ERYAPIXEFile file(CurrentERYAPIXEFilePath);
  wxArrayString ListElement,ListGammaPeak,ListFit,ListZ,ListCP,ListMG,ListSG,ListYS,ListYE,ListYF,ListSF,ListSM,ListSE,ProfilingDataTable;
  if(file.ERYAPIXEFileLoad(textMinimumEnergy, textMaximumEnergy, textStepSize, textDetectorAngle, textCharge, textThickness, ListElement, ListGammaPeak, ListFit, ListZ, ListCP, ListMG, ListSG, ListYS, ListYE, ListYF, ListSF, ListSM, ListSE, ProfilingDataTable, OpenDatabase, IterationSum, FitIterations, LogTau, LogStoichiometry, LogYield))
  {

   // Clear screen and allocate room for the loaded file

    int NumberElements = ListElement.GetCount();
    spinNumberElements->SetValue(NumberElements);
    this->GenerateTable(NumberElements);

  // Set the correct values for the pull-downs menus, and also for the check-box
  for (int k=0; k<NumberElements; k++)
  {
    int ElementPosition, GammaPosition;
    ElementPosition = choiceElement.Item(k)->FindString(ListElement.Item(k));
    choiceElement.Item(k)->SetSelection(ElementPosition);
    if(OpenDatabase.RebuildElementGammaMenu(choiceElement.Item(k), choiceGP.Item(k), ElementPosition))
    {
     choiceElement.Item(k)->SetSelection(ElementPosition);
     choiceGP.Item(k)->SetSelection(0);
     GammaPosition = choiceGP.Item(k)->FindString(ListGammaPeak.Item(k));
     choiceGP.Item(k)->SetSelection(GammaPosition);
    }

    if(ListFit.Item(k) == wxT("0"))
     checkFit.Item(k)->SetValue(false);
    else
     checkFit.Item(k)->SetValue(true);

    textZ.Item(k)->SetValue(ListZ.Item(k));
    textCP.Item(k)->SetValue(ListCP.Item(k));
    textMG.Item(k)->SetValue(ListMG.Item(k));
    textSG.Item(k)->SetValue(ListSG.Item(k));
    textYS.Item(k)->SetValue(ListYS.Item(k));
    textYE.Item(k)->SetValue(ListYE.Item(k));
    textYF.Item(k)->SetValue(ListYF.Item(k));
    textSF.Item(k)->SetValue(ListSF.Item(k));
    textSM.Item(k)->SetValue(ListSM.Item(k));
    textSE.Item(k)->SetValue(ListSE.Item(k));
  }

   //Fill the Profiling table
   this->GenerateLog();
   this->GenerateLog(ProfilingDataTable.GetCount()/(NumberElements+1));
   for(int k=0; k<ProfilingDataTable.GetCount(); k++)
     {
      int i = k / tableLogProfiling->GetNumberCols();
      int j = k % tableLogProfiling->GetNumberCols();
      tableLogProfiling->SetCellValue(i,j,ProfilingDataTable.Item(k));
     }

   // Delete previous renders
   barMainStatus->SetStatusText(wxT("File loaded successfully..."),0);
   renderDetectorFitting->DelAllLayers(false,true);
   renderError->DelAllLayers(false,true);
   renderStoichiometry->DelAllLayers(false,true);
   renderYield->DelAllLayers(false,true);

   // Make an optional recalculation
   wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to reevaluate the sample defined by this file?\nYou can press \"Run\" later if necessary..."), wxT("Reevaluate the loaded file?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
   if(dial->ShowModal() == wxID_YES)
   {
     // Store the elements as arrays
  wxArrayString CPList, MGList, SGList, YSList, YEList, YFList, SFList, SMList, SEList, ElementGroupList, ElementAdressList, ElementFitList;
  wxString MinimumEnergy, MaximumEnergy, StepEnergy, DetectorAngle, Charge, Thickness;
  MinimumEnergy = textMinimumEnergy->GetValue();
  MaximumEnergy = textMaximumEnergy->GetValue();
  StepEnergy = textStepSize->GetValue();
  DetectorAngle = textDetectorAngle->GetValue();
  Charge = textCharge->GetValue();
  Thickness = textThickness->GetValue();
  for(int k=0; k<choiceElement.GetCount(); k++)
   {
   int i = choiceElement.Item(k)->GetSelection();
   int j = choiceGP.Item(k)->GetSelection();
   int adress = OpenDatabase.FindElement( choiceElement.Item(k)->GetString(i), choiceGP.Item(k)->GetString(j));
   wxString temp = wxString::Format("%i", adress);
   ElementAdressList.Add(temp);
   bool Fitting = checkFit.Item(k)->GetValue();
   if (Fitting)
   {
    ElementFitList.Add(wxT("Yes"));
   }
   else
   {
    ElementFitList.Add(wxT("No"));
   }
   wxString GroupName = textZ.Item(k)->GetValue();
   if (GroupName.Len() == 0)
   {
    ElementGroupList.Add(wxT("0"));
   }
   else
   {
    ElementGroupList.Add(GroupName);
   }
   wxString ThisCP = textCP.Item(k)->GetValue();
   if (ThisCP.Len() == 0)
   {
    CPList.Add(wxT("1"));
   }
   else
   {
    CPList.Add(textCP.Item(k)->GetValue());
   }
   wxString ThisMG = textMG.Item(k)->GetValue();
   if (ThisMG.Len() == 0)
   {
    MGList.Add(wxT("0"));
   }
   else
   {
    MGList.Add(textMG.Item(k)->GetValue());
   }
   wxString ThisSG = textSG.Item(k)->GetValue();
   if (ThisSG.Len() == 0)
   {
    SGList.Add(wxT("1"));
   }
   else
   {
    SGList.Add(textSG.Item(k)->GetValue());
   }
   YSList.Add(textYS.Item(k)->GetValue());
   YEList.Add(textYE.Item(k)->GetValue());
   YFList.Add(textYF.Item(k)->GetValue());
   SFList.Add(textSF.Item(k)->GetValue());
   SMList.Add(textSM.Item(k)->GetValue());
   SEList.Add(textSE.Item(k)->GetValue());
   }
  // Call the main calculation routine:
  barMainStatus->SetStatusText(wxT("Evaluating, Please Wait..."),0);
  ReactionYield MainReactionYield(OpenDatabase,CurrentDetectorParameters,CurrentZieglerParameters,CurrentSRIMTables,MinimumEnergy,MaximumEnergy,StepEnergy,DetectorAngle,Charge,Thickness,CPList,ElementGroupList,ElementAdressList,ElementFitList,MGList,SGList,YSList,YEList,YFList,SFList,SMList,SEList,FitIterations,LogTau,LogYield,LogStoichiometry);
  SucefulFit = MainReactionYield.GetErrorStatus();
  IterationSum = MainReactionYield.GetNumberIteractions();
  // Display additional questions
  if(SucefulFit)
  {
   barMainStatus->SetStatusText(wxT("Fitting Requires ") + wxString::Format( "%i" , MainReactionYield.GetNumberIteractions()) + wxT(" steps..."),0);
    wxMessageDialog *dialcopy = new wxMessageDialog(NULL, wxT("Fitting was successful within ") + wxString::Format( "%i" , MainReactionYield.GetNumberIteractions()) + wxT(" steps...\n") + wxT("Do you want to update the original stoichiometry guess to the fitted stoichiometry ?") , wxT("Copy the Fitting Stoichiometry to Guess Stoichiometry?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
   if(dialcopy->ShowModal() == wxID_YES)
   {
    for(int z=0; z<YSList.GetCount(); z++) // Copy the values
    {
     textMG.Item(z)->SetValue(wxT("0"));
     textSG.Item(z)->SetValue(SFList.Item(z));
     textYS.Item(z)->SetValue(YFList.Item(z));
     textYF.Item(z)->SetValue(YFList.Item(z));
     textSF.Item(z)->SetValue(SFList.Item(z));
     textSM.Item(z)->SetValue(SMList.Item(z));
     textSE.Item(z)->SetValue(SEList.Item(z));
    }
   }
   else
   {
     // Store the results
  for(int z=0; z<YSList.GetCount(); z++)
   {
   textYS.Item(z)->SetValue(YSList.Item(z));
   textYF.Item(z)->SetValue(YFList.Item(z));
   textSF.Item(z)->SetValue(SFList.Item(z));
   textSM.Item(z)->SetValue(SMList.Item(z));
   textSE.Item(z)->SetValue(SEList.Item(z));
   }
   }
  }
  else
  {
   barMainStatus->SetStatusText(wxT("Fitting Failure!"),0);
   // Store the results
  for(int z=0; z<YSList.GetCount(); z++)
   {
   textYS.Item(z)->SetValue(YSList.Item(z));
   textYF.Item(z)->SetValue(YFList.Item(z));
   textSF.Item(z)->SetValue(SFList.Item(z));
   textSM.Item(z)->SetValue(SMList.Item(z));
   textSE.Item(z)->SetValue(SEList.Item(z));
   }
  }
  // Render the three graphic plots
  PlotPolygonal DisplayYields(wxT("Element Number"),wxT("Nuclear Yield"),false);
  DisplayYields.Add(YSList,wxT("Theoretical Nuclear Yield"));
  DisplayYields.Add(YEList,wxT("Experimental Nuclear Yield"));
  DisplayYields.Add(YFList,wxT("Fitted Nuclear Yield"));
  DisplayYields.GeneratePlotPolygonal(renderYield);
  PlotPolygonal DisplayStoichiometry(wxT("Element Number"),wxT("Stoichiometry"),true);
  DisplayStoichiometry.Add(SGList,wxT("Experimental Normalized Stoichiometry"));
  DisplayStoichiometry.Add(SFList,wxT("Fitted Normalized Stoichiometry"));
  DisplayStoichiometry.GeneratePlotPolygonal(renderStoichiometry);
  PlotPolygonal DisplayError(wxT("Element Number"),wxT("Fitting Error"),false);
  DisplayError.Add(SEList,wxT("Stoichiometry Fitting Error"));
  DisplayError.GeneratePlotPolygonal(renderError);
  // Render the Detector Efficiency plot
  PlotFunction DetectorPlot(MainReactionYield.GetDetectorFunction(),CurrentDetectorParameters);
  DetectorPlot.GetFunctionGraph(renderDetectorFitting);
   }
  }
 }
OpenDialog->Close();
}

void ERYAPIXEMainFrame::OnFileSave( wxCommandEvent& event )
{
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save ERYA Yield File As..."), wxEmptyString, wxEmptyString,wxT("ERYA files (*.epz)|*.epz"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 if (SaveDialog->ShowModal() == wxID_OK)
 {
 wxString CurrentERYAPIXEFilePath = SaveDialog->GetPath();
 ERYAPIXEFile file(CurrentERYAPIXEFilePath);
 file.ERYAPIXEFileSave(textMinimumEnergy, textMaximumEnergy, textStepSize, textDetectorAngle, textCharge, textThickness,  choiceElement, choiceGP, checkFit, textZ, textCP, textMG, textSG, textYS, textYE, textYF, textSF, textSM, textSE, tableLogProfiling, IterationSum, FitIterations, LogTau, LogStoichiometry, LogYield);
 }
SaveDialog->Close();
}

void ERYAPIXEMainFrame::OnFileSaveAs( wxCommandEvent& event )
{
wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save ERYA Yield File As..."), wxEmptyString, wxEmptyString,wxT("ERYA files (*.epz)|*.epz|Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 if (SaveDialog->ShowModal() == wxID_OK)
 {
  wxString CurrentERYAPIXEFilePath = SaveDialog->GetPath();
  wxFileName ERYAPIXEFileName(CurrentERYAPIXEFilePath);
  wxString Version = ERYAPIXEFileName.GetExt();
  if(Version == wxT("epz"))
  {
    ERYAPIXEFile file(CurrentERYAPIXEFilePath);
    file.ERYAPIXEFileSave(textMinimumEnergy, textMaximumEnergy, textStepSize, textDetectorAngle, textCharge, textThickness, choiceElement, choiceGP, checkFit, textZ, textCP, textMG, textSG, textYS, textYE, textYF, textSF, textSM, textSE, tableLogProfiling, IterationSum, FitIterations, LogTau, LogStoichiometry, LogYield);
  }
  else
  {
   // Fix the correct table dimension, by placing a correct number of blank columns, if necessary
   int AdditionalColumns = tableLogProfiling->GetNumberCols()-13;
   if(AdditionalColumns < 0)
    AdditionalColumns = 0;

   int ShiftColumns = 13 - tableLogProfiling->GetNumberCols();
   if(ShiftColumns < 0)
    ShiftColumns = 0;

    // Create a TableMatrix object to export the main spreadsheet

    TableMatrix CurrentYieldTable(choiceElement.GetCount()+1,13+AdditionalColumns);
    CurrentYieldTable.Clear();
    CurrentYieldTable.DeclareDimension(1,13+AdditionalColumns);

    // Add the first row to the labels

    CurrentYieldTable.Add(TableNode(wxT("Element")));
    CurrentYieldTable.Add(TableNode(wxT("Gamma Peak")));
    CurrentYieldTable.Add(TableNode(wxT("Fit")));
    CurrentYieldTable.Add(TableNode(wxT("Fixed Ratio Group")));
    CurrentYieldTable.Add(TableNode(wxT("Calibration Parameter")));
    CurrentYieldTable.Add(TableNode(wxT("Initial Mass")));
    CurrentYieldTable.Add(TableNode(wxT("Initial Composition")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Guess")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Experimental")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Fitted")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Atomic")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Mass")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Error")));
    for(int z=0; z<AdditionalColumns; z++)
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    // Add the contents to another rows

    for(int i=0; i<choiceElement.GetCount();i++)
    {
      wxString StoichiometryValue = textSG.Item(i)->GetValue(); //Evaluate the algebraic expression for the Stoichioometry, before pass to the Excel file
      if(StoichiometryValue.Trim().Len()==0)
          StoichiometryValue = wxT("1");
      AlgebraicFunction StoichiometryInput(StoichiometryValue);
      double StoichiometryData;
      if(StoichiometryInput.GetErrorString().Len()==0)
      {
       StoichiometryData = StoichiometryInput.GetAnsEval();
      }
      else
      {
        StoichiometryData = 1.0;
      }

      wxString MassFractionValue = textMG.Item(i)->GetValue(); //Evaluate the algebraic expression for the Mass Fraction, before pass to the Excel file
      if(MassFractionValue.Trim().Len()==0)
          MassFractionValue = wxT("0");
      AlgebraicFunction MassFractionInput(MassFractionValue);
      double MassFractionData;
      if(MassFractionInput.GetErrorString().Len()==0)
      {
       MassFractionData = MassFractionInput.GetAnsEval();
      }
      else
      {
        MassFractionData = 1.0;
      }

      int ElementSlot = choiceElement.Item(i)->GetSelection();
      int GammaPeakSlot = choiceGP.Item(i)->GetSelection();
      CurrentYieldTable.Add(TableNode(choiceElement.Item(i)->GetString(ElementSlot),0));
      CurrentYieldTable.Add(TableNode(choiceGP.Item(i)->GetString(GammaPeakSlot),1));
       if (checkFit.Item(i)->GetValue())
         CurrentYieldTable.Add(TableNode(wxT("true"),2));
       else
         CurrentYieldTable.Add(TableNode(wxT("false"),2));
      CurrentYieldTable.Add(TableNode(textZ.Item(i)->GetValue(),1));
      if(textCP.Item(i)->GetValue().Len() == 0)
       CurrentYieldTable.Add(TableNode(wxT("1"),1));
      else
       CurrentYieldTable.Add(TableNode(textCP.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(wxString::Format("%f",MassFractionData),1));
      CurrentYieldTable.Add(TableNode(wxString::Format("%f",StoichiometryData),1));
      CurrentYieldTable.Add(TableNode(textYS.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textYE.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textYF.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSF.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSM.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSE.Item(i)->GetValue(),1));

     for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }
    }

    // And the bottom line to the experimental parameters
    CurrentYieldTable.Add(TableNode(wxT("Detector:"),0));
    CurrentYieldTable.Add(TableNode(wxT("Emin(keV)="),0));
    CurrentYieldTable.Add(TableNode(textMinimumEnergy->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Emax(keV)="),0));
    CurrentYieldTable.Add(TableNode(textMaximumEnergy->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Step(keV)="),0));
    CurrentYieldTable.Add(TableNode(textStepSize->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Table Step(keV)"),0));
    CurrentYieldTable.Add(TableNode(textDetectorAngle->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Charge(uC)="),0));
    CurrentYieldTable.Add(TableNode(textCharge->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Thickness(ug/cm2)="),0));
    CurrentYieldTable.Add(TableNode(textThickness->GetValue(),1));
    for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }
    // And the fitting tuning parameters

    CurrentYieldTable.Add(TableNode(wxT("Fitting:"),0));
    CurrentYieldTable.Add(TableNode(wxT("Number Iterations="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%i",IterationSum),1));
    CurrentYieldTable.Add(TableNode(wxT("Maximum Iterations="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%i",FitIterations),1));
    CurrentYieldTable.Add(TableNode(wxT("Successful Fitting?"),0));
    if (SucefulFit)
      CurrentYieldTable.Add(TableNode(wxT("true"),2));
    else
      CurrentYieldTable.Add(TableNode(wxT("false"),2));
    CurrentYieldTable.Add(TableNode(wxT("Initial Damping="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogTau)),1));
    CurrentYieldTable.Add(TableNode(wxT("Yield Precision="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogYield)),1));
    CurrentYieldTable.Add(TableNode(wxT("Stoichiometry Precision="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogStoichiometry)),1));
    for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }

    // Copy the Profiling Data
    for(int z=0; z<12+AdditionalColumns; z++) //Empty Line
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    for (int z=0; z < tableLogProfiling->GetNumberCols(); z++) //Labels line
    {
      wxString label = tableLogProfiling->GetColLabelValue(z);
      CurrentYieldTable.Add(TableNode(label,0));
    }
    for(int z=0; z<ShiftColumns; z++)
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    for (int p = 0; p < tableLogProfiling->GetNumberRows(); p++)
    {
      for (int q = 0; q < tableLogProfiling->GetNumberCols(); q++)
      {
        wxString temp = tableLogProfiling->GetCellValue(p,q);
        CurrentYieldTable.Add(TableNode(temp,1));
      }
      for(int z=0; z<ShiftColumns; z++)
      {
       CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
      }
    }

    // Save the file as an Excel file
      XlsxFile ERYAPIXESpreadsheet(CurrentERYAPIXEFilePath,CurrentYieldTable,wxT("A1"));
      ERYAPIXESpreadsheet.WriteFile();
  }
 }
SaveDialog->Close();
}

void ERYAPIXEMainFrame::OnFileQuit( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to quit?\nAll opened data will be lost."), wxT("Quit?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
 Close();
 }
}

void ERYAPIXEMainFrame::OnDatabaseDetector( wxCommandEvent& event )
{
ERYAPIXEdialogDetectorSetup* detector = new ERYAPIXEdialogDetectorSetup(this);
detector->ShowModal();
}

void ERYAPIXEMainFrame::OnDatabaseElements( wxCommandEvent& event )
{
ERYAPIXEdialogDatabaseManager* database = new ERYAPIXEdialogDatabaseManager(this);
database->ShowModal();

// Refresh the main page
if (ResetDatabase)
{
 for(int z=0; z<choiceElement.GetCount(); z++)
 {
  choiceElement.Item(z)->Clear();
  choiceGP.Item(z)->Clear();
  checkFit.Item(z)->SetValue(false);
  textZ.Item(z)->Clear();
  textCP.Item(z)->Clear();
  textCP.Item(z)->SetValue(wxT("1"));
  textMG.Item(z)->Clear();
  textSG.Item(z)->Clear();
  textYS.Item(z)->Clear();
  textYE.Item(z)->Clear();
  textYF.Item(z)->Clear();
  textSF.Item(z)->Clear();
  textSM.Item(z)->Clear();
  textSE.Item(z)->Clear();
 }
 for(int k=0; k<choiceElement.GetCount(); k++)
 {
  if(OpenDatabase.ResetElementGammaMenu(choiceElement.Item(k), choiceGP.Item(k)))
  {
    choiceElement.Item(k)->SetSelection(0);
    choiceGP.Item(k)->SetSelection(0);
  }
 }
}
}

void ERYAPIXEMainFrame::OnDatabaseZiegler( wxCommandEvent& event )
{
ERYAPIXEdialogZieglerParameters* ziegler = new ERYAPIXEdialogZieglerParameters(this);
ziegler->ShowModal();
}

void ERYAPIXEMainFrame::OnDatabaseSetup( wxCommandEvent& event )
{
 ERYAPIXEdialogSetup* setup = new ERYAPIXEdialogSetup(this);
 setup->ShowModal();
 if(CloseProgram)
 {
  if(this->StartUpProgram())
    {
     barMainStatus->SetStatusText(wxT("ERYA-Bulk is ready...") ,0);
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The new ERYA-Bulk configuration file are in use.\nThe new default databases are also reloaded automatically."), wxT("Reset Complete!"), wxOK);
      dial->ShowModal();
    }
    else
    {
     barMainStatus->SetStatusText(wxT("Warning! ERYA Bulk starts in Default Mode") ,0);
    }
 }
}

void ERYAPIXEMainFrame::OnToolCalculator( wxCommandEvent& event )
{
 ERYAPIXEdialogERYACalculator *calculator = new ERYAPIXEdialogERYACalculator(this);
 calculator->ShowModal();
}

void ERYAPIXEMainFrame::OnToolReset( wxCommandEvent& event )
{
wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to reset the ERYA-Profiling configuration files?\nIt will start a new Setup Wizard..."), wxT("Reset ERYA-Profiling configurations?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
  if(this->DeleteSetupFile())
  {
    if(this->StartUpProgram())
    {
     barMainStatus->SetStatusText(wxT("ERYA-Bulk is ready...") ,0);
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The new ERYA-Bulk configuration file are in use.\nThe new default databases are also reloaded automatically."), wxT("Reset Complete!"), wxOK);
      dial->ShowModal();
    }
    else
    {
     barMainStatus->SetStatusText(wxT("Warning! ERYA Bulk starts in Default Mode") ,0);
    }
  }
  else
  {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot reset ERYA Bulk.\nRefer to the User Guide about\nhow to perform a manual reset."), wxT("Reset Error!"), wxOK | wxICON_ERROR);
  dial->ShowModal();
  }
 }
}

void ERYAPIXEMainFrame::OnHelpAbout( wxCommandEvent& event )
{
 wxMessageBox(wxT("Current Build Version 4.60\nBuilt with wxWidgets 3.0.4, with wxMathPlot 0.13\nand Custom Simple Xlsx File Library\n(c) 2019 LIBPHYS"), wxT("Welcome to ERYA"));
}

void ERYAPIXEMainFrame::OnElement( wxCommandEvent& event )
{
 for(int k=0; k<choiceElement.GetCount(); k++)
 {
  int i = choiceElement.Item(k)->GetSelection();
  if(OpenDatabase.RebuildElementGammaMenu(choiceElement.Item(k), choiceGP.Item(k), i))
   {
    choiceElement.Item(k)->SetSelection(i);
    choiceGP.Item(k)->SetSelection(0);
   }
 }
 this->GenerateLog();
}

void ERYAPIXEMainFrame::OnGammaPeak( wxCommandEvent& event )
{
 for(int k=0; k<choiceGP.GetCount(); k++)
 {
 int i = choiceElement.Item(k)->GetSelection();
 int j = choiceGP.Item(k)->GetSelection();
 choiceElement.Item(k)->SetSelection(i);
 choiceGP.Item(k)->SetSelection(j);
 }
 this->GenerateLog();
}


void ERYAPIXEMainFrame::OnNumberTrigger( wxSpinEvent& event )
{
 // Get the desired number of elements
 int NumberElements = spinNumberElements->GetValue();
 // Call the dynamic table procedure
 this->GenerateTable(NumberElements);
 this->GenerateLog();
}

// Needed for Win32 API to handle direct number input
void ERYAPIXEMainFrame::OnDirectNumberTrigger( wxCommandEvent& event )
{
 // Get the desired number of elements
 int NumberElements = spinNumberElements->GetValue();
 // Call the dynamic table procedure
 this->GenerateTable(NumberElements);
 this->GenerateLog();
}

void ERYAPIXEMainFrame::OnMainFit( wxCommandEvent& event )
{
if( IsDatabaseEmpty(OpenDatabase) || IsDetectorEmpty(CurrentDetectorParameters) || IsZieglerEmpty(CurrentZieglerParameters))
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Please select a database, and try again."), wxT("Error: Undefined Database"), wxOK | wxICON_ERROR);
  dial->ShowModal();
 }
 else
 {
  // Store the elements as arrays
  wxArrayString CPList, MGList, SGList, YSList, YEList, YFList, SFList, SMList, SEList, ElementGroupList, ElementAdressList, ElementFitList;
  wxString MinimumEnergy, MaximumEnergy, StepEnergy, DetectorAngle, Charge, Thickness;
  MinimumEnergy = textMinimumEnergy->GetValue();
  MaximumEnergy = textMaximumEnergy->GetValue();
  StepEnergy = textStepSize->GetValue();
  DetectorAngle = textDetectorAngle->GetValue();
  Charge = textCharge->GetValue();
  Thickness = textThickness->GetValue();
  for(int k=0; k<choiceElement.GetCount(); k++)
   {
   int i = choiceElement.Item(k)->GetSelection();
   int j = choiceGP.Item(k)->GetSelection();
   int adress = OpenDatabase.FindElement( choiceElement.Item(k)->GetString(i), choiceGP.Item(k)->GetString(j));
   wxString temp = wxString::Format("%i", adress);
   ElementAdressList.Add(temp);
   bool Fitting = checkFit.Item(k)->GetValue();
   if (Fitting)
   {
    ElementFitList.Add(wxT("Yes"));
   }
   else
   {
    ElementFitList.Add(wxT("No"));
   }
   wxString GroupName = textZ.Item(k)->GetValue();
   if (GroupName.Len() == 0)
   {
    ElementGroupList.Add(wxT("0"));
   }
   else
   {
    ElementGroupList.Add(GroupName);
   }
   wxString ThisCP = textCP.Item(k)->GetValue();
   if (ThisCP.Len() == 0)
   {
    CPList.Add(wxT("1"));
   }
   else
   {
    CPList.Add(textCP.Item(k)->GetValue());
   }
   wxString ThisMG = textMG.Item(k)->GetValue();
   if (ThisMG.Len() == 0)
   {
    MGList.Add(wxT("0"));
   }
   else
   {
    MGList.Add(textMG.Item(k)->GetValue());
   }
   wxString ThisSG = textSG.Item(k)->GetValue();
   if (ThisSG.Len() == 0)
   {
    SGList.Add(wxT("1"));
   }
   else
   {
    SGList.Add(textSG.Item(k)->GetValue());
   }
   YSList.Add(textYS.Item(k)->GetValue());
   YEList.Add(textYE.Item(k)->GetValue());
   YFList.Add(textYF.Item(k)->GetValue());
   SFList.Add(textSF.Item(k)->GetValue());
   SMList.Add(textSM.Item(k)->GetValue());
   SEList.Add(textSE.Item(k)->GetValue());
   }
  // Call the main calculation routine:
  barMainStatus->SetStatusText(wxT("Evaluating, Please Wait..."),0);
  ReactionYield MainReactionYield(OpenDatabase,CurrentDetectorParameters,CurrentZieglerParameters,CurrentSRIMTables,MinimumEnergy,MaximumEnergy,StepEnergy,DetectorAngle,Charge,Thickness,CPList,ElementGroupList,ElementAdressList,ElementFitList,MGList,SGList,YSList,YEList,YFList,SFList,SMList,SEList,FitIterations,LogTau,LogYield,LogStoichiometry);
  SucefulFit = MainReactionYield.GetErrorStatus();
  IterationSum = MainReactionYield.GetNumberIteractions();
  Matrix ProfilingData = MainReactionYield.GetElementsProfiling();
  // Display additional questions
  if(SucefulFit)
  {
   barMainStatus->SetStatusText(wxT("Fitting Requires ") + wxString::Format( "%i" , MainReactionYield.GetNumberIteractions()) + wxT(" steps..."),0);
    wxMessageDialog *dialcopy = new wxMessageDialog(NULL, wxT("Fitting was successful within ") + wxString::Format( "%i" , MainReactionYield.GetNumberIteractions()) + wxT(" steps...\n") + wxT("Do you want to update the original stoichiometry guess to the fitted stoichiometry ?") , wxT("Copy the Fitting Stoichiometry to Guess Stoichiometry?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
   if(dialcopy->ShowModal() == wxID_YES)
   {
    for(int z=0; z<YSList.GetCount(); z++) // Copy the values
    {
     textMG.Item(z)->SetValue(wxT("0")); // Erased since it was converted
     textSG.Item(z)->SetValue(SFList.Item(z));
     textYS.Item(z)->SetValue(YFList.Item(z));
     textYF.Item(z)->SetValue(YFList.Item(z));
     textSF.Item(z)->SetValue(SFList.Item(z));
     textSM.Item(z)->SetValue(SMList.Item(z));
     textSE.Item(z)->SetValue(SEList.Item(z));
    }
   }
   else
   {
     // Store the results
  for(int z=0; z<YSList.GetCount(); z++)
   {
   textYS.Item(z)->SetValue(YSList.Item(z));
   textYF.Item(z)->SetValue(YFList.Item(z));
   textSF.Item(z)->SetValue(SFList.Item(z));
   textSM.Item(z)->SetValue(SMList.Item(z));
   textSE.Item(z)->SetValue(SEList.Item(z));
   }
   }
  }
  else
  {
   barMainStatus->SetStatusText(wxT("Fitting Failure!"),0);
   // Store the results
  for(int z=0; z<YSList.GetCount(); z++)
   {
   textYS.Item(z)->SetValue(YSList.Item(z));
   textYF.Item(z)->SetValue(YFList.Item(z));
   textSF.Item(z)->SetValue(SFList.Item(z));
   textSM.Item(z)->SetValue(SMList.Item(z));
   textSE.Item(z)->SetValue(SEList.Item(z));
   }
  }
  // Render the three graphic plots
  PlotPolygonal DisplayYields(wxT("Element Number"),wxT("Nuclear Yield"),false);
  DisplayYields.Add(YSList,wxT("Theoretical Nuclear Yield"));
  DisplayYields.Add(YEList,wxT("Experimental Nuclear Yield"));
  DisplayYields.Add(YFList,wxT("Fitted Nuclear Yield"));
  DisplayYields.GeneratePlotPolygonal(renderYield);
  PlotPolygonal DisplayStoichiometry(wxT("Element Number"),wxT("Composition"),true);
  DisplayStoichiometry.Add(SGList,wxT("Experimental Normalized Composition"));
  DisplayStoichiometry.Add(SFList,wxT("Fitted Normalized Composition"));
  DisplayStoichiometry.GeneratePlotPolygonal(renderStoichiometry);
  PlotPolygonal DisplayError(wxT("Element Number"),wxT("Fitting Error"),false);
  DisplayError.Add(SEList,wxT("Stoichiometry Fitting Error"));
  DisplayError.GeneratePlotPolygonal(renderError);
  // Render the Detector Efficiency plot
  PlotFunction DetectorPlot(MainReactionYield.GetDetectorFunction(),CurrentDetectorParameters);
  DetectorPlot.GetFunctionGraph(renderDetectorFitting);
  // Convert profiling Data
  if (ProfilingData.GetNumberColumns() > 0 && ProfilingData.GetNumberRows() > 0)
  {
    // Set the Correct dimensions
    this->GenerateLog(ProfilingData.GetNumberRows());
    // Write the results
    for (int p = 0; p < ProfilingData.GetNumberRows(); p++)
    {
      for (int q = 0; q < ProfilingData.GetNumberColumns(); q++)
      {
        tableLogProfiling->SetCellValue(p,q,wxString::Format("%f",ProfilingData.GetValue(p,q)));
      }
    }
  }
 }
}

void ERYAPIXEMainFrame::OnMainAdvanced( wxCommandEvent& event )
{
 ERYAPIXEdialogAdvanced* fit = new ERYAPIXEdialogAdvanced(this);
 fit->ShowModal();
}


void ERYAPIXEMainFrame::OnMainHelp( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to quit?\nAll opened data will be lost."), wxT("Quit?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
 Close();
 }
}

void ERYAPIXEMainFrame::OnMainNew( wxCommandEvent& event )
{
 wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Do you want to clear all current contents?"), wxT("Clear the Table?"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
 if (dial->ShowModal() == wxID_YES)
 {
 for(int k=0; k<choiceElement.GetCount(); k++)
 {
  if(OpenDatabase.RebuildElementGammaMenu(choiceElement.Item(k), choiceGP.Item(k), 0))
   {
    choiceElement.Item(k)->SetSelection(0);
    choiceGP.Item(k)->SetSelection(0);
   }
  checkFit.Item(k)->SetValue(false);
  textZ.Item(k)->Clear();
  textCP.Item(k)->Clear();
  textCP.Item(k)->SetValue(wxT("1"));
  textMG.Item(k)->Clear();
  textSG.Item(k)->Clear();
  textYS.Item(k)->Clear();
  textYE.Item(k)->Clear();
  textYF.Item(k)->Clear();
  textSF.Item(k)->Clear();
  textSM.Item(k)->Clear();
  textSE.Item(k)->Clear();
  textMinimumEnergy->Clear();
  textMaximumEnergy->Clear();
  textStepSize->Clear();
  textCharge->Clear();
  textThickness->Clear();
  textMinimumEnergy->SetValue(wxT("0"));
  textStepSize->SetValue(wxT("1"));
  textDetectorAngle->SetValue(wxT("0"));
  textCharge->SetValue(wxT("1"));
  textThickness->SetValue(wxT("0"));
 }
  renderYield->DelAllLayers(false,true);
  renderStoichiometry->DelAllLayers(false,true);
  renderError->DelAllLayers(false,true);
  renderDetectorFitting->DelAllLayers(false,true);
  this->GenerateLog();
  barMainStatus->SetStatusText(wxT("ERYA is Ready...") ,0);
 }
}


void ERYAPIXEMainFrame::OnMainCheck( wxCommandEvent& event )
{
 for(int k=0; k<textSG.GetCount(); k++)
 {
    wxString temp1 = textMG.Item(k)->GetValue();
    wxString temp2 = textSG.Item(k)->GetValue();
    if(temp1.Len()==0) // Empty values for mass are equal to zero, by default.
        temp1 = wxT("0");
    if(temp2.Len()==0) // Empty values for atomic are equal to one, by default.
        temp2 = wxT("1");

    AlgebraicFunction test1(temp1);
    if(test1.GetErrorString().Trim().Len()==0)
    {
      double value = test1.GetAnsEval();
      textMG.Item(k)->SetValue(wxString::Format("%f",value));
    }
    else
    {
      wxMessageDialog *dial = new wxMessageDialog(NULL, test1.GetErrorString() , wxT("Error at Mass Composition Guess Entry #") + wxString::Format("%i",k+1), wxOK | wxICON_ERROR);
      dial->ShowModal();
      barMainStatus->SetStatusText(wxT("Warning! Invalid input entry, please check the values.") ,0);
    }

    AlgebraicFunction test2(temp2);
    if(test2.GetErrorString().Trim().Len()==0)
    {
      double value = test2.GetAnsEval();
      textSG.Item(k)->SetValue(wxString::Format("%f",value));
    }
    else
    {
      wxMessageDialog *dial = new wxMessageDialog(NULL, test2.GetErrorString() , wxT("Error at Mass Composition Guess Entry #") + wxString::Format("%i",k+1), wxOK | wxICON_ERROR);
      dial->ShowModal();
      barMainStatus->SetStatusText(wxT("Warning! Invalid input entry, please check the values.") ,0);
    }
 }
}

// Export the results to a file, which is an alternative to the menu option
void ERYAPIXEMainFrame::OnMainStop( wxCommandEvent& event )
{
   // Fix the correct table dimension, by placing a correct number of blank columns, if necessary
   int AdditionalColumns = tableLogProfiling->GetNumberCols()-13;
   if(AdditionalColumns < 0)
    AdditionalColumns = 0;

   int ShiftColumns = 13 - tableLogProfiling->GetNumberCols();
   if(ShiftColumns < 0)
    ShiftColumns = 0;

    // Create a TableMatrix object to export the main spreadsheet

    TableMatrix CurrentYieldTable(choiceElement.GetCount()+1,13+AdditionalColumns);
    CurrentYieldTable.Clear();
    CurrentYieldTable.DeclareDimension(1,13+AdditionalColumns);

    // Add the first row to the labels

    CurrentYieldTable.Add(TableNode(wxT("Element")));
    CurrentYieldTable.Add(TableNode(wxT("Gamma Peak")));
    CurrentYieldTable.Add(TableNode(wxT("Fit")));
    CurrentYieldTable.Add(TableNode(wxT("Fixed Ratio Group")));
    CurrentYieldTable.Add(TableNode(wxT("Calibration Parameter")));
    CurrentYieldTable.Add(TableNode(wxT("Initial Mass")));
    CurrentYieldTable.Add(TableNode(wxT("Initial Composition")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Guess")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Experimental")));
    CurrentYieldTable.Add(TableNode(wxT("Yield Fitted")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Atomic")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Mass")));
    CurrentYieldTable.Add(TableNode(wxT("Composition Error")));
    for(int z=0; z<AdditionalColumns; z++)
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    // Add the contents to another rows

    for(int i=0; i<choiceElement.GetCount();i++)
    {
      wxString StoichiometryValue = textSG.Item(i)->GetValue(); //Evaluate the algebraic expression for the Stoichioometry, before pass to the Excel file
      if(StoichiometryValue.Trim().Len()==0)
          StoichiometryValue = wxT("1");
      AlgebraicFunction StoichiometryInput(StoichiometryValue);
      double StoichiometryData;
      if(StoichiometryInput.GetErrorString().Len()==0)
      {
       StoichiometryData = StoichiometryInput.GetAnsEval();
      }
      else
      {
        StoichiometryData = 1.0;
      }

      wxString MassFractionValue = textMG.Item(i)->GetValue(); //Evaluate the algebraic expression for the Mass Fraction, before pass to the Excel file
      if(MassFractionValue.Trim().Len()==0)
          MassFractionValue = wxT("0");
      AlgebraicFunction MassFractionInput(MassFractionValue);
      double MassFractionData;
      if(MassFractionInput.GetErrorString().Len()==0)
      {
       MassFractionData = MassFractionInput.GetAnsEval();
      }
      else
      {
        MassFractionData = 1.0;
      }

      int ElementSlot = choiceElement.Item(i)->GetSelection();
      int GammaPeakSlot = choiceGP.Item(i)->GetSelection();
      CurrentYieldTable.Add(TableNode(choiceElement.Item(i)->GetString(ElementSlot),0));
      CurrentYieldTable.Add(TableNode(choiceGP.Item(i)->GetString(GammaPeakSlot),1));
       if (checkFit.Item(i)->GetValue())
         CurrentYieldTable.Add(TableNode(wxT("true"),2));
       else
         CurrentYieldTable.Add(TableNode(wxT("false"),2));
      CurrentYieldTable.Add(TableNode(textZ.Item(i)->GetValue(),1));
      if(textCP.Item(i)->GetValue().Len() == 0)
       CurrentYieldTable.Add(TableNode(wxT("1"),1));
      else
       CurrentYieldTable.Add(TableNode(textCP.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(wxString::Format("%f",MassFractionData),1));
      CurrentYieldTable.Add(TableNode(wxString::Format("%f",StoichiometryData),1));
      CurrentYieldTable.Add(TableNode(textYS.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textYE.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textYF.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSF.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSM.Item(i)->GetValue(),1));
      CurrentYieldTable.Add(TableNode(textSE.Item(i)->GetValue(),1));

     for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }
    }

    // And the bottom line to the experimental parameters
    CurrentYieldTable.Add(TableNode(wxT("Detector:"),0));
    CurrentYieldTable.Add(TableNode(wxT("Emin(keV)="),0));
    CurrentYieldTable.Add(TableNode(textMinimumEnergy->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Emax(keV)="),0));
    CurrentYieldTable.Add(TableNode(textMaximumEnergy->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Step(keV)="),0));
    CurrentYieldTable.Add(TableNode(textStepSize->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Table Step(keV)"),0));
    CurrentYieldTable.Add(TableNode(textDetectorAngle->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Charge(uC)="),0));
    CurrentYieldTable.Add(TableNode(textCharge->GetValue(),1));
    CurrentYieldTable.Add(TableNode(wxT("Thickness(ug/cm2)="),0));
    CurrentYieldTable.Add(TableNode(textThickness->GetValue(),1));
    for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }
    // And the fitting tuning parameters

    CurrentYieldTable.Add(TableNode(wxT("Fitting:"),0));
    CurrentYieldTable.Add(TableNode(wxT("Number Iterations="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%i",IterationSum),1));
    CurrentYieldTable.Add(TableNode(wxT("Maximum Iterations="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%i",FitIterations),1));
    CurrentYieldTable.Add(TableNode(wxT("Successful Fitting?"),0));
    if (SucefulFit)
      CurrentYieldTable.Add(TableNode(wxT("true"),2));
    else
      CurrentYieldTable.Add(TableNode(wxT("false"),2));
    CurrentYieldTable.Add(TableNode(wxT("Initial Damping="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogTau)),1));
    CurrentYieldTable.Add(TableNode(wxT("Yield Precision="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogYield)),1));
    CurrentYieldTable.Add(TableNode(wxT("Stoichiometry Precision="),0));
    CurrentYieldTable.Add(TableNode(wxString::Format("%f",std::pow(10,-1.0*LogStoichiometry)),1));
    for(int z=0; z<AdditionalColumns; z++)
     {
      CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
     }

    // Copy the Profiling Data
    for(int z=0; z<12+AdditionalColumns; z++) //Empty Line
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    for (int z=0; z < tableLogProfiling->GetNumberCols(); z++) //Labels line
    {
      wxString label = tableLogProfiling->GetColLabelValue(z);
      CurrentYieldTable.Add(TableNode(label,0));
    }
    for(int z=0; z<ShiftColumns; z++)
    {
     CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
    }

    for (int p = 0; p < tableLogProfiling->GetNumberRows(); p++)
    {
      for (int q = 0; q < tableLogProfiling->GetNumberCols(); q++)
      {
        wxString temp = tableLogProfiling->GetCellValue(p,q);
        CurrentYieldTable.Add(TableNode(temp,1));
      }
      for(int z=0; z<ShiftColumns; z++)
      {
       CurrentYieldTable.Add(TableNode(wxEmptyString)); //Add a empty cell
      }
    }


    // Save the file as an Excel file

    wxFileDialog *SaveDialog = new wxFileDialog(this, wxT("Save the Yield Spreadsheet as..."), wxEmptyString, wxEmptyString,wxT("Microsoft Excel Xlsx File(*.xlsx)|*.xlsx"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
    if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
    {
      wxString XlsxFilePath = SaveDialog->GetPath();
      XlsxFile ERYAPIXESpreadsheet(XlsxFilePath,CurrentYieldTable,wxT("A1"));
      ERYAPIXESpreadsheet.WriteFile();
    }
    SaveDialog->Close();
}

void ERYAPIXEMainFrame::GenerateLog(int Number)
{
 int CurrentTableSize = tableLogProfiling->GetNumberRows();
   int NewTableSize = Number;
   if (CurrentTableSize != NewTableSize)
   {
    if (CurrentTableSize < NewTableSize)
    {
     tableLogProfiling->AppendRows(NewTableSize-CurrentTableSize,true);
    }
    else
    {
     tableLogProfiling->DeleteRows(NewTableSize,CurrentTableSize-NewTableSize,true);
    }
   }
}

void ERYAPIXEMainFrame::GenerateLog()
{
 tableLogProfiling->ClearGrid();
 int InitialTableSize = tableLogProfiling->GetNumberCols();
 if(InitialTableSize > 1)
 {
   tableLogProfiling->DeleteCols(1,InitialTableSize-1,true);
 }
 int InitialDataSize = tableLogProfiling->GetNumberRows();
 if(InitialDataSize > 1)
 {
   tableLogProfiling->DeleteRows(1,InitialDataSize-1,true);
 }
 // Create a new label list
 wxArrayString TableLabels;
 TableLabels.Add(wxT("Energy (keV)"));
 // Fill the columns labels
 for (int k=0; k<choiceElement.GetCount(); k++)
 {
  int i = choiceElement.Item(k)->GetSelection();
  int j = choiceGP.Item(k)->GetSelection();
  wxString temp = wxT("Yield ") + choiceElement.Item(k)->GetString(i) + wxT(" (") + choiceGP.Item(k)->GetString(j) + wxT(" keV)");
  TableLabels.Add(temp);
 }
 // Generate the new table
 tableLogProfiling->AppendCols(TableLabels.GetCount()-1,true);
 for (int z=0; z<tableLogProfiling->GetNumberCols(); z++)
 {
   tableLogProfiling->SetColLabelValue(z,TableLabels.Item(z));
   tableLogProfiling->SetColSize( z, 350 );
 }
}

void ERYAPIXEMainFrame::GenerateTable(int Number )
{

 // Get the desired number of elements
 int NumberElements = spinNumberElements->GetValue();
 // Get the current number of elements
 int CurrentElements = choiceElement.GetCount();
 // Get the Number of Operations
 int ChangeElements = NumberElements - CurrentElements;

 // If the number of elements will increase
 if (ChangeElements > 0)
{
 // Get the pull-downs menus
    choiceElementChoices = OpenDatabase.GetAllListElements();
    choiceGPChoices = OpenDatabase.GetAllListGammaPeaks(choiceElementChoices.Item(0));

 for (int k=CurrentElements; k<NumberElements; k++)
 {
  choiceElement.Add( new wxChoice( scrollButtons, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceElementChoices, 0 ));
  choiceElement.Last()->SetSelection( 0 );
  sizerButtons->Add( choiceElement.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  choiceGP.Add(new wxChoice( scrollButtons, wxID_ANY, wxDefaultPosition, wxSize(120,-1), choiceGPChoices, 0 ));
  choiceGP.Last()->SetSelection( 0 );
  sizerButtons->Add( choiceGP.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  checkFit.Add(new wxCheckBox( scrollButtons, wxID_ANY, wxT("Fit ?"), wxDefaultPosition, wxDefaultSize, 0 ));
  sizerButtons->Add( checkFit.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textZ.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
  sizerButtons->Add( textZ.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textCP.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(120,-1), 0 ));
  sizerButtons->Add( textCP.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textMG.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
  sizerButtons->Add( textMG.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textSG.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
  sizerButtons->Add( textSG.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textYS.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
  sizerButtons->Add( textYS.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textYE.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), 0 ));
  sizerButtons->Add( textYE.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textYF.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
  sizerButtons->Add( textYF.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textSF.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
  sizerButtons->Add( textSF.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textSM.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
  sizerButtons->Add( textSM.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  textSE.Add(new wxTextCtrl( scrollButtons, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120,-1), wxTE_READONLY ));
  sizerButtons->Add( textSE.Last(), 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

  choiceElement.Item(k)->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ERYAPIXEMainFrame::OnElement ), NULL, this );
  choiceGP.Item(k)->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ERYAPIXEMainFrame::OnGammaPeak ), NULL, this );

 }
  // Redraw the new elements on screen
    scrollButtons->SetSizer( sizerButtons );
	scrollButtons->Layout();
    sizerButtons->Fit( scrollButtons );

	tabElements->SetSizer( sizerElements );
	tabElements->Layout();
	sizerElements->Fit( tabElements );
   // Win32 API require external sizer refitting to correctly redraw the Elements tab, unlike the GTK which is irrelevant.
    this->SetSizer( sizerMainFrame );
	this->Layout();
	this->GenerateLog();

}
// If the number of elements will increase
if (ChangeElements < 0)
{
 int NumberRemovedElements = std::abs(ChangeElements * 13); //Obtain the number of retired objects
 for (int i=0; i<NumberRemovedElements; i++)
 {
   int CurrentNumberElements = sizerButtons->GetItemCount();
   sizerButtons->Hide(CurrentNumberElements-1); //Hide last object
   sizerButtons->Remove(CurrentNumberElements-1); //Remove the last object
 }
 for(int k=0; k<std::abs(ChangeElements); k++) //Trim internal elements
 {
   choiceElement.RemoveAt(choiceElement.GetCount()-1);
   choiceGP.RemoveAt(choiceGP.GetCount()-1);
   checkFit.RemoveAt(checkFit.GetCount()-1);
   textCP.RemoveAt(textCP.GetCount()-1);
   textSE.RemoveAt(textSE.GetCount()-1);
   textSF.RemoveAt(textSF.GetCount()-1);
   textSG.RemoveAt(textSG.GetCount()-1);
   textMG.RemoveAt(textMG.GetCount()-1);
   textSM.RemoveAt(textSM.GetCount()-1);
   textYE.RemoveAt(textYE.GetCount()-1);
   textYF.RemoveAt(textYF.GetCount()-1);
   textYS.RemoveAt(textYS.GetCount()-1);
   textZ.RemoveAt(textZ.GetCount()-1);
 }
 // Redraw the new elements on screen
    scrollButtons->SetSizer( sizerButtons );
	scrollButtons->Layout();
    sizerButtons->Fit( scrollButtons );

	tabElements->SetSizer( sizerElements );
	tabElements->Layout();
	sizerElements->Fit( tabElements );
   // Win32 API require external sizer refitting to correctly redraw the Elements tab, unlike the GTK which is irrelevant.
    this->SetSizer( sizerMainFrame );
	this->Layout();
	this->GenerateLog();
}
 // No changes on number of elements, implies to modifications at all.
}

void ERYAPIXEMainFrame::GenerateTable()
{
 this->GenerateTable(1);
}

bool ERYAPIXEMainFrame::StartUpProgram()
{
 // Define some internal variables
 FitIterations = 100;
 LogTau = 3;
 LogYield = 3;
 LogStoichiometry =3;
 // Load the configuration file, or force the user to create a new configuration file
 if(!(this->LoadSetupFile(CurrentConfig,CurrentDetectorFile,CurrentDatabaseFile,CurrentZieglerFile)))
  {
   ERYAPIXEwizardFirstRun *firstrunwizard = new ERYAPIXEwizardFirstRun(this);
   firstrunwizard->StartWizard(); //Start the wizard at the first page...
   firstrunwizard->Destroy();
   if(AcceptSync) //The wizard was completed
   {
    wxBusyInfo* CreatingFiles = new wxBusyInfo(wxT("ERYA-Bulk are checking the files for consistency\nPlease wait, since on slow machines it can take\nseveral seconds to finish."));
    if(!(this->SaveSetupFile(CurrentConfig,CurrentDetectorFile,CurrentDatabaseFile,CurrentZieglerFile,CurrentMode)))
    {
     if(CreatingFiles)
      delete CreatingFiles;
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("Cannot create the configuration file, due to an error ...\nTry other settings, and also check if the target directory is not protected."), wxT("Error: Setup Failure!"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
    else //Copy and convert the selected databases to the target directory
    {
     if(!(this->StartUpDatabases(CurrentConfig,CurrentDetectorFile,CurrentDatabaseFile,CurrentZieglerFile)))
     {
      if(CreatingFiles)
       delete CreatingFiles;
      wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The initial selected databases are not compatible with ERYA Bulk..."), wxT("Error: Setup Failure!"), wxOK | wxICON_ERROR);
      dial->ShowModal();
      return false;
     }
    }
    if(CreatingFiles)
      delete CreatingFiles;
   }
  }
 this->GenerateTable();
 this->GenerateLog();
 if(CurrentDatabaseFile.Len()>0 && CurrentDetectorFile.Len()>0 && CurrentZieglerFile.Len()>0)
 {
 // Load the default database file
 wxFileName df(CurrentDatabaseFile);
 wxString DatabaseVersion = wxT("epd");
 DatabaseFile loadDatabase(CurrentConfig + wxFileName::GetPathSeparator() + CurrentDatabaseFile,DatabaseVersion,OpenDatabase);
   if(loadDatabase.ERYAPIXEDatabaseFileLoad())
    {
     OpenDatabase = loadDatabase.GetDatabase();
     // Rebuild the two pull down menus for all elements, while use the two STL integer vectors to handle their adress
     for(int k=0; k<choiceElement.GetCount(); k++)
     {
      OpenDatabase.ResetElementGammaMenu(choiceElement.Item(k),choiceGP.Item(k));
     }
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The default database is not compatible with ERYA Bulk."), wxT("Error: Invalid Database"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }

 // Load the default detector setup file
 wxFileName ef(CurrentDetectorFile);
 wxString DetectorVersion = wxT("epsd");
 DetectorFile loadDetector(CurrentConfig + wxFileName::GetPathSeparator() + CurrentDetectorFile,DetectorVersion,CurrentDetectorParameters);
   if(loadDetector.DetectorFileLoad())
    {
     CurrentDetectorParameters = loadDetector.GetParameters();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The default detector is not compatible with ERYA Bulk."), wxT("Error: Invalid Database"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }

 // Load the default Ziegler parameters file
 wxFileName zf(CurrentZieglerFile);
 wxString ZieglerVersion = wxT("epsz");
 ZieglerFile loadZiegler(CurrentConfig + wxFileName::GetPathSeparator() + CurrentZieglerFile,ZieglerVersion,CurrentZieglerParameters,CurrentSRIMTables,0);
   if(loadZiegler.ZieglerFileLoad())
    {
     CurrentZieglerParameters = loadZiegler.GetParameters();
     CurrentSRIMTables = loadZiegler.GetTables();
    }
    else
    {
     wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("The default Ziegler is not compatible with ERYA Bulk."), wxT("Error: Invalid Database"), wxOK | wxICON_ERROR);
     dial->ShowModal();
     return false;
    }
 }
 else
 {
  wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA Bulk will start without any default databases."), wxT("Error: Incorrect Configuration"), wxOK | wxICON_ERROR);
  dial->ShowModal();
  return false;
 }
 // Some Default Values
 for(int k=0; k<choiceElement.GetCount(); k++)
 {
  if(OpenDatabase.ResetElementGammaMenu(choiceElement.Item(k), choiceGP.Item(k)))
   {
    choiceElement.Item(k)->SetSelection(0);
    choiceGP.Item(k)->SetSelection(0);
   }
  checkFit.Item(k)->SetValue(false);
  textZ.Item(k)->Clear();
  textCP.Item(k)->Clear();
  textCP.Item(k)->SetValue(wxT("1"));
  textMG.Item(k)->Clear();
  textSG.Item(k)->Clear();
  textYS.Item(k)->Clear();
  textYE.Item(k)->Clear();
  textYF.Item(k)->Clear();
  textSF.Item(k)->Clear();
  textSM.Item(k)->Clear();
  textSE.Item(k)->Clear();
  textMinimumEnergy->Clear();
  textMaximumEnergy->Clear();
  textStepSize->Clear();
  textCharge->Clear();
  textThickness->Clear();
  textMinimumEnergy->SetValue(wxT("0"));
  textStepSize->SetValue(wxT("1"));
  textDetectorAngle->SetValue(wxT("0"));
  textCharge->SetValue(wxT("1"));
  textThickness->SetValue(wxT("0"));
 }
  renderYield->DelAllLayers(false,true);
  renderStoichiometry->DelAllLayers(false,true);
  renderError->DelAllLayers(false,true);
  renderDetectorFitting->DelAllLayers(false,true);
  this->GenerateTable();
 this->GenerateLog();
 return true; //Init successful
}

bool ERYAPIXEMainFrame::StartUpDatabases()
{
   return this->StartUpDatabases(CurrentConfig,CurrentDetectorFile,CurrentDetectorFile,CurrentZieglerFile);
}

bool ERYAPIXEMainFrame::StartUpDatabases(wxString MainDirectory, wxString MainDetector, wxString MainDatabase, wxString MainZiegler)
{
    // Global settings
    wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
    wxFileName mainconfig(fileconfig);
    wxString rootconfig = mainconfig.GetPath();
    // Select the default detector settings
    wxString DetectorFileName = MainDirectory + wxFileName::GetPathSeparator() + MainDetector;
    wxString OriginalDetector = rootconfig + wxFileName::GetPathSeparator() + MainDetector;
    // Load, test and store the selected Detector Setup
    wxFileName ief(DetectorFileName);
    wxString DetectorVersion = ief.GetExt();
    DetectorFile testDetector(OriginalDetector,DetectorVersion,CurrentDetectorParameters);
   if(testDetector.DetectorFileLoad())
    {
     CurrentDetectorParameters = testDetector.GetParameters();
     DetectorVersion = wxT("epsd");
     DetectorFile targetDetector(DetectorFileName,DetectorVersion,CurrentDetectorParameters);
     if(targetDetector.DetectorFileSave())
     {
      CurrentDetectorParameters.Clear();
      CurrentDetectorParameters = targetDetector.GetParameters();
     }
     else
     {
      return false;
     }
    }
    else
    {
     return false;
    }

    // Select the default Ziegler settings
    wxString ZieglerFileName = MainDirectory + wxFileName::GetPathSeparator() + MainZiegler;
    wxString OriginalZiegler = rootconfig + wxFileName::GetPathSeparator() + MainZiegler;

    // Load, test and store the default Ziegler Parameters
    wxFileName izf(ZieglerFileName);
    wxString ZieglerVersion = izf.GetExt();
    ZieglerFile testZiegler(OriginalZiegler,ZieglerVersion,CurrentZieglerParameters,CurrentSRIMTables,0);
   if(testZiegler.ZieglerFileLoad())
    {
     CurrentZieglerParameters = testZiegler.GetParameters();
     CurrentSRIMTables = testZiegler.GetTables();
     if(CurrentSRIMTables.GetCount()>0) //Force the SRIM version if exists SRIM tables
     {
       CurrentZieglerParameters.SetZieglerVersion(wxT("3"));
     }
     ZieglerVersion = wxT("epsz");
     ZieglerFile targetZiegler(ZieglerFileName,ZieglerVersion,CurrentZieglerParameters,CurrentSRIMTables,0);
     if(targetZiegler.ZieglerFileSave())
     {
      CurrentZieglerParameters.Clear();
      CurrentZieglerParameters = targetZiegler.GetParameters();
      CurrentSRIMTables.Clear();
      CurrentSRIMTables = targetZiegler.GetTables();
     }
     else
     {
      return false;
     }
    }
    else
    {
     return false;
    }

    // Select the default Elements settings
    wxString ElementsFileName = MainDirectory + wxFileName::GetPathSeparator() + MainDatabase;
    wxString OriginalElements = rootconfig + wxFileName::GetPathSeparator() + MainDatabase;
     // Load, test and store the selected database on the selected folder
    wxFileName idf(ElementsFileName);
    wxString ElementsVersion = idf.GetExt();
    DatabaseFile testDatabase(OriginalElements,ElementsVersion,OpenDatabase);
   if(testDatabase.ERYAPIXEDatabaseFileLoad())
    {
     OpenDatabase = testDatabase.GetDatabase();
     ElementsVersion = wxT("epd");
     DatabaseFile targetDatabase(ElementsFileName,ElementsVersion,OpenDatabase);
     if(targetDatabase.ERYAPIXEDatabaseFileSave())
     {
      OpenDatabase.Clear();
      OpenDatabase = targetDatabase.GetDatabase();
     }
     else
     {
      return false;
     }
    }
    else
    {
     return false;
    }
    this->GenerateTable();
   this->GenerateLog();
   return true;
}

bool ERYAPIXEMainFrame::LoadSetupFile(wxString& MainDirectory, wxString& MainDetector, wxString& MainDatabase, wxString& MainZiegler)
{
  // Get the two compatible name paths
  wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
  wxFileName mainconfig(fileconfig);
  wxString rootconfig = mainconfig.GetPath();
  wxString pathconfig = rootconfig;
  wxString nameconfig = mainconfig.GetName();
  wxString standardconfig = ::wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + nameconfig + wxT(".conf");
  wxString portableconfig = ::wxStandardPaths::Get().GetExecutablePath() + wxT(".conf");
  // Clear variable contents
  MainDirectory.Clear();
  MainDetector.Clear();
  MainDetector.Clear();
  MainZiegler.Clear();
  // Load the file, while try to load at local profile, then the root profile.
  wxXmlDocument LocalSetupFile;
  if(wxFile::Exists(standardconfig))
  {
   if(!(LocalSetupFile.Load(standardconfig)))
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA-Bulk don't find any configuration file\nThe program will start the Configuration Wizard..."), wxT("No Setup files found"), wxOK | wxICON_ERROR);
   dial->ShowModal();
   return false;
   }
  }
  else if(wxFile::Exists(portableconfig))
  {
   if(!(LocalSetupFile.Load(portableconfig)))
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA-Bulk don't find any configuration file\nThe program will start the Configuration Wizard..."), wxT("No Setup files found"), wxOK | wxICON_ERROR);
   dial->ShowModal();
   return false;
   }
  }
  else
  {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA-Bulk don't find any configuration file\nThe program will start the Configuration Wizard..."), wxT("No Setup files found"), wxOK | wxICON_ERROR);
   dial->ShowModal();
   return false;
  }
  // Found a configuration file
   if(LocalSetupFile.GetRoot()->GetName() != wxT("ERYA-Bulk_Setup"))
   {
    wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("ERYA-Bulk Configuration File is invalid!\nThe program will start the Configuration Wizard..."), wxT("Setup File Loading Error"), wxOK | wxICON_ERROR);
    dial->ShowModal();
    return false;
   }
   // Begins the file processing
  wxXmlNode *SetupLabel = LocalSetupFile.GetRoot()->GetChildren();
  while(SetupLabel)
  {
   if(SetupLabel->GetName() == wxT("Setup_Contents"))
   {
    wxXmlNode *SetupNode = SetupLabel->GetChildren();
    while(SetupNode)
    {
      if(SetupNode->GetName() == wxT("Databases_Directory")) //Load the default database directory
      {
        wxXmlNode *DirectoryValue = SetupNode->GetChildren();
        if(DirectoryValue->GetName() == wxT("value"))
         {
           MainDirectory = DirectoryValue->GetNodeContent();
         }
      }
      if(SetupNode->GetName() == wxT("Detector_Setup")) //Load the default detector settings
      {
        wxXmlNode *DetectorValue = SetupNode->GetChildren();
        if(DetectorValue->GetName() == wxT("value"))
         {
           MainDetector = DetectorValue->GetNodeContent();
         }
      }
      if(SetupNode->GetName() == wxT("Databases_Elements")) //Load the default database contents
      {
        wxXmlNode *DatabaseValue = SetupNode->GetChildren();
        if(DatabaseValue->GetName() == wxT("value"))
         {
           MainDatabase = DatabaseValue->GetNodeContent();
         }
      }
      if(SetupNode->GetName() == wxT("Ziegler_Parameters")) //Load the default Ziegler parameters
      {
        wxXmlNode *ZieglerValue = SetupNode->GetChildren();
        if(ZieglerValue->GetName() == wxT("value"))
         {
           MainZiegler = ZieglerValue->GetNodeContent();
         }
      }
     SetupNode = SetupNode->GetNext();
    }
   }
   SetupLabel = SetupLabel->GetNext();
  }
 return true;
}

bool ERYAPIXEMainFrame::SaveSetupFile(wxString MainDirectory, wxString MainDetector, wxString MainDatabase, wxString MainZiegler, bool LocalSetting)
{
 // Get the current time
  wxDateTime ThisTime = wxDateTime::Now();
  wxString ActualTime =ThisTime.Format(wxT("%Y-%m-%dT%H:%M:%S.00Z"),wxDateTime::WET);
  // Get the two compatible name paths
  wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
  wxFileName mainconfig(fileconfig);
  wxString rootconfig = mainconfig.GetPath();
  wxString pathconfig = rootconfig;
  wxString nameconfig = mainconfig.GetName();
  wxString standardconfig = ::wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + nameconfig + wxT(".conf");
  wxString portableconfig = ::wxStandardPaths::Get().GetExecutablePath() + wxT(".conf");
  wxString controltag;
  wxString configfile;
  if(LocalSetting)
  {
   controltag = wxT("true");
   wxString localpathconfig = ::wxStandardPaths::Get().GetUserLocalDataDir();
   wxMkDir(localpathconfig,wxS_DIR_DEFAULT);
   configfile = standardconfig;
  }
  else
  {
   controltag = wxT("false");
   configfile = portableconfig;
  }
  // An xml file when written, all node are declared backwarks, but any atribute of the same node should be declared forwards
  wxXmlDocument LocalSetup;
  wxXmlNode* setup = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "ERYA-Bulk_Setup");
  LocalSetup.SetRoot(setup);
  // save file data
  wxXmlNode* setupdata = new wxXmlNode(setup, wxXML_ELEMENT_NODE, "Setup_Contents");
  wxXmlNode* mainzig = new wxXmlNode(setupdata, wxXML_ELEMENT_NODE, "Ziegler_Parameters");
   wxXmlNode *valuezig = new wxXmlNode(mainzig, wxXML_ELEMENT_NODE, "value");
    valuezig->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, MainZiegler));
  wxXmlNode* maindat = new wxXmlNode(setupdata, wxXML_ELEMENT_NODE, "Databases_Elements");
   wxXmlNode *valuedat = new wxXmlNode(maindat, wxXML_ELEMENT_NODE, "value");
    valuedat->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, MainDatabase));
  wxXmlNode* maindet = new wxXmlNode(setupdata, wxXML_ELEMENT_NODE, "Detector_Setup");
   wxXmlNode *valuedet = new wxXmlNode(maindet, wxXML_ELEMENT_NODE, "value");
    valuedet->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, MainDetector));
  wxXmlNode* maindir = new wxXmlNode(setupdata, wxXML_ELEMENT_NODE, "Databases_Directory");
   wxXmlNode *valuedir = new wxXmlNode(maindir, wxXML_ELEMENT_NODE, "value");
    valuedir->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, MainDirectory));
  wxXmlNode* maindef = new wxXmlNode(setupdata, wxXML_ELEMENT_NODE, "Databases_Default");
   wxXmlNode *valuedef = new wxXmlNode(maindef, wxXML_ELEMENT_NODE, "value");
    valuedef->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, controltag));
  // Save file header
  wxXmlNode* fileversion = new wxXmlNode(setup, wxXML_ELEMENT_NODE, "File_Details");
   wxXmlNode* details = new wxXmlNode(fileversion, wxXML_ELEMENT_NODE, "Contents");
   wxXmlNode* programfilename = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Actual_File_Name");
     programfilename->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, configfile));
   wxXmlNode* programdatetime = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Date_File_Creation");
     programdatetime->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, ActualTime));
   wxXmlNode* programversion = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Version");
     programversion->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("4.60")));
    wxXmlNode* programname = new wxXmlNode(details, wxXML_ELEMENT_NODE, "Program_Name");
     programname->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxEmptyString, wxT("ERYA-Bulk")));
  // Save the file
  return LocalSetup.Save(configfile,0);
}

bool ERYAPIXEMainFrame::DeleteSetupFile()
{
// Get the potential setup file on root directory (for portable installations, and local profile folders)
     wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
     wxFileName mainconfig(fileconfig);
     wxString rootconfig = mainconfig.GetPath();
     wxString nameconfig = mainconfig.GetName();
     wxString CheckLocalSetupFileName = ::wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + nameconfig + wxT(".conf");
     wxString CheckRootSetupFileName = ::wxStandardPaths::Get().GetExecutablePath() + wxT(".conf");
     wxString CheckUserProfileFolder = ::wxStandardPaths::Get().GetUserLocalDataDir();
   if(wxFileExists(CheckRootSetupFileName))
   {
   if(!(wxRemoveFile(CheckRootSetupFileName)))
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete file at ") + CheckRootSetupFileName + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
   }
   if(wxFileExists(CheckLocalSetupFileName))
   {
    if(!(wxRemoveFile(CheckLocalSetupFileName)))
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete file at ") + CheckLocalSetupFileName + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
   }
   if(wxDirExists(CheckUserProfileFolder))
   {
     wxString TestFileName = wxFindFirstFile(CheckUserProfileFolder + wxFileName::GetPathSeparator() + wxT("*.*"));
     do
     {
      if(wxFileExists(CheckUserProfileFolder + wxFileName::GetPathSeparator() + TestFileName))
      {
       if(!(wxRemoveFile(CheckUserProfileFolder + wxFileName::GetPathSeparator() + TestFileName)))
       {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete file at ") + CheckUserProfileFolder + wxFileName::GetPathSeparator() + TestFileName + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
       return false;
       }
      }
      TestFileName = wxFindNextFile();
     }while(TestFileName.Len()>0);
    if(!(wxFileName::Rmdir(CheckUserProfileFolder,wxPATH_RMDIR_RECURSIVE)))
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete folder ") + CheckUserProfileFolder + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
   }
   return true;
}

bool ERYAPIXEMainFrame::ReplaceSetupFile()
{
// Get the potential setup file on root directory (for portable installations, and local profile folders)
     wxString fileconfig = ::wxStandardPaths::Get().GetExecutablePath();
     wxFileName mainconfig(fileconfig);
     wxString rootconfig = mainconfig.GetPath();
     wxString nameconfig = mainconfig.GetName();
     wxString CheckLocalSetupFileName = ::wxStandardPaths::Get().GetUserLocalDataDir() + wxFileName::GetPathSeparator() + nameconfig + wxT(".conf");
     wxString CheckRootSetupFileName = ::wxStandardPaths::Get().GetExecutablePath() + wxT(".conf");
     wxString CheckUserProfileFolder = ::wxStandardPaths::Get().GetUserLocalDataDir();
   if(wxFileExists(CheckRootSetupFileName))
   {
   if(!(wxRemoveFile(CheckRootSetupFileName)))
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete file at ") + CheckRootSetupFileName + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
   }
   if(wxFileExists(CheckLocalSetupFileName))
   {
    if(!(wxRemoveFile(CheckLocalSetupFileName)))
    {
     wxMessageDialog *error = new wxMessageDialog(NULL, wxT("Cannot delete file at ") + CheckLocalSetupFileName + wxT("\nSince it was protected."), wxT("Error: Protected Folder"), wxOK | wxICON_ERROR);
     error->ShowModal();
     return false;
    }
   }
   return true;
}
