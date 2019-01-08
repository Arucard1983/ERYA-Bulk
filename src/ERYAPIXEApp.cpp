/***************************************************************
 * Name:      ERYAPIXEApp.cpp
 * Purpose:   ERYA Main Application Class
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

#include "ERYAPIXEApp.h"
#include "ERYAPIXEMainFrame.h"

IMPLEMENT_APP(ERYAPIXEApp);

bool ERYAPIXEApp::OnInit()
{

    ERYAPIXEMainFrame* frame = new ERYAPIXEMainFrame(0L);

    frame->Show();

    frame->Maximize(true);

    wxFileSystem::AddHandler(new wxZipFSHandler);

    wxImage::AddHandler( new wxXPMHandler );

    return true;
}
