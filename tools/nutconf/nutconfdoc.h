#ifndef NUTCONFDOC_H_
#define NUTCONFDOC_H_

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
 * Revision 1.4  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wx.h>
#include <wx/docview.h>

#include "nutconfview.h"
#include "configitem.h"

#include "nutcomponent.h"

/*!
 * \brief Configuration document class.
 */
class CNutConfDoc:public wxDocument {
    DECLARE_DYNAMIC_CLASS(CNutConfDoc)
  public:
    CNutConfDoc();
    ~CNutConfDoc();

    virtual bool OnCreate(const wxString & path, long flags);
    virtual bool OnOpenDocument(const wxString & filename);
    virtual bool CNutConfDoc::OnSaveDocument(const wxString& filename);
    virtual bool OnNewDocument();
    virtual bool OnCloseDocument();

    bool ReadRepository(const wxString & repositoryname, const wxString & configname);
    void ReleaseRepository();

    void DeleteItems();
    void AddAllItems();
    void AddChildItems(NUTCOMPONENT * compo, wxTreeItemId parent);

    wxList & GetItems();
    CConfigItem *GetItem(size_t i);

    bool SetValue(CConfigItem & ti, long nValue);
    bool SetValue(CConfigItem & ti, const wxString & strValue);
    bool SetActive(CConfigItem & ti, bool bEnabled);

    wxString GetBuildTree();
    bool GenerateBuildTree();
    bool GenerateApplicationTree();

    wxString GetInstallDir();

    NUTCOMPONENTOPTION *FindOptionByName(NUTCOMPONENT * compo, char *name);
    bool IsOptionActive(char *name);
    void DeactivateOptionList(char **exlist);

  protected:
    NUTCOMPONENT *m_root;
    wxList m_items;
    bool IsRequirementProvided(NUTCOMPONENT *compo, char *requirement);
    bool IsRequirementProvided(char *requirement);
    void SaveComponentOptions(FILE *fp, NUTCOMPONENT * compo);
};

#endif
