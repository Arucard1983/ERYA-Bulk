/***************************************************************
 * Name:      ERYAPIXEdialogZieglerParameters.h
 * Purpose:   ERYA Ziegler Bragg parameters class
 * Author:    Vasco Manteigas (vm.manteigas@campus.fct.utl.pt)
 * Created:   2015-12-01
 * Copyright: Grupo de Reacoes Nucleares - LIBPHYS
 * License:
 **************************************************************/

#ifndef __ERYAPIXEdialogZieglerParameters__
#define __ERYAPIXEdialogZieglerParameters__

/**
@file
Subclass of dialogZieglerParameters, which is generated by wxFormBuilder.
*/

#include "ERYAPIXE.h"
#include "FileLibrary.h"
#include "ParserLibrary.h"
#include "DatabaseLibrary.h"

//// end generated include

/** Implementing dialogZieglerParameters */
class ERYAPIXEdialogZieglerParameters : public dialogZieglerParameters
{
	protected:
		// Handlers for dialogZieglerParameters events.
		void OnZieglerSelectElement( wxSpinEvent& event );
		void OnZieglerSelectElementText( wxCommandEvent& event );
		void OnZieglerSelectElementEnter( wxCommandEvent& event );
                void OnUpdateZieglerSRIM( wxCommandEvent& event );
		void OnEraseZieglerSRIM( wxCommandEvent& event );
		void OnZieglerSelect( wxGridRangeSelectEvent& event );
                void OnZieglerCopy( wxCommandEvent& event );
		void OnZieglerPaste( wxCommandEvent& event );
		void OnZieglerLoad( wxCommandEvent& event );
		void OnZieglerSave( wxCommandEvent& event );
		void OnZieglerClear( wxCommandEvent& event );
		void OnZieglerOK( wxCommandEvent& event );
		void OnZieglerCancel( wxCommandEvent& event );
		void OnZieglerHelp( wxCommandEvent& event );
    private:
        ElementSRIMArray LocalSRIMTable;
	public:
		/** Constructor */
		ERYAPIXEdialogZieglerParameters( wxWindow* parent );
	//// end generated class members


};

#endif // __ERYAPIXEdialogZieglerParameters__
