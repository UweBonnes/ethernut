/* ----------------------------------------------------------------------------
 * Copyright (C) 2004-2007 by egnite Software GmbH
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
 * $Log: bldoptdlg.cpp,v $
 * Revision 1.5  2007/04/25 16:01:39  haraldkipp
 * Path entry validator added.
 * Transfer functions return actual result.
 *
 * Revision 1.4  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.3  2005/04/22 15:07:12  haraldkipp
 * Avoid compiler warnings.
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#ifdef __GNUG__
    #pragma implementation "bldoptdlg.h"
#endif

#include <wx/valgen.h>
#include <wx/dir.h>

#include "ids.h"
#include "nutconf.h"
#include "pathvalidator.h"
#include "bldoptdlg.h"

IMPLEMENT_CLASS(CBuildOptionsDialog, wxPanel)

BEGIN_EVENT_TABLE(CBuildOptionsDialog, wxPanel)
    EVT_BUTTON(ID_BROWSE_BUTTON, CBuildOptionsDialog::OnBrowseBuildPath)
    EVT_BUTTON(ID_BROWSE_SRCDIR, CBuildOptionsDialog::OnBrowseSourceDir)
    EVT_BUTTON(ID_BROWSE_INSTALL, CBuildOptionsDialog::OnBrowseInstallPath)
    EVT_TEXT_ENTER(ID_COMBO_SRCDIR, CBuildOptionsDialog::OnPlatformEnter)
    EVT_TEXT(ID_ENTRY_SRCDIR, CBuildOptionsDialog::OnSourceDirChange) 
END_EVENT_TABLE()

CBuildOptionsDialog::CBuildOptionsDialog(wxWindow* parent)
: wxPanel(parent, ID_SETTINGS_BUILD)
{
    CSettings *opts = wxGetApp().GetSettings();
    CPathValidator srcDirValid(VALIDPATH_NOT_EMPTY | VALIDPATH_IS_DIRECTORY | VALIDPATH_EXISTS | VALIDPATH_SHOW_NATIVE | VALIDPATH_TO_UNIX, &opts->m_source_dir);
    CPathValidator firstIncValid(VALIDPATH_LIST | VALIDPATH_IS_DIRECTORY | VALIDPATH_EXISTS | VALIDPATH_SHOW_NATIVE| VALIDPATH_TO_UNIX, &opts->m_firstidir);
    CPathValidator lastIncValid(VALIDPATH_LIST | VALIDPATH_IS_DIRECTORY | VALIDPATH_EXISTS | VALIDPATH_SHOW_NATIVE | VALIDPATH_TO_UNIX, &opts->m_lastidir);
    CPathValidator bldDirValid(VALIDPATH_NOT_EMPTY | VALIDPATH_IS_DIRECTORY | VALIDPATH_SHOW_NATIVE | VALIDPATH_TO_UNIX, &opts->m_buildpath);
    CPathValidator libDirValid(VALIDPATH_NOT_EMPTY | VALIDPATH_IS_DIRECTORY | VALIDPATH_SHOW_NATIVE | VALIDPATH_TO_UNIX, &opts->m_lib_dir);

    wxStaticBox *grpSource = new wxStaticBox(this, -1, wxT("Source Directory"));
    m_entSourceDir = new wxTextCtrl(this, ID_ENTRY_SRCDIR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, srcDirValid);
    wxButton *btnSourceDir = new wxButton(this, ID_BROWSE_SRCDIR, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0);
    wxStaticText *lblPlatform = new wxStaticText(this, -1, wxT("Platform"));
    m_cbxPlatform = new wxComboBox(this, ID_COMBO_SRCDIR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxGenericValidator(&opts->m_platform));

    wxStaticBox *grpBuild = new wxStaticBox(this, -1, wxT("Build Directory"));
    m_entBuildDir = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, bldDirValid);
    wxButton *btnBrowseBuild = new wxButton(this, ID_BROWSE_BUTTON, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0);

    wxStaticBox *grpInclude = new wxStaticBox(this, -1, wxT("Include Directories"));
    wxStaticText *lblFirst = new wxStaticText(this, -1, wxT("First"));
    m_entInclFirstDir = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, firstIncValid);
    wxButton *btnBrowseInclFirst = new wxButton(this, ID_BROWSE_INCLFIRST, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0);
    wxStaticText *lblLast = new wxStaticText(this, -1, wxT("Last"));
    m_entInclLastDir = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, lastIncValid);
    wxButton *btnBrowseInclLast = new wxButton(this, ID_BROWSE_INCLLAST, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0);

    wxStaticBox *grpInstall = new wxStaticBox(this, -1, wxT("Install Directory"));
    m_entInstallDir = new wxTextCtrl(this, ID_PATH_INSTALL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, libDirValid);
    wxButton *btnBrowseInstall = new wxButton(this, ID_BROWSE_INSTALL, wxT("Browse..."), wxDefaultPosition, wxDefaultSize, 0);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    wxSizer *szrSource = new wxStaticBoxSizer(grpSource, wxVERTICAL);
    wxSizer *szrSourceDir = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *szrPlatform = new wxBoxSizer(wxHORIZONTAL);

    wxSizer *szrInclude = new wxStaticBoxSizer(grpInclude, wxVERTICAL);
    wxSizer *szrInclFirst = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *szrInclLast = new wxBoxSizer(wxHORIZONTAL);

    wxSizer *sizerBuildDir = new wxStaticBoxSizer(grpBuild, wxHORIZONTAL);
    wxSizer *sizerInstallDir = new wxStaticBoxSizer(grpInstall, wxHORIZONTAL);

    szrSourceDir->Add(m_entSourceDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrSourceDir->Add(btnSourceDir, 0, wxALIGN_RIGHT | wxGROW | wxALL, 5);

    szrPlatform->Add(lblPlatform, 0, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrPlatform->Add(m_cbxPlatform, 0, wxALIGN_LEFT | wxGROW | wxALL, 5);

    szrSource->Add(szrSourceDir, 0, wxGROW | wxALL, 5);
    szrSource->Add(szrPlatform, 0, wxGROW | wxALL, 5);



    szrInclFirst->Add(lblFirst, 0, wxALIGN_LEFT | wxALL, 5);
    szrInclFirst->Add(m_entInclFirstDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrInclFirst->Add(btnBrowseInclFirst, 0, wxALIGN_RIGHT | wxALL, 5);

    szrInclLast->Add(lblLast, 0, wxALIGN_LEFT | wxALL, 5);
    szrInclLast->Add(m_entInclLastDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrInclLast->Add(btnBrowseInclLast, 0, wxALIGN_RIGHT | wxALL, 5);

    szrInclude->Add(szrInclFirst, 0, wxGROW | wxALL, 5);
    szrInclude->Add(szrInclLast, 0, wxGROW | wxALL, 5);


    sizerBuildDir->Add(m_entBuildDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    sizerBuildDir->Add(btnBrowseBuild, 0, wxALIGN_RIGHT | wxALL, 5);

    sizerInstallDir->Add(m_entInstallDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    sizerInstallDir->Add(btnBrowseInstall, 0, wxALIGN_RIGHT | wxALL, 5);

    sizerTop->Add(szrSource, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);
    sizerTop->Add(szrInclude, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);
    sizerTop->Add(sizerBuildDir, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);
    sizerTop->Add(sizerInstallDir, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);

    SetAutoLayout(true);
    SetSizer(sizerTop);

    PopulatePlatform();
}

bool CBuildOptionsDialog::TransferDataToWindow()
{
    return wxPanel::TransferDataToWindow();
}

bool CBuildOptionsDialog::TransferDataFromWindow()
{
    return wxPanel::TransferDataFromWindow();
}

void CBuildOptionsDialog::OnBrowseBuildPath(wxCommandEvent& WXUNUSED(event))
{
    wxString path = ((wxTextCtrl*)FindWindow(ID_PATH_ENTRY))->GetValue();

    wxDirDialog dlg(this, wxT("Choose a build directory"), path, wxDD_NEW_DIR_BUTTON);

    if (dlg.ShowModal() == wxID_OK) {
        wxString val = dlg.GetPath();
#ifdef __WXMSW__
        val.Replace(wxT("\\"), wxT("/"));
#endif
        ((wxTextCtrl*)FindWindow(ID_PATH_ENTRY))->SetValue(val);
    }
}

void CBuildOptionsDialog::OnBrowseSourceDir(wxCommandEvent& WXUNUSED(event))
{
    wxString path = m_entSourceDir->GetValue();

    wxDirDialog dlg(this, wxT("Choose a source directory"), path);

    if (dlg.ShowModal() == wxID_OK) {
        wxString val = dlg.GetPath();
#ifdef __WXMSW__
        val.Replace(wxT("\\"), wxT("/"));
#endif
        m_entSourceDir->SetValue(val);
    }
}

void CBuildOptionsDialog::OnBrowseInstallPath(wxCommandEvent& WXUNUSED(event))
{
    wxString path = m_entInstallDir->GetValue();

    wxDirDialog dlg(this, wxT("Choose an install directory"), path, wxDD_NEW_DIR_BUTTON);

    if (dlg.ShowModal() == wxID_OK) {
        wxString val = dlg.GetPath();
#ifdef __WXMSW__
        val.Replace(wxT("\\"), wxT("/"));
#endif
        m_entInstallDir->SetValue(val);
    }
}


void CBuildOptionsDialog::OnPlatformEnter(wxCommandEvent& WXUNUSED(event))
{
}

void CBuildOptionsDialog::PopulatePlatform()
{
    //CSettings *opts = wxGetApp().GetSettings();
    wxString src_dir = m_entSourceDir->GetValue();
    wxString platform = m_cbxPlatform->GetValue();

    if(wxDir::Exists(src_dir)) {
        if(m_lastSourceDir != src_dir) {
            m_lastSourceDir = src_dir;
            wxDir dir(src_dir);
            if(dir.IsOpened()) {
                wxString entry;

                m_cbxPlatform->Clear();
                bool cont = dir.GetFirst(&entry, wxT("Makedefs.*"));
                while (cont) {
                    m_cbxPlatform->Append(entry.AfterLast('.'));
                    cont = dir.GetNext(&entry);
                }
                if(!platform.IsEmpty()) {
                    m_cbxPlatform->SetValue(platform);
                }
            }
        }
    }
}

void CBuildOptionsDialog::OnSourceDirChange(wxCommandEvent& WXUNUSED(event))
{
    PopulatePlatform();
}
