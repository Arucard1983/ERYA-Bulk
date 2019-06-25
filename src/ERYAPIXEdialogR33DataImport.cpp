/***************************************************************
 * Name:      ERYAPIXEdialogR33DataImport.cpp
 * Purpose:   ERYA ITN/R33 Text Data Import Tool functions
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2017-01-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

 #include "ERYAPIXEdialogR33DataImport.h"
 #include "ERYAPIXEdialogAddDatabase.h"

ERYAPIXEdialogR33DataImport::ERYAPIXEdialogR33DataImport( wxWindow* parent)
:
dialogR33DataImport( parent )
{

}

void ERYAPIXEdialogR33DataImport::OnDataImportValidate( wxCommandEvent& event )
{
 ERYAPIXEdialogAddDatabase *Parent = (ERYAPIXEdialogAddDatabase *) GetParent();
 Parent->SetSelectUnit(radioDataImportUnits->GetSelection());
 Parent->SetSelectAngle(radioDataImportAngle->GetSelection());
 Parent->SetIgnoreInvalidLine(checkIgnoreNonNumericalData->GetValue());
 Close();
}

void ERYAPIXEdialogR33DataImport::OnDataImportAbort( wxCommandEvent& event )
{
 ERYAPIXEdialogAddDatabase *Parent = (ERYAPIXEdialogAddDatabase *) GetParent();
 Parent->SetSelectUnit(0);
 Parent->SetSelectAngle(0);
 Parent->SetIgnoreInvalidLine(false);
 Close();
}

void ERYAPIXEdialogR33DataImport::OnDataImportReset( wxCommandEvent& event )
{
 radioDataImportUnits->SetSelection(0);
 radioDataImportAngle->SetSelection(0);
 checkIgnoreNonNumericalData->SetValue(true);
}

void ERYAPIXEdialogR33DataImport::OnDataImportHelp( wxCommandEvent& event )
{
 Close();
}
