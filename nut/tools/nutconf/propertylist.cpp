/* ----------------------------------------------------------------------------
 * Copyright (C) 2004-2005 by egnite Software GmbH
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
 * $Log: propertylist.cpp,v $
 * Revision 1.6  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.5  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.4  2005/04/22 15:20:32  haraldkipp
 * Avoid compiler warnings.
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

#include "nutconf.h"
#include "utils.h"
#include "propertylist.h"

const wxChar *CPropertyList::m_propertyName[nutMAXFIELDTYPE] = { 
    wxT("Enabled"), 
    wxT("Active"),
    wxT("Requires"), 
    wxT("Provides"), 
    wxT("Exclusivity"), 
    wxT("Type"), 
    wxT("File"), 
    wxT("Macro"), 
    wxT("Value")
};


/*
 * CPropertyList
 */

IMPLEMENT_CLASS(CPropertyList, wxListCtrl)

BEGIN_EVENT_TABLE(CPropertyList, wxListCtrl)
    EVT_RIGHT_DOWN(CPropertyList::OnRightClick)
    EVT_LEFT_DCLICK(CPropertyList::OnDoubleClick)
END_EVENT_TABLE();

CPropertyList::CPropertyList(wxWindow * parent, wxWindowID id, const wxPoint & pt,
                                 const wxSize & sz, long style):wxListCtrl(parent, id, pt, sz, style)
{
    m_pti = NULL;

    AddColumns();
}

void CPropertyList::OnRightClick(wxMouseEvent & WXUNUSED(event))
{
}

void CPropertyList::AddColumns()
{
    InsertColumn(0, wxT("Property"), wxLIST_FORMAT_LEFT, 100);
    InsertColumn(1, wxT("Value"), wxLIST_FORMAT_LEFT, 300);
}

/*
 * Fill property list.
 */
void CPropertyList::Fill(CConfigItem * pti)
{
    /*
     * No item specified, clear the list.
     */
    if (pti == NULL) {
        ClearAll();
        AddColumns();
        m_pti = NULL;
    } 
    /*
     * Display this item's properties.
     */
    else {
        m_pti = pti;
        int i;

        /*
         * Remove previously set item data.
         */
        for (i = GetItemCount() - 1; i >= 0; --i) {
            SetItemData(i, 0);
        }

        if (!m_pti->GetRequirementList().IsEmpty()) {
            SetItem(nutRequires, m_pti->GetRequirementList());
        }
        if (!m_pti->GetProvisionList().IsEmpty()) {
            SetItem(nutProvides, m_pti->GetProvisionList());
        }
        if (!m_pti->GetExclusivityList().IsEmpty()) {
            SetItem(nutExclusivity, m_pti->GetExclusivityList());
        }

        /*
         * Set the file name property. Typically a relative path to a
         * source code header file.
         */
        if (!m_pti->GetFilename().IsEmpty()) {
            SetItem(nutFile, m_pti->GetFilename());
        }

        /*
         * Set the macro property.
         */
        if (!m_pti->GetMacro().IsEmpty()) {
            SetItem(nutMacro, m_pti->GetMacro());

            /*
             * The macro's value is optional.
             */
            switch (m_pti->GetOptionType()) {
            case nutOptionTypeNone:
                break;
            case nutEnumerated:
            case nutString:
                if(!m_pti->StringValue().IsEmpty()) {
                    SetItem(nutValue, m_pti->StringValue());
                }
                break;
            case nutInteger:
                break;
            default:
                wxASSERT(false);
                break;
            }

            /*
             * Boolean options must be active to be included.
             */
            if (m_pti->HasBool()) {
                SetItem(nutActive, m_pti->IsActive()? wxT("True") : wxT("False"));
            }
        }

        /*
         * An item may be disabled, if it requires a property which
         * is not provided by any active component.
         */
        SetItem(nutEnabled, m_pti->IsEnabled()? wxT("Yes") : wxT("No"));

        /*
         * Remove all items from the property list without previously
         * set data.
         */
        for (i = GetItemCount() - 1; i >= 0; --i) {
            if (GetItemData(i) == 0) {
                DeleteItem(i);
            }
        }
    }
    Refresh();
}

void CPropertyList::RefreshValue()
{
    if(m_pti) {
        if (m_pti->HasBool()) {
            SetItem(nutActive, m_pti->IsActive()? wxT("True") : wxT("False"));
        }
        SetItem(nutEnabled, m_pti->IsEnabled()? wxT("Yes") : wxT("No"));
        if (m_pti->GetOptionType () != nutOptionTypeNone) {
            if(!m_pti->StringValue().IsEmpty()) {
                SetItem(nutValue, m_pti->StringValue());
            }
        }
    }
}

int CPropertyList::SetItem(nutPropertyType type, const wxString & value)
{
    int nIndex;

    /* Check if this item is in the list already. */
    nIndex = FindItem(0, m_propertyName[type]);
    if (nIndex == -1) {
        nIndex = InsertItem(GetItemCount(), m_propertyName[type]);
    }
    wxListCtrl::SetItem(nIndex, 1, value);
    SetItemData(nIndex, 1);

    return nIndex;
}

void CPropertyList::OnDoubleClick(wxMouseEvent & event)
{
    int flags;
    long item = HitTest(event.GetPosition(), flags);
    if (item > -1) {
        wxString strText;
        wxListItem listItem;
        listItem.m_mask = wxLIST_MASK_TEXT;
        listItem.m_itemId = item;
        listItem.m_col = 0;
        if (GetItem(listItem))
            strText = listItem.m_text;
        else
            strText = wxEmptyString;

        if (strText == wxT("File")) {
            m_pti->ViewHeaderFile();
        }
    }
    event.Skip();
}

