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
 * $Log: nutconfdoc.cpp,v $
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */


#include "nutconf.h"
#include "treeitemdata.h"
#include "nutconfhint.h"
#include "nutconfdoc.h"

IMPLEMENT_DYNAMIC_CLASS(CNutConfDoc, wxDocument);

/*!
 * \brief Default constructor.
 */
CNutConfDoc::CNutConfDoc()
{
    m_bRepositoryOpen = FALSE;
}

/*!
 * \brief Destructor.
 */
CNutConfDoc::~CNutConfDoc()
{
    CloseRepository();
    DeleteItems();
}

/*!
 * \brief Create a document from a specified file.
 */
bool CNutConfDoc::OnCreate(const wxString & path, long flags)
{
    wxGetApp().m_currentDoc = this;

    if (flags & wxDOC_NEW) {
        m_bRepositoryOpen = FALSE;

        if (!OpenRepository(wxEmptyString)) {
            wxGetApp().m_currentDoc = NULL;
            return FALSE;
        }

        Modify(FALSE);
        SetDocumentSaved(FALSE);

        wxString rootName(wxT("untitled"));
        wxStripExtension(rootName);
        SetFilename("default.nut");
    }
    // Creates the view, so do any view updating after this
    bool success = wxDocument::OnCreate(path, flags);

    if (success) {
        if (flags & wxDOC_NEW) {
            wxBusyCursor wait;

            CNutConfHint hint(NULL, nutSelChanged);
            UpdateAllViews(NULL, &hint);

            SetFilename(GetFilename(), true);
        }
    }
    return success;
}

/*!
 * \brief Create an empty document.
 *
 * \todo Doesn't do anything right now.
 */
bool CNutConfDoc::OnNewDocument()
{
    return true;
}

/*!
 * \brief Open an existing document.
 *
 * \todo Doesn't do anything right now.
 */
bool CNutConfDoc::OnOpenDocument(const wxString & filename)
{
    return true;
}

/*!
 * \brief Close the document.
 */
bool CNutConfDoc::OnCloseDocument()
{
    if (wxDocument::OnCloseDocument()) {
        DeleteItems();
        return TRUE;
    }
    return FALSE;
}


void CNutConfDoc::CloseRepository()
{
    if (m_bRepositoryOpen) {
        m_bRepositoryOpen = FALSE;
    }
}

/*
 * Delete all items.
 */
void CNutConfDoc::DeleteItems()
{
    // Delete any remaining items
    wxNode *node = m_items.First();
    while (node) {
        wxNode *next = node->Next();

        // Note: automatically removes itself from this list in ~ecConfigItem
        //delete item;
        node = next;
    }
}

bool CNutConfDoc::OpenRepository(const wxString & pszRepository)
{
    if (!m_bRepositoryOpen) {
        wxString strNewRepository;
        while (!m_bRepositoryOpen) {
            // Use what came in as parameter or what was found in registry
            if (!pszRepository.IsEmpty())
                strNewRepository = pszRepository;
            else
                strNewRepository = "nut/conf/repository.nut";

            wxString str;
            if (strNewRepository.IsEmpty())
                str.Printf(wxT("Opening repository..."));
            else
                str.Printf(wxT("Opening repository %s..."), (const wxChar *) strNewRepository);
            wxGetApp().SetStatusText(str);

            wxString strNewPackagesDir;

            wxBusyCursor wait;

            m_root = LoadRepository(strNewRepository);


            wxDocument::OnNewDocument();
            AddAllItems();
            m_bRepositoryOpen = TRUE;
        }

    }
    return m_bRepositoryOpen;
}


void CNutConfDoc::AddChildItems(NUTCOMPONENT * compo, wxTreeItemId parent)
{
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    compo = compo->nc_child;
    while (compo) {
        CConfigItem *item;
        item = new CConfigItem(item, compo);
        wxTreeItemId childId = treeCtrl->AppendItem(parent, wxT(""), -1, -1, new CTreeItemData(item));
        item->SetTreeItem(childId);
        item->UpdateTreeItem(*treeCtrl);
        m_items.Append(item);


        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            item = new CConfigItem(item, opts);
            wxTreeItemId optId = treeCtrl->AppendItem(childId, wxT(""), -1, -1, new CTreeItemData(item));
            item->SetTreeItem(optId);
            item->UpdateTreeItem(*treeCtrl);
            m_items.Append(item);
            opts = opts->nco_nxt;
        }

        AddChildItems(compo, childId);
        compo = compo->nc_nxt;
    }
}

/*!
 * \brief Create all configuration items.
 */
void CNutConfDoc::AddAllItems()
{
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(NULL);

    treeCtrl->DeleteAllItems();

    CConfigItem *item = new CConfigItem();
    wxTreeItemId rootId = treeCtrl->AddRoot(wxT(""), -1, -1, new CTreeItemData(item));

    item->SetTreeItem(rootId);
    item->UpdateTreeItem(*treeCtrl);
    m_items.Append(item);

    AddChildItems(m_root, rootId);

    UpdateAllViews();

    if (GetItems().Number() > 0) {
        wxGetApp().GetMainFrame()->GetTreeCtrl()->Expand(rootId);
    }
    wxGetApp().GetMainFrame()->GetTreeCtrl()->SetFocus();
}

/*!
 * \brief Return the list of items.
 */
wxList & CNutConfDoc::GetItems()
{
    return m_items;
}

/*!
 * \brief Return a specified item.
 */
CConfigItem *CNutConfDoc::GetItem(size_t i)
{
    return (CConfigItem *) m_items[i];
}


/*!
 * \brief Find NUTCOMPONENTOPTION by name.
 */
NUTCOMPONENTOPTION *CNutConfDoc::FindOption(NUTCOMPONENT * compo, char *name)
{

    while (compo) {
        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            if (strcmp(opts->nco_name, name) == 0) {
                return opts;
            }
            opts = opts->nco_nxt;
        }
        if ((opts = FindOption(compo->nc_child, name)) != NULL) {
            return opts;
        }
        compo = compo->nc_nxt;
    }
    return NULL;
}

/*!
 * \brief Return the activation state of an item with a specified name.
 */
bool CNutConfDoc::IsOptionActive(char *name)
{
    NUTCOMPONENTOPTION *opt = FindOption(m_root->nc_child, name);

    if (opt && opt->nco_value && atoi(opt->nco_value)) {
        return true;
    }
    return false;
}

/*!
 * \brief Set value of an integer item.
 */
bool CNutConfDoc::SetValue(CConfigItem & item, long nValue)
{
    if (item.m_option) {
        if (item.m_option->nco_value) {
            free(item.m_option->nco_value);
            item.m_option->nco_value = NULL;
        }
        wxString str;
        str.Printf("%ld", nValue);
        item.m_option->nco_value = strdup(str);
    }
    return true;
}

/*!
 * \brief Set value of a string item.
 */
bool CNutConfDoc::SetValue(CConfigItem & item, const wxString & strValue)
{
    if (item.m_option) {
        if (item.m_option->nco_value) {
            free(item.m_option->nco_value);
        }
        item.m_option->nco_value = strdup(strValue);

        CNutConfHint hint(&item, nutValueChanged);
        UpdateAllViews(NULL, &hint);
    }
    return true;
}

/*!
 * \brief Set value of a boolean item.
 */
bool CNutConfDoc::SetEnabled(CConfigItem & item, bool bEnabled)
{
    item.SetEnabled(bEnabled);

    Modify(TRUE);
    CNutConfHint hint(&item, nutValueChanged);
    UpdateAllViews(NULL, &hint);

    return true;
}

/*!
 * \brief Return build tree path.
 */
wxString CNutConfDoc::GetBuildTree()
{
    return wxString(wxT("nutbld"));
}

/*!
 * \brief Create all header amd make files.
 */
bool CNutConfDoc::GenerateBuildTree()
{
    wxBusyCursor wait;

    CreateMakeFiles(m_root);
    CreateHeaderFiles(m_root);

    return true;
}
