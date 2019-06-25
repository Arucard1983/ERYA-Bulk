/***************************************************************
 * Name:      ERYAPIXEdialogXlsxDataImport.cpp
 * Purpose:   ERYA Xlsx Data Import Tool functions
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2017-01-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

 #include "ERYAPIXEdialogXlsxDataImport.h"
 #include "ERYAPIXEdialogAddDatabase.h"

ERYAPIXEdialogXlsxDataImport::ERYAPIXEdialogXlsxDataImport( wxWindow* parent)
:
dialogXlsxDataImport( parent )
{

}

void ERYAPIXEdialogXlsxDataImport::OnDataImportValidate( wxCommandEvent& event )
{
 ERYAPIXEdialogAddDatabase *Parent = (ERYAPIXEdialogAddDatabase *) GetParent();
 Parent->SetSelectUnit(radioDataImportUnits->GetSelection());
 Parent->SetSelectAngle(radioDataImportAngle->GetSelection());
 Parent->SetSelectMode(radioDataImportColumn->GetSelection());
 Close();
}

void ERYAPIXEdialogXlsxDataImport::OnDataImportAbort( wxCommandEvent& event )
{
 ERYAPIXEdialogAddDatabase *Parent = (ERYAPIXEdialogAddDatabase *) GetParent();
 Parent->SetSelectUnit(0);
 Parent->SetSelectAngle(0);
 Parent->SetSelectMode(0);
 Close();
}

void ERYAPIXEdialogXlsxDataImport::OnDataImportReset( wxCommandEvent& event )
{
 radioDataImportUnits->SetSelection(0);
 radioDataImportAngle->SetSelection(0);
 radioDataImportColumn->SetSelection(0);
}

void ERYAPIXEdialogXlsxDataImport::OnDataImportHelp( wxCommandEvent& event )
{
 Close();
}
