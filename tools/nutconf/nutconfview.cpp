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
 * $Log: nutconfview.cpp,v $
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "nutconf.h"
#include "utils.h"
#include "treeitemdata.h"
#include "nutconfhint.h"
#include "nutconfview.h"

IMPLEMENT_DYNAMIC_CLASS(CNutConfView, wxView)

    BEGIN_EVENT_TABLE(CNutConfView, wxView)
    END_EVENT_TABLE();

CNutConfView::CNutConfView()
{
    m_expandedForFind = wxTreeItemId();
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool CNutConfView::OnCreate(wxDocument * doc, long WXUNUSED(flags))
{
    wxGetApp().GetDocManager()->ActivateView(this, true);

    return true;
}

void CNutConfView::OnDraw(wxDC * dc)
{
}

void CNutConfView::OnUpdate(wxView * WXUNUSED(sender), wxObject * hintObj)
{
    CNutConfDoc *pDoc = (CNutConfDoc *) GetDocument();
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    wxASSERT(pDoc);

    CConfigItem *selItem = NULL;

    wxTreeItemId sel = treeCtrl->GetSelection();
    if (sel.IsOk()) {
        CTreeItemData *data = (CTreeItemData *) treeCtrl->GetItemData(sel);
        if (data)
            selItem = data->GetConfigItem();
    }

    CNutConfHint *hint = (CNutConfHint *) hintObj;
    int hintOp = nutNoHint;
    if (hint)
        hintOp = hint->m_op;

    switch (hintOp) {
    case nutSelChanged:

        {
            // Note that we're cheating a bit here, since we're using the tree view
            // to update another view, instead of having a view per control as in the MFC
            // version. However, it doesn't seem to be worth the extra machinery.
            // Update the description window
            if (selItem) {
                wxGetApp().GetMainFrame()->GetShortDescriptionWindow()->SetValue(selItem->GetDescription());

                // Update the properties window
                wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(selItem);
            } else {
                wxGetApp().GetMainFrame()->GetShortDescriptionWindow()->Clear();
                wxGetApp().GetMainFrame()->GetPropertyListWindow()->ClearAll();
            }

        }
        break;
    case nutAllSaved:

        {
            int nItem;
            for (nItem = 0; nItem < pDoc->GetItems().Number(); nItem++) {
                CConfigItem *pItem = (CConfigItem *) pDoc->GetItems()[nItem];
                wxTreeItemId treeItem = pItem->GetTreeItem();
                if (treeItem) {
                    treeCtrl->SetItemText(treeItem, pItem->GetName());
                }
            }

            // Update the properties window
            if (selItem) {
                wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(selItem);
            }
            // Update the value pane
            wxGetApp().GetMainFrame()->GetValueWindow()->Refresh();
        }
        break;
    case nutFilenameChanged:

        {
            // Update the properties window
            if (selItem) {
                wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(selItem);
            }
        }
        break;
    case nutNameFormatChanged:
        {
            int nItem;
            for (nItem = 0; nItem < pDoc->GetItems().Number(); nItem++) {
                CConfigItem *pItem = (CConfigItem *) pDoc->GetItems()[nItem];
                wxString strName(pItem->GetName());
                treeCtrl->SetItemText(pItem->GetTreeItem(), strName);
            }

            treeCtrl->Refresh();
        }
        break;
    case nutIntFormatChanged:
        if (selItem && selItem->GetOptionType() == nutInteger) {
            wxGetApp().GetMainFrame()->GetPropertyListWindow()->SetItem(CPropertyList::nutValue, selItem->StringValue());
        }
        break;
    case nutClear:
        {
            m_expandedForFind = wxTreeItemId();

            treeCtrl->DeleteAllItems();

            wxGetApp().GetMainFrame()->GetShortDescriptionWindow()->Clear();
            wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(NULL);
            wxGetApp().GetMainFrame()->GetValueWindow()->Refresh();
            break;
        }

    case nutValueChanged:
        {
            CConfigItem & ti = *(CConfigItem *) hint->m_item;

            // Update the tree item
            ti.UpdateTreeItem(*treeCtrl);

            // Update the value cell
            CValueWindow *valueWindow = wxGetApp().GetMainFrame()->GetValueWindow();
            if (valueWindow) {
                wxRect rect = valueWindow->GetItemRect(&ti);
#ifdef __WXMSW__
                valueWindow->Refresh(true, &rect);
#else
                // For some reason, on Linux, the value window gets
                // blanked out if a dialog pops up as a result of the value change.
                valueWindow->Refresh();
#endif
            }
            // Properties window
            if (selItem) {
            }
        }
        break;

    case nutExternallyChanged:
        {
            int nItem;
            for (nItem = 0; nItem < pDoc->GetItems().Number(); nItem++) {
                CConfigItem *pItem = (CConfigItem *) pDoc->GetItems()[nItem];
                pItem->UpdateTreeItem(*treeCtrl);
            }
        }
        break;
    default:
        break;                  // not for us, apparently
    }
}

// Clean up windows used for displaying the view.
bool CNutConfView::OnClose(bool deleteWindow)
{
    CNutConfHint hint(NULL, nutClear);
    GetDocument()->UpdateAllViews(NULL, &hint);

    if (!GetDocument()->Close())
        return FALSE;

    wxGetApp().GetDocManager()->ActivateView(this, FALSE);

    Activate(FALSE);

    return true;
}

void CNutConfView::Refresh(const wxString & macroName)
{
}

void CNutConfView::Refresh(wxTreeItemId h)
{
}
