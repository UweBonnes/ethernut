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
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */


#include "nutconf.h"
#include "treeitemdata.h"
#include "nutconfhint.h"
#include "nutconfdoc.h"

/*
 * The doc/view framework will create instances of this class dynamically.
 */
IMPLEMENT_DYNAMIC_CLASS(CNutConfDoc, wxDocument);

/*!
 * \brief Default constructor.
 */
CNutConfDoc::CNutConfDoc()
{
}

/*!
 * \brief Destructor.
 */
CNutConfDoc::~CNutConfDoc()
{
    ReleaseRepository();
    DeleteItems();
}

/*!
 * \brief Create a document from a specified file.
 *
 * \param path Pathname of the configuration file.
 */
bool CNutConfDoc::OnCreate(const wxString & path, long flags)
{
    bool rc;

    wxGetApp().m_currentDoc = this;

    if ((rc = ReadRepository(wxGetApp().GetSettings()->m_repositoryname, path)) == true) {
        Modify(false);
        SetDocumentSaved(false);

        rc = wxDocument::OnCreate(path, flags);
        if (rc) {
            if (flags & wxDOC_NEW) {
                wxBusyCursor wait;

                CNutConfHint hint(NULL, nutSelChanged);
                UpdateAllViews(NULL, &hint);

                SetFilename(GetFilename(), true);
            }
        }
    }
    if(!rc) {
        wxGetApp().m_currentDoc = NULL;
    }
    return rc;
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
 */
bool CNutConfDoc::OnOpenDocument(const wxString & filename)
{
    /*
     * Check if the current document has been modified.
     */
    if (!OnSaveModified())
        return false;

    wxString msgTitle;
    if (wxGetApp().GetAppName() != "")
        msgTitle = wxGetApp().GetAppName();
    else
        msgTitle = wxString("File error");

    SetFilename(filename, true);
    Modify(false);
    UpdateAllViews();

    return true;
}

void CNutConfDoc::SaveComponentOptions(FILE *fp, NUTCOMPONENT * compo)
{
    while (compo) {
        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            if(opts->nco_enabled && opts->nco_active) {
                if(opts->nco_value) {
                    fprintf(fp, "%s = \"%s\"\n", opts->nco_name, opts->nco_value);
                }
                else {
                    fprintf(fp, "%s = \"\"\n", opts->nco_name);
                }
            }
            opts = opts->nco_nxt;
        }
        SaveComponentOptions(fp, compo->nc_child);
        compo = compo->nc_nxt;
    }
}


bool CNutConfDoc::OnSaveDocument(const wxString& filename)
{
    //NUTCOMPONENT *compo;
    //NUTCOMPONENTOPTION *opt;

    if (filename == "") {
        return false;
    }

    FILE *fp = fopen(filename, "w");
    if (fp) {
        SaveComponentOptions(fp, m_root->nc_child);
        fclose(fp);
    }

    Modify(false);
    SetFilename(filename);

    return true;
}

/*!
 * \brief Close the document.
 */
bool CNutConfDoc::OnCloseDocument()
{
    if (wxDocument::OnCloseDocument()) {
        DeleteItems();
        return true;
    }
    return false;
}


void CNutConfDoc::ReleaseRepository()
{
}

/*
 * Delete all items.
 */
void CNutConfDoc::DeleteItems()
{
    wxNode *node = m_items.First();
    while (node) {
        wxNode *next = node->Next();
        node = next;
    }
}

bool CNutConfDoc::ReadRepository(const wxString & repositoryname, const wxString & configname)
{
    wxString str;
    str.Printf(wxT("Loading %s..."), (const wxChar *)configname);
    wxGetApp().SetStatusText(str);

    wxBusyCursor wait;
    NUTREPOSITORY *repo = OpenRepository(repositoryname);
    m_root = LoadComponents(repo);
    ConfigureComponents(repo, m_root, configname);
    RefreshComponents(m_root);
    CloseRepository(repo);


    wxDocument::OnNewDocument();
    AddAllItems();
    wxGetApp().SetStatusText(wxEmptyString);

    return true;
}


void CNutConfDoc::AddChildItems(NUTCOMPONENT * compo, wxTreeItemId parent)
{
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    compo = compo->nc_child;
    while (compo) {
        CConfigItem *item = new CConfigItem(NULL, compo);

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
NUTCOMPONENTOPTION *CNutConfDoc::FindOptionByName(NUTCOMPONENT * compo, char *name)
{
    while (compo) {
        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            if (strcmp(opts->nco_name, name) == 0) {
                return opts;
            }
            opts = opts->nco_nxt;
        }
        if ((opts = FindOptionByName(compo->nc_child, name)) != NULL) {
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
    NUTCOMPONENTOPTION *opt = FindOptionByName(m_root->nc_child, name);

    if (opt && opt->nco_active) {
        return true;
    }
    return false;
}

bool CNutConfDoc::IsRequirementProvided(NUTCOMPONENT *compo, char *requirement)
{
    int i;

    while (compo) {
        if(compo->nc_enabled) {
            for (i = 0; compo->nc_provides[i]; i++) {
                if(strcmp(compo->nc_provides[i], requirement) == 0) {
                    return true;
                }
            }

            NUTCOMPONENTOPTION *opts = compo->nc_opts;
            while (opts) {
                for (i = 0; opts->nco_provides[i]; i++) {
                    if(strcmp(opts->nco_provides[i], requirement) == 0) {
                        if(opts->nco_active) {
                            return true;
                        }
                    }
                }
                opts = opts->nco_nxt;
            }
            if (IsRequirementProvided(compo->nc_child, requirement)) {
                return true;
            }
        }
        compo = compo->nc_nxt;
    }
    return false;
}

bool CNutConfDoc::IsRequirementProvided(char *requirement)
{
    return IsRequirementProvided(m_root, requirement);
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
bool CNutConfDoc::SetActive(CConfigItem & item, bool bEnabled)
{
    item.SetActive(bEnabled);

    RefreshComponents(m_root);
    Modify(true);
    CNutConfHint hint(&item, nutExternallyChanged);
    UpdateAllViews(NULL, &hint);

    return true;
}

/*!
 * \brief Return build tree path.
 */
wxString CNutConfDoc::GetBuildTree()
{
    return wxGetApp().GetSettings()->m_buildpath;
}

/*!
 * \brief Create all header amd make files.
 */
bool CNutConfDoc::GenerateBuildTree()
{
    CSettings *cfg = wxGetApp().GetSettings();
    
    wxBusyCursor wait;
    CreateMakeFiles(m_root, cfg->m_buildpath.c_str(), cfg->m_source_dir.c_str(), cfg->m_platform.c_str());
    CreateHeaderFiles(m_root, cfg->m_buildpath.c_str());

    return true;
}
