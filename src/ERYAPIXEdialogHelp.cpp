/***************************************************************
 * Name:      ERYAPIXEdialogHelp.cpp
 * Purpose:   ERYA Help Viewer implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#include "ERYAPIXEdialogHelp.h"
#include "ERYAPIXEMainFrame.h"
#include "ERYAPIXEdialogAddDatabase.h"
#include "ERYAPIXEdialogDatabaseManager.h"
#include "ERYAPIXEdialogDetectorSetup.h"
#include "ERYAPIXEdialogZieglerParameters.h"
#include "ERYAPIXEdialogAdvanced.h"
#include "ERYAPIXEdialogR33DataImport.h"
#include "ERYAPIXEdialogXlsxDataImport.h"

ERYAPIXEdialogHelp::ERYAPIXEdialogHelp( wxWindow* parent )
:
dialogHelp( parent )
{

}

ERYAPIXEdialogHelp::ERYAPIXEdialogHelp( wxWindow* parent, wxString HelpFile)
:
dialogHelp( parent )
{
  wxString rootpath = ::wxStandardPaths::Get().GetExecutablePath();
  wxFileName rootfile(rootpath);
  rootpath = rootfile.GetPath();
  pageHelpViewer->LoadPage(rootpath + wxFileName::GetPathSeparator() + HelpFile);
  helpMainPage = HelpFile;
}

void ERYAPIXEdialogHelp::OnHelpMain( wxCommandEvent& event )
{
 wxString rootpath = ::wxStandardPaths::Get().GetExecutablePath();
 wxFileName rootfile(rootpath);
 rootpath = rootfile.GetPath();
 pageHelpViewer->LoadPage(rootpath + wxFileName::GetPathSeparator() + helpMainPage);
}

void ERYAPIXEdialogHelp::OnHelpIndex( wxCommandEvent& event )
{
 wxString rootpath = ::wxStandardPaths::Get().GetExecutablePath();
 wxFileName rootfile(rootpath);
 rootpath = rootfile.GetPath();
 pageHelpViewer->LoadPage(rootpath + wxFileName::GetPathSeparator() + wxT("Index.html"));
}

void ERYAPIXEdialogHelp::OnHelpReadme( wxCommandEvent& event )
{
 wxString rootpath = ::wxStandardPaths::Get().GetExecutablePath();
 wxFileName rootfile(rootpath);
 rootpath = rootfile.GetPath();
 pageHelpViewer->LoadPage(rootpath + wxFileName::GetPathSeparator() + wxT("readme.txt"));
}

void ERYAPIXEdialogHelp::OnHelpBack( wxCommandEvent& event )
{
  pageHelpViewer->HistoryBack();
}

void ERYAPIXEdialogHelp::OnHelpForward( wxCommandEvent& event )
{
  pageHelpViewer->HistoryForward();
}

void ERYAPIXEdialogHelp::OnHelpClose( wxCommandEvent& event )
{
 Close();
}
