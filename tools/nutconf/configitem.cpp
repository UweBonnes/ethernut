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
 * $Log: configitem.cpp,v $
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:24  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */


#include "ids.h"
#include "utils.h"
#include "nutconf.h"
#include "configtree.h"
#include "configitem.h"
#include "enumeditctrl.h"
#include "inteditctrl.h"
#include "texteditctrl.h"

#ifdef __WXMSW__
#define strcasecmp stricmp
#endif

IMPLEMENT_CLASS(CConfigItem, wxObject);

/*!
 * \brief Default constructor.
 */
CConfigItem::CConfigItem()
{
    m_compo = NULL;
    m_option = NULL;
    m_name = wxEmptyString;
    m_configType = nutConfigTypeNone;
    m_optionType = nutOptionTypeNone;
    m_parent = NULL;
    m_hint = nutHintNone;
    m_itemId = wxTreeItemId();
}

/*!
 * \brief Creates a CConfigItem from a given NUTCOMPONENT.
 */
CConfigItem::CConfigItem(CConfigItem * parent, NUTCOMPONENT * compo)
{
    m_compo = compo;
    m_option = NULL;
    m_name = compo->nc_name;
    if (compo->nc_child) {
        m_configType = nutLibrary;
    } else {
        m_configType = nutModule;
    }
    m_optionType = nutString;
    m_parent = parent;
    m_hint = nutHintNone;
    m_itemId = wxTreeItemId();

    switch (m_optionType) {
    case nutString:
    case nutEnumerated:
        m_value = wxEmptyString;
        break;
    case nutInteger:
        m_value = (long) 0;
        break;
    case nutBool:
        m_value = false;
        break;
    }
}

/*!
 * \brief Creates a CConfigItem from a given NUTCOMPONENTOPTION.
 */
CConfigItem::CConfigItem(CConfigItem * parent, NUTCOMPONENTOPTION * option)
{
    m_compo = NULL;
    m_option = option;
    m_name = option->nco_name;
    m_configType = nutOption;
    m_optionType = nutString;
    m_parent = parent;
    m_hint = nutHintNone;
    m_itemId = wxTreeItemId();

    switch (m_optionType) {
    case nutString:
    case nutEnumerated:
        m_value = wxEmptyString;
        break;
    case nutInteger:
        m_value = (long) 0;
        break;
    case nutBool:
        m_value = false;
        break;
    }
}

wxTreeItemId CConfigItem::GetTreeItem() const
{
    return m_itemId;
}

void CConfigItem::SetTreeItem(wxTreeItemId id)
{
    m_itemId = id;
}

nutComponentType CConfigItem::GetConfigType() const
{
    return m_configType;
}

void CConfigItem::SetUIHint(nutUIHint hint)
{
    m_hint = hint;
}

nutUIHint CConfigItem::GetUIHint() const
{
    return m_hint;
}

void CConfigItem::SetValue(const wxVariant & value)
{
    m_value = value;
}

wxVariant & CConfigItem::GetValue()
{
    return m_value;
}

/*!
 * \brief Refresh a CConfigItem entry in a specified CConfigTree.
 */
bool CConfigItem::UpdateTreeItem(CConfigTree & treeCtrl)
{
    treeCtrl.SetItemText(m_itemId, GetBriefDescription());

    static wxColour normalColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_WINDOWTEXT);
    static wxColour disabledColour = wxSystemSettings::GetSystemColour(wxSYS_COLOUR_GRAYTEXT);

    treeCtrl.SetItemTextColour(m_itemId, normalColour);

    int iconState = 0;
    wxString iconName;

    switch (GetConfigType()) {
    case nutFolder:
        iconName = wxT("Folder");
        break;
    case nutLibrary:
        iconName = wxT("Library");
        break;
    case nutModule:
        iconName = wxT("Module");
        break;
    case nutOption:
        if (GetOptionFlavor() == nutFlavorData) {
            switch (GetOptionType()) {
            case nutInteger:
                iconName = wxT("Integer");
                break;
            case nutEnumerated:
                iconName = wxT("Enumerated");
                break;
            case nutString:
                iconName = wxT("Text");
                break;
            case nutBool:
                if (GetUIHint() == nutHintRadio)
                    iconName = wxT("Radiobox");
                else
                    iconName = wxT("Checkbox");
                iconState = (m_value.GetBool()? 0 : 1);
                break;
            }
        }
        if (GetOptionFlavor() == nutFlavorBoolData || GetOptionFlavor() == nutFlavorBool) {
            if (GetUIHint() == nutHintRadio)
                iconName = wxT("Radiobox");
            else
                iconName = wxT("Checkbox");
            iconState = (IsActive()? 0 : 1);
        }
        break;
    }

    if (!iconName.IsEmpty()) {
        int iconId = treeCtrl.GetIconDB().GetIconId(iconName, iconState, IsEnabled());
        treeCtrl.SetItemImage(m_itemId, iconId, wxTreeItemIcon_Normal);
        treeCtrl.SetItemImage(m_itemId, iconId, wxTreeItemIcon_Selected);
    }
    return true;
}

/*!
 * \brief Create a control to edit our value.
 *
 * This routine is called when the user clicks on an editable
 * item in the value window.
 */
wxWindow *CConfigItem::CreateEditWindow(wxWindow * parent)
{
    wxWindow *window = NULL;

    switch (GetOptionType()) {
    case nutEnumerated:
        /* Create drop down box for enumerated values. */
        {
            window = new CEnumEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, 0);
            wxArrayString arEnumStrings;
            GetEnumStrings(arEnumStrings);
            for (unsigned int i = 0; i < arEnumStrings.GetCount(); i++) {
                ((CEnumEditCtrl *) window)->Append(arEnumStrings[i]);
            }
        }
        break;
    case nutInteger:
        /* Use a spin control for integer values. */
        window = new CIntEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);
        break;
    case nutString:
        /* Normal entry control for string values. */
        window = new CTextEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        break;
    }

    wxASSERT(window != NULL);

    return window;
}

/*!
 * \brief Process a left mouse button click on our icon.
 *
 * Boolean values will be inverted.
 *
 * \param treeCtrl The tree control that received the click.
 */
void CConfigItem::OnIconLeftDown(CConfigTree & treeCtrl)
{
    if (GetConfigType() != nutOption)
        return;

    switch (GetOptionFlavor()) {
    case nutFlavorBool:
    case nutFlavorBoolData:
        if (IsEnabled()) {
            wxGetApp().GetNutConfDoc()->SetActive(*this, !IsActive());
        }
    default:
        break;
    }
}

/*!
 * \brief Launch external file viewer.
 */
bool CConfigItem::ViewHeaderFile()
{
    bool rc = false;
    wxString strFile(GetFilename());

    if (!strFile.IsEmpty()) {
        CNutConfDoc *pDoc = wxGetApp().GetNutConfDoc();
        if (pDoc->GetBuildTree().IsEmpty()) {
            wxMessageBox(wxT("Cannot display header file until configuration is saved"));
        } else {
            strFile.Prepend(wxT("/"));
            strFile.Prepend(pDoc->GetBuildTree());
            //rc = wxGetApp().Launch(strFile, "notepad.exe");
            rc = wxGetApp().Launch(strFile, wxEmptyString);
        }
    }
    return rc;
}

/*! 
 * \brief Return the item's brief description.
 */
wxString CConfigItem::GetBriefDescription() const
{
    wxString str;

    if (m_compo) {
        if (m_compo->nc_brief) {
            str = m_compo->nc_brief;
        } else {
            str = m_compo->nc_name;
        }
    } else if (m_option) {
        if (m_option->nco_brief) {
            str = m_option->nco_brief;
        } else {
            str = m_option->nco_name;
        }
    } else {
        str = wxT("Nut/OS Components");
    }
    return str;
}

wxString CConfigItem::GetName() const
{
    wxString str;

    if (m_compo) {
        str = m_compo->nc_name;
    } else if (m_option) {
        str = m_option->nco_name;
    }
    return str;
}

wxString CConfigItem::GetDescription() const
{
    wxString str;

    if (m_compo) {
        str = m_compo->nc_description;
    } else if (m_option) {
        str = m_option->nco_description;
    }
    return str;
}

nutOptionFlavor CConfigItem::GetOptionFlavor() const
{
    if (m_option) {
        if (m_option->nco_flavor) {
            if (strcasecmp(m_option->nco_flavor, "boolean") == 0) {
                return nutFlavorBool;
            }
            if (strcasecmp(m_option->nco_flavor, "booldata") == 0) {
                return nutFlavorBoolData;
            }
        }
        return nutFlavorData;
    }
    return nutFlavorNone;
}

nutOptionType CConfigItem::GetOptionType() const
{
    if (m_option) {
        if (m_option->nco_type) {
            if (strcasecmp(m_option->nco_type, "integer") == 0) {
                return nutInteger;
            }
            if (strcasecmp(m_option->nco_type, "bool") == 0) {
                return nutBool;
            }
            if (strcasecmp(m_option->nco_type, "enumerated") == 0) {
                return nutEnumerated;
            }
        }
        return nutString;
    }
    return nutOptionTypeNone;
}

wxString CConfigItem::GetDisplayValue() const
{
    wxString str;

    switch (GetOptionType()) {
    case nutEnumerated:
    case nutInteger:
    case nutString:
        str = StringValue();
        break;
    }
    return str;
}

/*!
 * \brief Return configuration item as a string.
 */
wxString CConfigItem::StringValue() const
{
    if (m_option) {
        if (m_option->nco_value) {
            return wxString(m_option->nco_value);
        }
        if (GetOptionType() == nutEnumerated) {
            if (m_option->nco_choices && m_option->nco_choices[0]) {
                return wxString(m_option->nco_choices[0]);
            }
        }
    }
    return wxEmptyString;
}

wxString CConfigItem::GetFilename() const
{
    if (m_option &&  m_option->nco_file) {
        return wxString(m_option->nco_file);
    }
    return wxEmptyString;
}

wxString CConfigItem::GetMacro() const
{
    if (m_option && m_option->nco_name) {
        return wxString(m_option->nco_name);
    }
    return wxEmptyString;
}

wxString CConfigItem::GetRequirementList() const
{
    wxString str;

    if (m_option && m_option->nco_requires) {
        for (int i = 0; m_option->nco_requires[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += m_option->nco_requires[i];
        }
    }
    if (m_compo && m_compo->nc_requires) {
        for (int i = 0; m_compo->nc_requires[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += m_compo->nc_requires[i];
        }
    }
    return str;
}

wxString CConfigItem::GetProvisionList() const
{
    wxString str;

    if (m_option && m_option->nco_provides) {
        for (int i = 0; m_option->nco_provides[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += m_option->nco_provides[i];
        }
    }
    if (m_compo && m_compo->nc_provides) {
        for (int i = 0; m_compo->nc_provides[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += m_compo->nc_provides[i];
        }
    }
    return str;
}

int CConfigItem::GetEnumStrings(wxArrayString & arEnumStrings) const
{
    if (m_option && m_option->nco_choices) {
        for (int i = 0; m_option->nco_choices[i]; i++) {
            arEnumStrings.Add(m_option->nco_choices[i]);
        }
    }
    return arEnumStrings.GetCount();
}


/*!
 * \brief Check if this is a boolean item.
 */
bool CConfigItem::HasBool() const
{
    if (m_option && m_option->nco_flavor) {
        if (strcasecmp(m_option->nco_flavor, "boolean") == 0 || strcasecmp(m_option->nco_flavor, "booldata") == 0) {
            return true;
        }
    }
    return false;
}

/*!
 * \brief Check if this item is enabled.
 *
 * An item is disabled, if it requires other items being activated.
 */
bool CConfigItem::IsEnabled() const
{
    if (m_option) {
        return m_option->nco_enabled != 0;
    }
    if (m_compo) {
        return m_compo->nc_enabled != 0;
    }
    return false;
}

/*!
 * \brief Check if this item is active.
 *
 * An option is activated by the user or automatically by active_if.
 * Only options may become incative, components are always active.
 */
bool CConfigItem::IsActive() const
{
    if (m_option) {
        if(m_option->nco_active == 0) {
            if(m_option->nco_active_if) {
                CNutConfDoc *pDoc = wxGetApp().GetNutConfDoc();
                m_option->nco_active = pDoc->IsOptionActive(m_option->nco_active_if);
            }
        }
        return m_option->nco_active != 0;
    }
    return true;
}

/*!
 * \brief Set boolean value of this item.
 */
void CConfigItem::SetActive(bool ena)
{
    if (m_option) {
        m_option->nco_active = ena;
    }
}

bool CConfigItem::CanEdit() const
{
    if (!IsEnabled())
        return false;

    if (GetConfigType() != nutOption)
        return false;

    if (GetOptionFlavor() != nutFlavorData && GetOptionFlavor() != nutFlavorBoolData)
        return false;

    return true;
}
