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
 * $Log: repoptdlg.cpp,v $
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/valgen.h>

#include "ids.h"
#include "nutconf.h"
#include "repoptdlg.h"

IMPLEMENT_CLASS(CRepositoryOptionsDialog, wxPanel)

CRepositoryOptionsDialog::CRepositoryOptionsDialog(wxWindow* parent)
: wxPanel(parent, ID_SETTINGS_REPOSITORY)
{
    CSettings *opts = wxGetApp().GetSettings();

    wxStaticBox *groupPath = new wxStaticBox(this, -1, wxT("Repository File"));
    wxTextCtrl *entryPath = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&opts->m_repositoryname));
    wxButton *btnBrowse = new wxButton(this, ID_BROWSE_BUTTON, wxT("&Browse..."), wxDefaultPosition, wxDefaultSize, 0);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    wxSizer *sizerGroup = new wxStaticBoxSizer(groupPath, wxHORIZONTAL);

    sizerGroup->Add(entryPath, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    sizerGroup->Add(btnBrowse, 0, wxALIGN_RIGHT | wxALL, 5);
    sizerTop->Add(sizerGroup, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);

    SetAutoLayout(true);
    SetSizer(sizerTop);
}

bool CRepositoryOptionsDialog::TransferDataToWindow()
{
    wxPanel::TransferDataToWindow();

    return true;
}

bool CRepositoryOptionsDialog::TransferDataFromWindow()
{
    wxPanel::TransferDataFromWindow();

    return true;
}
