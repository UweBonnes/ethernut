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
 * $Log: propertylist.cpp,v $
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "nutconf.h"
#include "utils.h"
#include "propertylist.h"

const wxChar *CPropertyList::sm_fieldTypeImage[nutMAXFIELDTYPE] =
    { wxT("Type"), wxT("Value"), wxT("Default"), wxT("Macro"), wxT("File"), wxT("Enabled") };


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
    m_nFirstProperty = 0;

    AddColumns();
}

void CPropertyList::OnRightClick(wxMouseEvent & event)
{
}

void CPropertyList::AddColumns()
{
    InsertColumn(0, "Property", wxLIST_FORMAT_LEFT, 100);
    InsertColumn(1, "Value", wxLIST_FORMAT_LEFT, 300);
}

/*
 * Fill property list.
 */
void CPropertyList::Fill(CConfigItem * pti)
{
    if (pti == NULL) {
        ClearAll();
        AddColumns();
        m_nFirstProperty = 0;
        m_pti = NULL;
    } else {
        m_pti = pti;
        nutOptionType type = m_pti->GetOptionType();
        int i;

        for (i = GetItemCount() - 1; i >= 0; --i) {
            SetItemData(i, 0);
        }

        if (m_pti->HasBool()) {
            SetItem(nutEnabled, m_pti->IsEnabled()? wxT("True") : wxT("False"));
        }

        if (!m_pti->GetFilename().IsEmpty()) {
            SetItem(nutFile, m_pti->GetFilename());
        }
        if (!m_pti->GetMacro().IsEmpty()) {
            SetItem(nutMacro, m_pti->GetMacro());
        }

        if (type != nutOptionTypeNone) {

            switch (type) {
            case nutString:
                SetItem(nutValue, m_pti->StringValue());
                break;
            case nutInteger:
                break;
            case nutEnumerated:
                break;
            default:
                wxASSERT(FALSE);
                break;
            }
        }

        for (i = GetItemCount() - 1; i >= 0; --i) {
            if (0 == GetItemData(i)) {
                DeleteItem(i);
                if (i < m_nFirstProperty) {
                    m_nFirstProperty--;
                }
            }
        }
    }

    Refresh();
}

int CPropertyList::SetItem(nutFieldType f, const wxString & value)
{
    int nIndex = SetItem(sm_fieldTypeImage[f], value, m_nFirstProperty);
    if (nIndex == m_nFirstProperty) {
        m_nFirstProperty++;
    }
    SetItemData(nIndex, 1);
    return nIndex;
}

int CPropertyList::SetItem(const wxString & item, const wxString & value, int nInsertAs, int nRepeat)
{
    wxASSERT(nInsertAs <= GetItemCount());

    int nIndex = -1;
    nIndex = FindItem(0, /* nIndex */ item);

    if (-1 == nIndex) {
        nIndex = InsertItem(nInsertAs, item);
    }

    wxListCtrl::SetItem(nIndex, 1, value);


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
