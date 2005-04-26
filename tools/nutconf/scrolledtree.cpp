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
 *
 * Mainly taken from wxWindows/contribs/gizmos, copyright by 
 *
 * Julian Smart, wxWindows licence.
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
 * $Log: scrolledtree.cpp,v $
 * Revision 1.5  2005/04/26 12:49:36  haraldkipp
 * Minor wxWidgets runtime warning avoided.
 *
 * Revision 1.4  2005/04/22 15:26:21  haraldkipp
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <math.h>

#include "scrolledtree.h"
#include "wx/generic/treectlg.h"

#ifdef __WXMSW__
#include <wx/msw/private.h>
#include <wx/window.h>
#endif


IMPLEMENT_CLASS(CScrolledTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(CScrolledTreeCtrl, wxTreeCtrl)
    EVT_SIZE(CScrolledTreeCtrl::OnSize)
    EVT_TREE_ITEM_EXPANDED(-1, CScrolledTreeCtrl::OnExpand)
    EVT_TREE_ITEM_COLLAPSED(-1, CScrolledTreeCtrl::OnExpand)
    EVT_SCROLLWIN(CScrolledTreeCtrl::OnScroll)
    EVT_PAINT(CScrolledTreeCtrl::OnPaint)
END_EVENT_TABLE();

CScrolledTreeCtrl::CScrolledTreeCtrl(wxWindow * parent, wxWindowID id, const wxPoint & pt,
     const wxSize & sz, long style)
     :wxTreeCtrl(parent, id, pt, sz, style & ~wxTR_ROW_LINES)
{
    m_companionWindow = NULL;
    m_drawRowLines = (style & wxTR_ROW_LINES) != 0;
    wxLogVerbose(wxT("Creating STC, style=%08lX"), style);
}

CScrolledTreeCtrl::~CScrolledTreeCtrl()
{
}


void CScrolledTreeCtrl::HideVScrollbar()
{
#ifdef __WXMSW__
    if (!IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxLogVerbose(wxT("-+-+- CScrolledTreeCtrl::HideVScrollbar"));
        ::ShowScrollBar((HWND) GetHWND(), SB_VERT, false);
    }
#endif
}

void CScrolledTreeCtrl::SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                                      int noUnitsX, int noUnitsY, 
                                      int xPos, int yPos, bool noRefresh)
{
#if !defined(__WXMSW__)
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;
        // Pass TRUE for noRefresh so that it doesn't
        // draw part of the tree as if the scroll view is
        // at zero vertically.
        win->wxGenericTreeCtrl::SetScrollbars(pixelsPerUnitX, pixelsPerUnitY, noUnitsX, 0, xPos, 0, /* noRefresh */ true);

        wxScrolledWindow *scrolledWindow = GetScrolledWindow();
        if (scrolledWindow) {
            wxLogVerbose(wxT("  - >  CScrolledTreeCtrl::SetScrollbars"));
            scrolledWindow->SetScrollbars(0, pixelsPerUnitY, 0, noUnitsY, 0, yPos, noRefresh);
        }
    }
#endif
}

int CScrolledTreeCtrl::GetScrollPos(int orient) const
{
#if !defined(__WXMSW__)
    wxScrolledWindow *scrolledWindow = GetScrolledWindow();

    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;

        if (orient == wxHORIZONTAL) {
            return win->wxGenericTreeCtrl::GetScrollPos(orient);
        } else {
            return scrolledWindow->GetScrollPos(orient);
        }
    }
#endif
    return 0;
}

void CScrolledTreeCtrl::GetViewStart(int *x, int *y) const
{
    wxScrolledWindow *scrolledWindow = GetScrolledWindow();

#if !defined(__WXMSW__)
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
        int x1, y1, x2, y2;
        win->wxGenericTreeCtrl::GetViewStart(& x1, & y1);
        *x = x1; 
        *y = y1;
        if (!scrolledWindow) {
            return;
        }
        scrolledWindow->GetViewStart(&x2, &y2);
        *y = y2;
    }
    else
#endif
    {
        // x is wrong since the horizontal scrollbar is controlled by the
        // tree control, but we probably don't need it.
        scrolledWindow->GetViewStart(x, y);
    }
}

void CScrolledTreeCtrl::PrepareDC(wxDC & dc)
{
#if !defined(__WXMSW__)
    if (IsKindOf(CLASSINFO(wxTreeCtrl))) {
        wxScrolledWindow *scrolledWindow = GetScrolledWindow();

        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;

        int startX, startY;
        GetViewStart(&startX, &startY);

        int xppu1, yppu1, xppu2, yppu2;
        win->wxGenericTreeCtrl::GetScrollPixelsPerUnit(&xppu1, &yppu1);
        scrolledWindow->GetScrollPixelsPerUnit(&xppu2, &yppu2);

        dc.SetDeviceOrigin(-startX * xppu1, -startY * yppu2);
    }
#endif
}

void CScrolledTreeCtrl::ScrollToLine(int posHoriz, int posVert)
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::ScrollToLine(%d,%d)"), posHoriz, posVert);
#ifdef __WXMSW__
    {
        UINT sbCode = SB_THUMBPOSITION;
        HWND vertScrollBar = 0;
        MSWDefWindowProc((WXUINT) WM_VSCROLL, MAKELONG(sbCode, posVert), (WXLPARAM) vertScrollBar);
    }
#else
    wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;
    win->Refresh();
#endif
}

void CScrolledTreeCtrl::OnSize(wxSizeEvent & event)
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::OnSize"));
    HideVScrollbar();
    AdjustRemoteScrollbars();
    event.Skip();
}

void CScrolledTreeCtrl::OnExpand(wxTreeEvent & event)
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::OnExpand"));
    AdjustRemoteScrollbars();
    event.Skip();

    // If we don't have this, we get some bits of lines still remaining
    if (event.GetEventType() == wxEVT_COMMAND_TREE_ITEM_COLLAPSED)
        Refresh();

    // Pass on the event
    if (m_companionWindow)
        m_companionWindow->GetEventHandler()->ProcessEvent(event);
}

/*
 * Paint the row lines.
 */
void CScrolledTreeCtrl::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxTreeCtrl::OnPaint(event);

    if (! m_drawRowLines)
        return;

    // Reset the device origin since it may have been set
    dc.SetDeviceOrigin(0, 0);

    wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT), 1, wxSOLID);
    dc.SetPen(pen);
    dc.SetBrush(* wxTRANSPARENT_BRUSH);

    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    int cy=0;
    wxTreeItemId h, lastH;
    for(h = GetFirstVisibleItem(); h.IsOk(); h = GetNextVisible(h)) {
        if (GetBoundingRect(h, itemRect)) {
            cy = itemRect.GetTop();
            dc.DrawLine(0, cy, clientSize.x, cy);
            lastH = h;
        }
        if (!IsVisible(h)) {
            break;
        }
    }
    if (lastH.IsOk() && GetBoundingRect(lastH, itemRect)) {
        cy = itemRect.GetBottom();
        dc.DrawLine(0, cy, clientSize.x, cy);
    }
}

// Adjust the containing wxScrolledWindow's scrollbars appropriately
void CScrolledTreeCtrl::AdjustRemoteScrollbars()
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::AdjustRemoteScrollbars"));
#if !defined(__WXMSW__)
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        // This is for the generic tree control.
        // It calls SetScrollbars which has been overridden
        // to adjust the parent scrolled window vertical
        // scrollbar.
        ((wxGenericTreeCtrl *) this)->AdjustMyScrollbars();
        return;
    } else
#endif
    {
        // This is for the wxMSW tree control
        wxScrolledWindow *scrolledWindow = GetScrolledWindow();
        if (scrolledWindow) {
            wxRect itemRect;
            if (GetBoundingRect(GetRootItem(), itemRect)) {
                // Actually, the real height seems to be 1 less than reported
                // (e.g. 16 instead of 16)
                int itemHeight = itemRect.GetHeight() - 1;

                int w, h;
                GetClientSize(&w, &h);

                wxRect rect(0, 0, 0, 0);
                CalcTreeSize(rect);

                double f = ((double) (rect.GetHeight()) / (double) itemHeight);
                int treeViewHeight = (int) ceil(f);

                int scrollPixelsPerLine = itemHeight;
                int scrollPos = -(itemRect.y / itemHeight);

                scrolledWindow->SetScrollbars(0, scrollPixelsPerLine, 0, treeViewHeight, 0, scrollPos);

                // Ensure that when a scrollbar becomes hidden or visible,
                // the contained window sizes are right.
                // Problem: this is called too early (?)
                wxSizeEvent event(scrolledWindow->GetSize(), scrolledWindow->GetId());
                scrolledWindow->GetEventHandler()->ProcessEvent(event);
            }
        }
    }
}

// Calculate the area that contains both rectangles
static wxRect CombineRectangles(const wxRect & rect1, const wxRect & rect2)
{
    wxRect rect;

    int right1 = rect1.GetRight();
    int bottom1 = rect1.GetBottom();
    int right2 = rect2.GetRight();
    int bottom2 = rect2.GetBottom();

    wxPoint topLeft = wxPoint(wxMin(rect1.x, rect2.x), wxMin(rect1.y, rect2.y));
    wxPoint bottomRight = wxPoint(wxMax(right1, right2), wxMax(bottom1, bottom2));

    rect.x = topLeft.x;
    rect.y = topLeft.y;
    rect.SetRight(bottomRight.x);
    rect.SetBottom(bottomRight.y);

    return rect;
}


void CScrolledTreeCtrl::CalcTreeSize(wxRect & rect)
{
    CalcTreeSize(GetRootItem(), rect);
}

void CScrolledTreeCtrl::CalcTreeSize(const wxTreeItemId & id, wxRect & rect)
{
    //wxLogVerbose(wxT("CScrolledTreeCtrl::CalcTreeSize"));
    // More efficient implementation would be to find the last item (but how?)
    // Q: is the bounding rect relative to the top of the virtual tree workspace
    // or the top of the window? How would we convert?
    wxRect itemSize;
    if (GetBoundingRect(id, itemSize)) {
        rect = CombineRectangles(rect, itemSize);
    }

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = GetFirstChild(id, cookie);
    while (childId) {
        CalcTreeSize(childId, rect);
        childId = GetNextChild(childId, cookie);
    }
}


wxScrolledWindow *CScrolledTreeCtrl::GetScrolledWindow() const
{
    //wxLogVerbose(wxT("CScrolledTreeCtrl::GetScrolledWindow"));
    wxWindow *parent = wxWindow::GetParent();
    while (parent) {
        if (parent->IsKindOf(CLASSINFO(wxScrolledWindow))) {
            return (wxScrolledWindow *) parent;
        }
        parent = parent->GetParent();
    }
    wxLogVerbose(wxT("CScrolledTreeCtrl::GetScrolledWindow(!!NONE!!)"));
    return NULL;
}

void CScrolledTreeCtrl::OnScroll(wxScrollWinEvent & event)
{
    wxLogVerbose(wxT("    CScrolledTreeCtrl::OnScroll"));
    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL) {
        event.Skip();
        return;
    }
    wxScrolledWindow *scrollWin = GetScrolledWindow();
    if (!scrollWin)
        return;

    int x, y;
    scrollWin->GetViewStart(&x, &y);

    wxLogVerbose(wxT("  CScrolledTreeCtrl -> ScrollToLine()"));
    ScrollToLine(-1, y);
}

void CScrolledTreeCtrl::SetCompanionWindow(wxWindow * companion)
{
    wxLogVerbose(wxT("SetCompanionWindow"));
    m_companionWindow = companion;
}

wxWindow *CScrolledTreeCtrl::GetCompanionWindow() const
{
    wxLogVerbose(wxT("GetCompanionWindow"));
    return m_companionWindow;
}

