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
 * $Log: settings.cpp,v $
 * Revision 1.5  2004/09/17 13:03:48  haraldkipp
 * New settings page for tool options
 *
 * Revision 1.4  2004/09/07 19:20:07  haraldkipp
 * Initial/default lib/inc dirs updated
 *
 * Revision 1.3  2004/08/18 17:22:28  haraldkipp
 * STK500 is the default programmer
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include "nutconf.h"
#include "settings.h"

IMPLEMENT_DYNAMIC_CLASS(CSettings, wxObject)

/*!
 * \brief Default constructor.
 */
CSettings::CSettings()
{
    /* We may later have a more versatile way to locate these. */
    m_configname_default = wxT("nut/conf/ethernut21.conf");
    m_repositoryname_default = wxT("nut/conf/repository.nut");
    m_firstidir_default = wxEmptyString;
    m_lastidir_default = wxEmptyString;
    m_buildpath_default = wxT("nut/conf/repository.nut");
    m_lib_dir_default = wxT("");
    m_source_dir_default = wxT("nut");
    m_platform_default = wxT("avr-gcc");
    m_app_dir_default = wxT("nutapp");
    m_programmer_default = wxT("avr-uisp-stk500");
    m_toolpath_default = wxT("");

    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();
        pConfig->SetPath(wxT("/Settings"));

        pConfig->Read("ConfigName", &m_configname, m_configname_default);
        pConfig->Read("RepositoryName", &m_repositoryname, m_repositoryname_default);
        pConfig->Read("BuildPath", &m_buildpath, m_buildpath_default);
        pConfig->Read("FirstInclude", &m_firstidir, m_firstidir_default);
        pConfig->Read("LastInclude", &m_lastidir, m_lastidir_default);
        pConfig->Read("InstallPath", &m_lib_dir, m_lib_dir_default);
        pConfig->Read("SourceDirectory", &m_source_dir, m_source_dir_default);
        pConfig->Read("TargetPlatform", &m_platform, m_platform_default);
        pConfig->Read("ToolPath", &m_toolpath, m_toolpath_default);
        pConfig->Read("ApplicationDirectory", &m_app_dir, m_app_dir_default);
        pConfig->Read("Programmer", &m_programmer, m_programmer_default);

        pConfig->SetPath(lastPath);
    }
}

/*!
 * \brief Default destructor.
 */
CSettings::~CSettings()
{
}

bool CSettings::Save()
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();
        pConfig->SetPath(wxT("/Settings"));
        pConfig->Write("ConfigName", m_configname);
        pConfig->Write("RepositoryName", m_repositoryname);
        pConfig->Write("BuildPath", m_buildpath);
        pConfig->Write("FirstInclude", m_firstidir);
        pConfig->Write("LastInclude", m_lastidir);
        pConfig->Write("InstallPath", m_lib_dir);
        pConfig->Write("SourceDirectory", m_source_dir);
        pConfig->Write("TargetPlatform", m_platform);
        pConfig->Write("ToolPath", m_toolpath);
        pConfig->Write("ApplicationDirectory", m_app_dir);
        pConfig->Write("Programmer", m_programmer);
        pConfig->SetPath(lastPath);
    }
    return true;
}

