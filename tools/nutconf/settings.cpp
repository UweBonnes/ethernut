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
    m_config = new wxConfig(wxGetApp().GetAppName());

    m_config->Read("ConfigName", &m_configname, "nut/conf/ethernut2.conf");
    m_config->Read("RepositoryName", &m_repositoryname, "nut/conf/repository.nut");
    m_config->Read("BuildPath", &m_buildpath, "nutbld");
    m_config->Read("SourceDirectory", &m_source_dir, "c:/ethernut/nutconf/nut");
    m_config->Read("TargetPlatform", &m_platform, "avr-gcc");
    m_config->Read("ToolPath", &m_toolpath, "c:/iccavr/bin;tools/win32");
}

/*!
 * \brief Default destructor.
 */
CSettings::~CSettings()
{
    delete m_config;
}

bool CSettings::Save()
{
    m_config->Write("ConfigName", m_configname);
    m_config->Write("RepositoryName", m_repositoryname);
    m_config->Write("BuildPath", m_buildpath);
    m_config->Write("SourceDirectory", m_source_dir);
    m_config->Write("TargetPlatform", m_platform);
    m_config->Write("ToolPath", m_toolpath);

    return true;
}

