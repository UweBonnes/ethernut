#ifndef MAINFRAME_H_
#define MAINFRAME_H_

/* ----------------------------------------------------------------------------
 * Copyright (C) 2004 by egnite Software GmbH
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 * Parts are
 *
 * Copyright (C) 1998, 1999, 2000 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 */

/*
 * $Log$
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wx.h>
#include <wx/laywin.h>
#include <wx/splitter.h>
#include <wx/docview.h>
#include <wx/docmdi.h>
#include <wx/treectrl.h>

#include "configtree.h"
#include "conflictlist.h"
#include "valuewindow.h"
#include "splitscroll.h"
#include "infowindow.h"
#include "propertylist.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

class CMainFrame:public wxDocParentFrame {
  public:
    CMainFrame(wxDocManager * manager, const wxString & title);

    void OnSize(wxSizeEvent & event);
    void OnSashDrag(wxSashEvent & event);

    wxTextCtrl *GetOutputWindow() const;
    CConfigTree *GetTreeCtrl() const;
    CValueWindow *GetValueWindow() const;
    CInfoWindow *GetShortDescriptionWindow() const;
    CPropertyList *GetPropertyListWindow() const;
    CConflictList *GetConflictsWindow() const;

    /* ---------- Menu Event Handlers ---------- */
    void OnQuit(wxCommandEvent & event);
    void OnGenerateBuildTree(wxCommandEvent & event);

  protected:
    void CreateNutMenuBar();
    void CreateNutToolBar();
    void CreateNutStatusBar();
    void CreateNutWindows();

  protected:
     wxSashLayoutWindow * m_outputSashWindow;
    wxSashLayoutWindow *m_configSashWindow;
    wxSashLayoutWindow *m_conflictsSashWindow;
    wxSashLayoutWindow *m_propertiesSashWindow;
    wxSashLayoutWindow *m_shortDescrSashWindow;
    wxTextCtrl *m_outputWindow;
    wxSplitterWindow *m_splitter;
    CSplitScroll *m_scrolledWindow;
    CConfigTree *m_tree;
    CConflictList *m_conflictsWindow;
    CValueWindow *m_valueWindow;
    CInfoWindow *m_shortDescrWindow;
    CPropertyList *m_propertyListWindow;
  private:
     DECLARE_EVENT_TABLE()

};

#endif
