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
 * $Log: mainframe.cpp,v $
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/config.h>

#include "ids.h"
#include "nutconf.h"
#include "settings.h"
#include "settingsdlg.h"
#include "mainframe.h"

BEGIN_EVENT_TABLE(CMainFrame, wxDocParentFrame)
    EVT_MENU(wxID_EXIT, CMainFrame::OnQuit)
    EVT_MENU(ID_GENERATE_BUILD_TREE, CMainFrame::OnGenerateBuildTree)
    EVT_SIZE(CMainFrame::OnSize)
    EVT_SASH_DRAGGED_RANGE(ID_CONFIG_SASH_WINDOW, ID_OUTPUT_SASH_WINDOW, CMainFrame::OnSashDrag)
    EVT_MENU(ID_SETTINGS, CMainFrame::OnSettings)


    END_EVENT_TABLE();

CMainFrame::CMainFrame(wxDocManager * manager, const wxString & title)
:wxDocParentFrame(manager, (wxFrame *) NULL, ID_MAIN_FRAME, title)
{
    CreateNutMenuBar();
    CreateNutToolBar();
    CreateNutStatusBar();
    CreateNutWindows();

    /*
     * Restore frame position and size.
     */
    wxConfigBase *pConfig = wxConfigBase::Get();
    Move(pConfig->Read(wxT("/MainFrame/x"), 50), pConfig->Read(wxT("/MainFrame/y"), 50));
    SetClientSize(pConfig->Read(wxT("/MainFrame/w"), 350), pConfig->Read(wxT("/MainFrame/h"), 200));
}

/*!
 * \brief Destructor.
 */
CMainFrame::~CMainFrame()
{
    // Save frame size and position.
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();
        pConfig->SetPath(wxT("/MainFrame"));

        /*
         * Frame window position and client window size.
         */
        int x, y;
        int w, h;
        GetPosition(&x, &y);
        pConfig->Write(wxT("x"), (long) x);
        pConfig->Write(wxT("y"), (long) y);
        GetClientSize(&w, &h);
        pConfig->Write(wxT("w"), (long) w);
        pConfig->Write(wxT("h"), (long) h);

        wxSize sz;
        sz = m_configSashWindow->GetSize();
        pConfig->Write(wxT("tree_w"), (long) sz.x);
        pConfig->Write(wxT("tree_h"), (long) sz.y);

        sz = m_propertiesSashWindow->GetSize();
        pConfig->Write(wxT("prop_w"), (long) sz.x);
        pConfig->Write(wxT("prop_h"), (long) sz.y);

        //sz = m_conflictsSashWindow->GetSize();
        //pConfig->Write(wxT("cflc_w"), (long) sz.x);
        //pConfig->Write(wxT("cflc_h"), (long) sz.y);

        sz = m_shortDescrSashWindow->GetSize();
        pConfig->Write(wxT("desc_w"), (long) sz.x);
        pConfig->Write(wxT("desc_h"), (long) sz.y);

        sz = m_outputSashWindow->GetSize();
        pConfig->Write(wxT("outp_w"), (long) sz.x);
        pConfig->Write(wxT("outp_h"), (long) sz.y);

        x = m_splitter->GetSashPosition();
        if(x > 0) {
            pConfig->Write(wxT("split_x"), (long) x);
        }
        pConfig->SetPath(lastPath);
    }
}

void CMainFrame::CreateNutMenuBar()
{
    wxMenu *fileMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    //td fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Creates a new document"));
    fileMenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Opens an existing document"));
    fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Saves the active document"));
    fileMenu->Append(wxID_SAVEAS, wxT("Save &As..."), wxT("Saves the active document with a new name"));
    //td fileMenu->AppendSeparator();
    //td fileMenu->Append(ID_IMPORT, wxT("&Import..."), wxT("Imports a minimal configuration exported from another configuration"));
    //td fileMenu->Append(ID_EXPORT, wxT("&Export..."), wxT("Exports a minimal configuration for importing into another configuration"));
    //td fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quits the application"));

    wxMenu *editMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    editMenu->Append(ID_SETTINGS, wxT("&Settings...\tCtrl+T"), wxT("Shows the application settings dialog"));
    //td editMenu->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"), wxT("Cuts the output pane selection and moves it to the Clipboard"));
    //td editMenu->Append(wxID_COPY, wxT("&Copy\tCtrl+C"), wxT("Copies the output pane selection to the clipboard"));
    //td editMenu->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"), wxT("Inserts Clipboard contents"));
    //td editMenu->Append(wxID_CLEAR, wxT("&Clear"), wxT("Erases everything in the output pane"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(wxID_SELECTALL, wxT("&Select All\tCtrl+A"), wxT("Selects the entire output pane"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(wxID_FIND, wxT("&Find...\tCtrl+F"), wxT("Finds the specified text"));
    //td editMenu->Append(ID_FIND_NEXT, wxT("Find &Next\tF3"), wxT("Finds the next item matching the Find text"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(ID_SAVE_OUTPUT, wxT("Sa&ve Output..."), wxT("Saves the contents of the output pane"));

    //td wxMenu *viewMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    //td viewMenu->Append(ID_SETTINGS, wxT("&Settings...\tCtrl+T"), wxT("Shows the application settings dialog"));
    //td viewMenu->AppendSeparator();
    //td viewMenu->Append(ID_TOOLBARS, wxT("&Toolbar"), wxT("Shows or hides the toolbar"), true);
    //td viewMenu->Append(ID_TOGGLE_PROPERTIES, wxT("&Properties\tAlt+1"), wxT("Shows or hides the properties window"), true);
    //td viewMenu->Append(ID_TOGGLE_OUTPUT, wxT("&Output\tAlt+2"), wxT("Shows the output window"), true);
    //td viewMenu->Append(ID_TOGGLE_SHORT_DESCR, wxT("&Short Description\tAlt+3"), wxT("Shows or hides the short description window"), true);
    //td viewMenu->Append(ID_TOGGLE_CONFLICTS, wxT("&Conflicts\tAlt+4"), wxT("Shows or hides the conflicts window"), true);

    wxMenu *buildMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    //td buildMenu->Append(ID_BUILD_LIBRARY, wxT("&Library\tF7"), wxT("Builds the library"));
    //td buildMenu->Append(ID_BUILD_TESTS, wxT("&Tests\tShift+F7"), wxT("Builds the tests"));
    //td buildMenu->Append(ID_CLEAN, wxT("&Clean"), wxT("Deletes intermediate and output files"));
    //td buildMenu->Append(ID_STOP_BUILD, wxT("&Stop"), wxT("Stops the build"));
    //td buildMenu->AppendSeparator();
    buildMenu->Append(ID_GENERATE_BUILD_TREE, wxT("&Generate Build Tree"), wxT("Explicitly recreates the build tree"));
    //td buildMenu->AppendSeparator();
    //td buildMenu->Append(ID_BUILD_OPTIONS, wxT("&Options..."), wxT("Changes build options"));
    //td buildMenu->Append(ID_BUILD_REPOSITORY, wxT("&Repository..."), wxT("Selects repository"));
    //td buildMenu->Append(ID_BUILD_TEMPLATES, wxT("&Templates..."), wxT("Selects the package templates"));
    //td buildMenu->Append(ID_BUILD_PACKAGES, wxT("&Packages..."), wxT("Selects individual packages"));

    //td wxMenu *toolsMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    //td wxMenu *pathMenu = new wxMenu;
    //td pathMenu->Append(ID_PATHS_BUILD_TOOLS, wxT("&Build Tools..."), wxT("Specifies the folder containing the build tools"));
    //td pathMenu->Append(ID_PATHS_USER_TOOLS, wxT("&User Tools..."), wxT("Specifies the folder containing the user tools"));
    //td toolsMenu->Append(ID_PATHS, wxT("&Paths"), pathMenu);
    //td toolsMenu->Append(ID_SHELL, wxT("&Shell..."), wxT("Invokes a command shell"));
    //td toolsMenu->Append(ID_RUN_TESTS, wxT("&Run Tests...\tCtrl+F5"), wxT("Runs the configuration tests"));
    //td toolsMenu->Append(ID_PLATFORMS, wxT("&Platforms..."), wxT("Edits the platforms list"));
    //td toolsMenu->Append(ID_RESOLVE_CONFLICTS, wxT("Resolve &Conflicts..."), wxT("Resolves conflicts"));
    //td toolsMenu->Append(ID_ADMINISTRATION, wxT("&Administration..."), wxT("Performs repository administration tasks"));
    //td toolsMenu->AppendSeparator();
    //td toolsMenu->Append(ID_INDEX_DOCS, wxT("Regenerate Help &Index"), wxT("Regenerates the online help contents"));

    //td wxMenu *helpMenu = new wxMenu;
    //td helpMenu->Append(ID_CONFIGTOOL_HELP, wxT("&Configuration Tool Help\tShift+F1"), wxT("Displays help"));
    //td helpMenu->Append(ID_NUTOS_HELP, wxT("&Nut/OS Documentation"), wxT("Displays the documentation home page"));
    //td helpMenu->Append(ID_CONTEXT_HELP, wxT("&Help On..."), wxT("Displays help for clicked-on windows"));
    //td helpMenu->AppendSeparator();

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(editMenu, wxT("&Edit"));
    //td menuBar->Append(viewMenu, wxT("&View"));
    menuBar->Append(buildMenu, wxT("&Build"));
    //td menuBar->Append(toolsMenu, wxT("&Tools"));
    //td menuBar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menuBar);
}


void CMainFrame::CreateNutToolBar()
{
    wxToolBarBase *toolBar = CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL | wxTB_DOCKABLE, ID_TOOLBAR);
    toolBar->SetMargins(4, 4);

    //td toolBar->AddTool(wxID_NEW, wxBITMAP(TBB_NEW), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("New file"));
    toolBar->AddTool(wxID_OPEN, wxBITMAP(TBB_OPEN), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Open file"));
    toolBar->AddTool(wxID_SAVE, wxBITMAP(TBB_SAVE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Save file"));
    //td toolBar->AddSeparator();
    //td toolBar->AddTool(wxID_CUT, wxBITMAP(TBB_CUT), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Cut");
    //td toolBar->AddTool(wxID_COPY, wxBITMAP(TBB_COPY), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Copy");
    //td toolBar->AddTool(wxID_PASTE, wxBITMAP(TBB_PASTE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Paste");
    //td toolBar->AddTool(wxID_FIND, wxBITMAP(TBB_SEARCH), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Search");
    //td toolBar->AddSeparator();
    //td toolBar->AddTool(ID_STOP_BUILD, wxBITMAP(TBB_STOPBUILD), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Stop build");
    //td toolBar->AddTool(ID_BUILD_LIBRARY, wxBITMAP(TBB_BUILDLIBRARY), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Build library");
    //td toolBar->AddSeparator();
    //td toolBar->AddTool(ID_CONTEXT_HELP, wxBITMAP(TBB_CSHELP), wxNullBitmap, false, -1, -1, (wxObject *) NULL,
    //td                  "Show help for clicked-on windows");
    //td toolBar->AddTool(ID_NUTOS_HELP, wxBITMAP(TBB_HELP), wxNullBitmap, false, -1, -1, (wxObject *) NULL, "Show help");
    toolBar->Realize();

    toolBar->SetHelpText(wxT("The toolbar allows quick access to commonly-used commands."));
}

void CMainFrame::CreateNutStatusBar()
{
    CreateStatusBar(4, wxST_SIZEGRIP);
    int widths[] = { -1, 100, 40, 80 };
    SetStatusWidths(4, widths);
    SetStatusText(wxT("Ready"), 0);
}

void CMainFrame::CreateNutWindows()
{
    wxSize sz;
    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString lastPath = pConfig->GetPath();

    pConfig->SetPath(wxT("/MainFrame"));

    m_outputSashWindow = new wxSashLayoutWindow(this, ID_OUTPUT_SASH_WINDOW);
    sz = wxSize(pConfig->Read(wxT("outp_w"), 100), pConfig->Read(wxT("outp_h"), 50));
    m_outputSashWindow->SetDefaultSize(sz);
    m_outputSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_outputSashWindow->SetAlignment(wxLAYOUT_BOTTOM);
    m_outputSashWindow->SetSashVisible(wxSASH_TOP, true);

    m_configSashWindow = new wxSashLayoutWindow(this, ID_CONFIG_SASH_WINDOW);
    sz = wxSize(pConfig->Read(wxT("tree_w"), 100), pConfig->Read(wxT("tree_h"), 50));
    m_configSashWindow->SetDefaultSize(sz);
    m_configSashWindow->SetOrientation(wxLAYOUT_VERTICAL);
    m_configSashWindow->SetAlignment(wxLAYOUT_LEFT);
    m_configSashWindow->SetSashVisible(wxSASH_RIGHT, true);

#if 0
    m_conflictsSashWindow =
        new wxSashLayoutWindow(this, ID_CONFLICTS_SASH_WINDOW, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxCLIP_SIBLINGS);
    sz = wxSize(pConfig->Read(wxT("cflc_w"), 100), pConfig->Read(wxT("cflc_h"), 50));
    m_conflictsSashWindow->SetDefaultSize(sz);
    m_conflictsSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_conflictsSashWindow->SetAlignment(wxLAYOUT_TOP);
    m_conflictsSashWindow->SetSashVisible(wxSASH_BOTTOM, true);
#endif

    m_propertiesSashWindow = new wxSashLayoutWindow(this, ID_PROPERTIES_SASH_WINDOW);
    sz = wxSize(pConfig->Read(wxT("prop_w"), 100), pConfig->Read(wxT("prop_h"), 50));
    m_propertiesSashWindow->SetDefaultSize(sz);
    m_propertiesSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_propertiesSashWindow->SetAlignment(wxLAYOUT_TOP);
    m_propertiesSashWindow->SetSashVisible(wxSASH_BOTTOM, true);

    m_shortDescrSashWindow = new wxSashLayoutWindow(this, ID_SHORT_DESCR_SASH_WINDOW);
    sz = wxSize(pConfig->Read(wxT("desc_w"), 100), pConfig->Read(wxT("desc_h"), 50));
    m_shortDescrSashWindow->SetDefaultSize(sz);
    m_shortDescrSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_shortDescrSashWindow->SetAlignment(wxLAYOUT_TOP);

    m_outputWindow = new wxTextCtrl(m_outputSashWindow, ID_OUTPUT_WINDOW);
    //m_conflictsWindow = new CConflictList(m_conflictsSashWindow, ID_CONFLICTS_WINDOW,
    //                                      wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxCLIP_CHILDREN | wxSUNKEN_BORDER);
    m_propertyListWindow = new CPropertyList(m_propertiesSashWindow, ID_PROPERTIES_WINDOW,
                                             wxDefaultPosition, wxDefaultSize,
                                             wxLC_REPORT | wxCLIP_CHILDREN | wxLC_VRULES | wxLC_HRULES | wxSUNKEN_BORDER);
    m_shortDescrWindow =
        new CInfoWindow(m_shortDescrSashWindow, ID_SHORT_DESCR_WINDOW, wxDefaultPosition, wxDefaultSize,
                        wxTE_MULTILINE | wxCLIP_CHILDREN | wxTE_READONLY);

    m_scrolledWindow = new CSplitScroll(m_configSashWindow, ID_SCROLLED_WINDOW, wxDefaultPosition,
                                        wxSize(400, 100), wxNO_BORDER | wxCLIP_CHILDREN | wxVSCROLL);
    m_splitter = new wxSplitterWindow(m_scrolledWindow, ID_SPLITTER_WINDOW, wxDefaultPosition,
                                      wxSize(400, 100), wxSP_3DBORDER | wxCLIP_CHILDREN);
    m_splitter->SetSashSize(5);
    m_tree = new CConfigTree(m_splitter, ID_TREE_CTRL,
                             wxDefaultPosition, wxSize(200, 100), wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxNO_BORDER);

    m_valueWindow = new CValueWindow(m_splitter, ID_VALUE_WINDOW, wxDefaultPosition, wxSize(200, 100), wxNO_BORDER);

    m_splitter->SplitVertically(m_tree, m_valueWindow);
    m_splitter->SetMinimumPaneSize(100);
    int x = pConfig->Read(wxT("split_x"), 200);
    if(x <= 0) {
        x = 200;    
    }
    m_splitter->SetSashPosition(x);

    m_scrolledWindow->SetTargetWindow(m_tree);
    m_scrolledWindow->EnableScrolling(false, false);

    // Let the two controls know about each other
    m_valueWindow->SetTreeCtrl(m_tree);
    m_tree->SetCompanionWindow(m_valueWindow);

    pConfig->SetPath(lastPath);
}

wxTextCtrl *CMainFrame::GetOutputWindow() const
{
    return m_outputWindow;
}

CConfigTree *CMainFrame::GetTreeCtrl() const
{
    return m_tree;
}

CValueWindow *CMainFrame::GetValueWindow() const
{
    return m_valueWindow;
}

CInfoWindow *CMainFrame::GetShortDescriptionWindow() const
{
    return m_shortDescrWindow;
}

CPropertyList *CMainFrame::GetPropertyListWindow() const
{
    return m_propertyListWindow;
}

//CConflictList *CMainFrame::GetConflictsWindow() const
//{
//    return m_conflictsWindow;
//}

void CMainFrame::OnSize(wxSizeEvent & WXUNUSED(event))
{
    wxLayoutAlgorithm layout;
    layout.LayoutFrame(this);
}

void CMainFrame::OnSashDrag(wxSashEvent & event)
{
    if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
        return;

    switch (event.GetId()) {
    case ID_CONFIG_SASH_WINDOW:
        m_configSashWindow->SetDefaultSize(wxSize(event.GetDragRect().width, 2000));
        break;
    //case ID_CONFLICTS_SASH_WINDOW:
    //    m_conflictsSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
    //    break;
    case ID_PROPERTIES_SASH_WINDOW:
        m_propertiesSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    case ID_SHORT_DESCR_SASH_WINDOW:
        m_shortDescrSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    case ID_OUTPUT_SASH_WINDOW:
        m_outputSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    }

    wxLayoutAlgorithm layout;
    if (!layout.LayoutFrame(this)) {
        wxNode *node = GetChildren().First();
        while (node) {
            wxWindow *win = (wxWindow *) node->Data();
            if (win->IsKindOf(CLASSINFO(wxSashLayoutWindow))) {
                wxSashLayoutWindow *sashWin = (wxSashLayoutWindow *) win;
                wxSize sz = sashWin->GetSize();
                sashWin->SetDefaultSize(sz);
            }
            node = node->Next();
        }
    }
}

/* ------------------------- Menu Events ------------------------- */

void CMainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close();
}

void CMainFrame::OnSettings(wxCommandEvent& event)
{
    CSettingsDialog dialog(this);
    dialog.ShowModal();
}

void CMainFrame::OnGenerateBuildTree(wxCommandEvent & event)
{
    if (wxGetApp().GetNutConfDoc()) {
        wxGetApp().GetNutConfDoc()->GenerateBuildTree();
    }
}

