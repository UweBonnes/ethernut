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
 * $Log: bldoptdlg.cpp,v $
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/valgen.h>
#include <wx/dir.h>

#include "ids.h"
#include "nutconf.h"
#include "bldoptdlg.h"

IMPLEMENT_CLASS(CBuildOptionsDialog, wxPanel)

BEGIN_EVENT_TABLE(CBuildOptionsDialog, wxPanel)
    EVT_BUTTON(ID_BROWSE_BUTTON, CBuildOptionsDialog::OnBrowseBuildPath)
    EVT_BUTTON(ID_BUTTON_SRCDIR, CBuildOptionsDialog::OnButtonSourceDir)
    EVT_TEXT_ENTER(ID_COMBO_SRCDIR, OnPlatformEnter)
    EVT_TEXT(ID_ENTRY_SRCDIR, OnSourceDirChange) 
END_EVENT_TABLE()

CBuildOptionsDialog::CBuildOptionsDialog(wxWindow* parent)
: wxPanel(parent, ID_SETTINGS_BUILD)
{
    CSettings *opts = wxGetApp().GetSettings();

    wxStaticBox *grpSource = new wxStaticBox(this, -1, wxT("Source Directory"));
    m_entSourceDir = new wxTextCtrl(this, ID_ENTRY_SRCDIR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&opts->m_source_dir));
    wxButton *btnSourceDir = new wxButton(this, ID_BUTTON_SRCDIR, wxT("&Browse..."), wxDefaultPosition, wxDefaultSize, 0);
    wxStaticText *lblPlatform = new wxStaticText(this, -1, wxT("Platform"));
    m_cbxPlatform = new wxComboBox(this, ID_COMBO_SRCDIR, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxGenericValidator(&opts->m_platform));

    wxStaticBox *groupPath = new wxStaticBox(this, -1, wxT("Build Directory"));
    wxTextCtrl *entryPath = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&opts->m_buildpath));
    wxButton *btnBrowse = new wxButton(this, ID_BROWSE_BUTTON, wxT("&Browse..."), wxDefaultPosition, wxDefaultSize, 0);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    wxSizer *szrSource = new wxStaticBoxSizer(grpSource, wxVERTICAL);
    wxSizer *szrSourceDir = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *szrPlatform = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *sizerPath = new wxStaticBoxSizer(groupPath, wxHORIZONTAL);

    szrSourceDir->Add(m_entSourceDir, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrSourceDir->Add(btnSourceDir, 0, wxALIGN_RIGHT | wxGROW | wxALL, 5);

    szrPlatform->Add(lblPlatform, 0, wxALIGN_LEFT | wxGROW | wxALL, 5);
    szrPlatform->Add(m_cbxPlatform, 0, wxALIGN_LEFT | wxGROW | wxALL, 5);

    szrSource->Add(szrSourceDir, 0, wxGROW | wxALL, 5);
    szrSource->Add(szrPlatform, 0, wxGROW | wxALL, 5);

    sizerPath->Add(entryPath, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    sizerPath->Add(btnBrowse, 0, wxALIGN_RIGHT | wxALL, 5);
    sizerTop->Add(szrSource, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);
    sizerTop->Add(sizerPath, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);

    SetAutoLayout(true);
    SetSizer(sizerTop);

    PopulatePlatform();
}

bool CBuildOptionsDialog::TransferDataToWindow()
{
    wxPanel::TransferDataToWindow();

    return true;
}

bool CBuildOptionsDialog::TransferDataFromWindow()
{
    wxPanel::TransferDataFromWindow();

    return true;
}

void CBuildOptionsDialog::OnBrowseBuildPath(wxCommandEvent& event)
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

void CBuildOptionsDialog::OnButtonSourceDir(wxCommandEvent& event)
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

void CBuildOptionsDialog::OnPlatformEnter(wxCommandEvent& event)
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
                bool cont = dir.GetFirst(&entry, "Makedefs.*");
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

void CBuildOptionsDialog::OnSourceDirChange(wxCommandEvent& event)
{
    PopulatePlatform();
}
