#ifndef SETTINGS_H_
#define SETTINGS_H_

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
 * Revision 1.3  2004/09/17 13:03:48  haraldkipp
 * New settings page for tool options
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/wx.h>
#include <wx/config.h>

class CSettings: public wxObject
{
DECLARE_DYNAMIC_CLASS(CSettings)
public:
    CSettings();
    ~CSettings();

    bool Save();

    wxString m_configname;
    wxString m_configname_default;
    wxString m_repositoryname;
    wxString m_repositoryname_default;
    wxString m_firstidir;
    wxString m_firstidir_default;
    wxString m_lastidir;
    wxString m_lastidir_default;
    wxString m_buildpath;
    wxString m_buildpath_default;
    wxString m_lib_dir;
    wxString m_lib_dir_default;
    wxString m_source_dir;
    wxString m_source_dir_default;
    wxString m_platform;
    wxString m_platform_default;
    wxString m_toolpath;
    wxString m_toolpath_default;
    wxString m_app_dir;
    wxString m_app_dir_default;
    wxString m_programmer;
    wxString m_programmer_default;
};

#endif
