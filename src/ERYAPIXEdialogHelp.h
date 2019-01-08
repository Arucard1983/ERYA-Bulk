/***************************************************************
 * Name:      ERYAPIXEdialogHelp.h
 * Purpose:   ERYA Help Viewer implementation
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/
 
#ifndef ERYAPIXEDIALOGHELP_H_INCLUDED
#define ERYAPIXEDIALOGHELP_H_INCLUDED

/**
@file
Subclass of dialogHelp, which is generated by wxFormBuilder.
*/

#include "ERYAPIXE.h"

//// end generated include

/** Implementing dialogHelp */
class ERYAPIXEdialogHelp : public dialogHelp
{
	protected:
		// Handlers for dialoghelp events.
		void OnHelpMain( wxCommandEvent& event );
		void OnHelpIndex( wxCommandEvent& event );
		void OnHelpReadme( wxCommandEvent& event );
		void OnHelpBack( wxCommandEvent& event );
		void OnHelpForward( wxCommandEvent& event );
		void OnHelpClose( wxCommandEvent& event );
	public:
		/** Constructor */
       ERYAPIXEdialogHelp( wxWindow* parent, wxString HelpFile );
		ERYAPIXEdialogHelp( wxWindow* parent );
	//// end generated class members


};


#endif // ERYAPIXEDIALOGHELP_H_INCLUDED
