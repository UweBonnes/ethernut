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
 * $Log: settingsdlg.cpp,v $
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/notebook.h>

#include "ids.h"
#include "nutconf.h"
#include "settingsdlg.h"

#ifdef __WXMSW__
#define PROPERTY_DIALOG_WIDTH   380
#define PROPERTY_DIALOG_HEIGHT  420
#else
#define PROPERTY_DIALOG_WIDTH   550 // 460
#define PROPERTY_DIALOG_HEIGHT  480
#endif

IMPLEMENT_CLASS(CSettingsDialog, wxDialog)


CSettingsDialog::CSettingsDialog(wxWindow* parent)
: wxDialog(parent, -1, wxGetApp().GetAppName() + wxT(" Settings"), wxPoint(0, 0), wxSize(PROPERTY_DIALOG_WIDTH, PROPERTY_DIALOG_HEIGHT))
{
    m_notebook = new wxNotebook(this, ID_SETTINGS_NOTEBOOK, wxPoint(2, 2), wxSize(PROPERTY_DIALOG_WIDTH - 4, PROPERTY_DIALOG_HEIGHT - 4));

    m_repositoryOptions = new CRepositoryOptionsDialog(m_notebook);
    m_buildOptions = new CBuildOptionsDialog(m_notebook);
    //m_toolOptions = new CToolOptionsDialog(m_notebook);

    m_notebook->AddPage(m_repositoryOptions, wxT("Repository"));
    m_notebook->AddPage(m_buildOptions, wxT("Build"));
    //m_notebook->AddPage(m_toolOptions, wxT("Tools"));

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

    sizerTop->Add(m_notebook, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT | wxTOP, 5);

    wxSizer *sizerBot = new wxBoxSizer(wxHORIZONTAL);

    wxButton *okButton = new wxButton(this, wxID_OK, "&OK", wxDefaultPosition, wxDefaultSize, 0);
    wxButton *cancelButton = new wxButton( this, wxID_CANCEL, "&Cancel", wxDefaultPosition, wxDefaultSize, 0);
    sizerBot->Add(okButton, 0, wxALIGN_CENTRE | wxALL, 5);
    sizerBot->Add( cancelButton, 0, wxALIGN_CENTRE | wxALL, 5);

    sizerTop->Add(4, 4, 0, wxALIGN_CENTRE|wxALL, 0);
    sizerTop->Add(sizerBot, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL | wxALL, 0);

    this->SetAutoLayout(true);
    this->SetSizer(sizerTop);

    okButton->SetDefault();
    okButton->SetFocus();

    Layout();

    m_repositoryOptions->Layout();

    Centre(wxBOTH);
}

bool CSettingsDialog::TransferDataToWindow()
{
    m_repositoryOptions->TransferDataToWindow();
    m_buildOptions->TransferDataToWindow();
    //m_toolOptions->TransferDataToWindow();

    return true;
}

bool CSettingsDialog::TransferDataFromWindow()
{
    m_repositoryOptions->TransferDataFromWindow();
    m_buildOptions->TransferDataFromWindow();
    //m_toolOptions->TransferDataFromWindow();

    wxGetApp().GetSettings()->Save();

    return true;
}
