/***************************************************************
 * Name:      ERYAPIXEdialogAdvanced.cpp
 * Purpose:   ERYA Fitting Tuning applet implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/


#include "ERYAPIXEdialogAdvanced.h"
#include "ERYAPIXEMainFrame.h"


ERYAPIXEdialogAdvanced::ERYAPIXEdialogAdvanced( wxWindow* parent )
:
dialogAdvancedFit( parent )
{
 // Reset to the default values
 int LocalIterations, LocalTau, LocalYield, LocalStoichiometry;
 LocalIterations = 100;
 LocalTau = 3;
 LocalYield = 3;
 LocalStoichiometry = 3;
 // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Sync the local data from parent frame to child frame
 Parent->GetFitParameters(LocalIterations,LocalTau,LocalYield,LocalStoichiometry);
 spinNumberIterations->SetValue(LocalIterations);
 spinDampingFactor->SetValue(LocalTau);
 spinYieldConvergence->SetValue(LocalYield);
 spinStoichiometryConvergence->SetValue(LocalStoichiometry);
}


void ERYAPIXEdialogAdvanced::OnAdvancedOK( wxCommandEvent& event )
{
 int LocalIterations, LocalTau, LocalYield, LocalStoichiometry;
 LocalIterations = spinNumberIterations->GetValue();
 LocalTau = spinDampingFactor->GetValue();
 LocalYield = spinYieldConvergence->GetValue();
 LocalStoichiometry = spinStoichiometryConvergence->GetValue();
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 Parent->SyncFitParameters(LocalIterations,LocalTau,LocalYield,LocalStoichiometry);
 Close();
}

void ERYAPIXEdialogAdvanced::OnAdvancedCancel( wxCommandEvent& event )
{
  Close();
}

void ERYAPIXEdialogAdvanced::OnAdvancedReset( wxCommandEvent& event )
{
  // Set the default values
 int LocalIterations, LocalTau, LocalYield, LocalStoichiometry;
 LocalIterations = 100;
 LocalTau = 3;
 LocalYield = 3;
 LocalStoichiometry = 3;
 // Call the parent frame
 ERYAPIXEMainFrame *Parent = (ERYAPIXEMainFrame *) GetParent();
 // Sync the local data from parent frame to child frame
 Parent->SyncFitParameters(LocalIterations,LocalTau,LocalYield,LocalStoichiometry);
 // Set the appropriate selectors
 spinNumberIterations->SetValue(100);
 spinDampingFactor->SetValue(3);
 spinYieldConvergence->SetValue(3);
 spinStoichiometryConvergence->SetValue(3);
}

void ERYAPIXEdialogAdvanced::OnAdvancedHelp( wxCommandEvent& event )
{
 Close();
}
