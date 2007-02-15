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
 * Revision 1.10  2007/02/15 19:33:45  haraldkipp
 * Version 1.4.1 works with wxWidgets 2.8.0.
 * Several wide character issues fixed.
 *
 * Revision 1.9  2006/10/21 12:48:18  christianwelzel
 * Added support for multiple configurations / settings
 *
 * Revision 1.8  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.7  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.6  2005/07/20 09:23:15  haraldkipp
 * Prepend current directory to the default file paths.
 *
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

#include <wx/filename.h>

#include "nutconf.h"
#include "settings.h"

IMPLEMENT_DYNAMIC_CLASS(CSettings, wxObject)

wxString CSettings::FindAbsoluteDir(wxString refPathName)
{
    int i = 4;
    size_t len = refPathName.Len();
    wxString sep(wxFileName::GetPathSeparator());

#ifdef __WXMSW__
    refPathName.Replace(wxT("/"), wxT("\\"));
#endif
    if (!wxFileName::FileExists(refPathName)) {
        while (i--) {
            if (wxFileName::DirExists(wxT("nut") + sep)) {
                refPathName.Prepend(wxT("nut") + sep);
            }
            else {
                refPathName.Prepend(wxT("..") + sep);
            }
            if (wxFileName::FileExists(refPathName)) {
                break;
            }
        }
        if (i < 0) {
            return wxString(wxT("."));
        }
    }

    wxFileName fn(refPathName);
    fn.MakeAbsolute();
    refPathName = fn.GetFullPath();
    refPathName.Truncate(refPathName.Len() - len);
#ifdef __WXMSW__
    refPathName.Replace(wxT("\\"), wxT("/"));
#endif
    if (refPathName.Last() == '/') {
        refPathName.RemoveLast();
    }

    return refPathName;
}

wxString CSettings::FindRelativeDir(wxString refPathName)
{

    refPathName = FindAbsoluteDir(refPathName);

#ifdef __WXMSW__
    refPathName.Replace(wxT("/"), wxT("\\"));
#endif
    wxFileName fn(refPathName + wxT("\\dummy"));
    fn.MakeRelativeTo(::wxGetCwd());
    refPathName = fn.GetPath();
    if (refPathName.IsEmpty()) {
        return wxString(wxT("."));
    }
#ifdef __WXMSW__
    refPathName.Replace(wxT("\\"), wxT("/"));
#endif

    return refPathName;
}

/*!
 * \brief Default constructor.
 */
CSettings::CSettings()
{
    Load(wxT(""));
}

/*!
 * \brief Default destructor.
 */
CSettings::~CSettings()
{
}

bool CSettings::Load(wxString ConfigFileName)
{
    /* Get source path, which is relative to our working directory. */
    m_relsrcpath = FindRelativeDir(wxT("os/version.c"));
    
    /* Get absolute tools path. */
    wxFileName fn(m_relsrcpath + wxT("/dummy"));
    fn.AppendDir(wxT("tools"));
#ifdef _WIN32
    fn.AppendDir(wxT("win32"));
#else
    fn.AppendDir(wxT("linux"));
#endif
    fn.MakeAbsolute();
    wxString toolPath = fn.GetPath();

    /* Get source path component. */
    m_source_dir_default = fn.GetFullPath(wxPATH_UNIX);
    if (!m_source_dir_default.Contains(wxT("/nut/"))) {
        m_source_dir_default = m_source_dir_default.BeforeLast('/');
        m_source_dir_default = m_source_dir_default.BeforeLast('/');
        m_source_dir_default = m_source_dir_default.BeforeLast('/');
        m_source_dir_default = m_source_dir_default.AfterLast('/');
        m_buildpath_default = m_source_dir_default + wxT("-bld");
        m_app_dir_default = m_source_dir_default + wxT("-app");
    }
    else {
        m_source_dir_default = wxString(wxT("nut"));
        m_buildpath_default = wxString(wxT("nutbld"));
        m_app_dir_default = wxString(wxT("nutapp"));
    }
    m_lib_dir_default = m_buildpath_default + wxT("/lib");

    wxFileName fname(ConfigFileName);
    wxString ConfigName = fname.GetFullName();
    if (ConfigName == wxEmptyString) {
        m_configname_default = m_relsrcpath + wxT("/conf/ethernut21.conf");
    }
    else {
        m_configname_default = m_relsrcpath + wxT("/conf/") + ConfigName;
    }
    
    m_repositoryname_default = m_relsrcpath + wxT("/conf/repository.nut");
    m_mulConfig_default = false;

    m_firstidir_default = wxEmptyString;
    m_lastidir_default = wxEmptyString;
    m_platform_default = wxT("(Select)");
    m_programmer_default = wxT("avr-uisp-stk500");
    m_toolpath_default = toolPath + wxT(";\"(Add compiler paths here)\";");

    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();
        
        pConfig->SetPath(wxT("/Settings"));
        pConfig->Read(wxT("ConfigName"), &m_configname, m_configname_default);
        if (ConfigFileName != wxEmptyString) {
            m_configname = ConfigFileName;
        }
        pConfig->Read(wxT("MulConfigurations"), &m_mulConfig, m_mulConfig_default);
        
        if (m_mulConfig == true) {
            pConfig->SetPath(lastPath);
            pConfig->SetPath(wxT("/Settings/") + ConfigName);
        }
        
        pConfig->Read(wxT("RepositoryName"), &m_repositoryname, m_repositoryname_default);

        pConfig->Read(wxT("BuildPath"), &m_buildpath, m_buildpath_default);
        pConfig->Read(wxT("FirstInclude"), &m_firstidir, m_firstidir_default);
        pConfig->Read(wxT("LastInclude"), &m_lastidir, m_lastidir_default);
        pConfig->Read(wxT("InstallPath"), &m_lib_dir, m_lib_dir_default);
        pConfig->Read(wxT("SourceDirectory"), &m_source_dir, m_source_dir_default);
        pConfig->Read(wxT("TargetPlatform"), &m_platform, m_platform_default);
        pConfig->Read(wxT("ToolPath"), &m_toolpath, m_toolpath_default);
        pConfig->Read(wxT("ApplicationDirectory"), &m_app_dir, m_app_dir_default);
        pConfig->Read(wxT("Programmer"), &m_programmer, m_programmer_default);

        pConfig->SetPath(lastPath);
    }
    return true;
}

bool CSettings::Save(wxString ConfigFileName)
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();

        pConfig->SetPath(wxT("/Settings"));
        pConfig->Write(wxT("ConfigName"), ConfigFileName);
        pConfig->Write(wxT("MulConfigurations"), m_mulConfig);
        
        if (m_mulConfig == true) {
            wxFileName fname(ConfigFileName);
            wxString ConfigName = fname.GetFullName();
            pConfig->SetPath(lastPath);
            pConfig->SetPath(wxT("/Settings/") + ConfigName);
        }
        
        pConfig->Write(wxT("RepositoryName"), m_repositoryname);

        pConfig->Write(wxT("BuildPath"), m_buildpath);
        pConfig->Write(wxT("FirstInclude"), m_firstidir);
        pConfig->Write(wxT("LastInclude"), m_lastidir);
        pConfig->Write(wxT("InstallPath"), m_lib_dir);
        pConfig->Write(wxT("SourceDirectory"), m_source_dir);
        pConfig->Write(wxT("TargetPlatform"), m_platform);
        pConfig->Write(wxT("ToolPath"), m_toolpath);
        pConfig->Write(wxT("ApplicationDirectory"), m_app_dir);
        pConfig->Write(wxT("Programmer"), m_programmer);
        pConfig->SetPath(lastPath);
    }
    return true;
}

