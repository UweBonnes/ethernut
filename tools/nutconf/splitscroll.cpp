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
 * $Log: splitscroll.cpp,v $
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "splitscroll.h"

// wxSplitterScrolledWindow
IMPLEMENT_CLASS(CSplitScroll, wxScrolledWindow)

    BEGIN_EVENT_TABLE(CSplitScroll, wxScrolledWindow)
    EVT_SCROLLWIN(CSplitScroll::OnScroll)
    EVT_SIZE(CSplitScroll::OnSize)
    END_EVENT_TABLE()

    CSplitScroll::CSplitScroll(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & sz, long style)
:  wxScrolledWindow(parent, id, pos, sz, style)
{
}

void CSplitScroll::OnSize(wxSizeEvent & event)
{
    wxSize sz = GetClientSize();
    if (GetChildren().First()) {
        ((wxWindow *) GetChildren().First()->Data())->SetSize(0, 0, sz.x, sz.y);
    }
}

void CSplitScroll::OnScroll(wxScrollWinEvent & event)
{
    // Ensure that events being propagated back up the window hierarchy
    // don't cause an infinite loop
    static bool inOnScroll = FALSE;
    if (inOnScroll) {
        event.Skip();
        return;
    }
    inOnScroll = TRUE;

    int orient = event.GetOrientation();

    int nScrollInc = CalcScrollInc(event);
    if (nScrollInc == 0) {
        inOnScroll = FALSE;
        return;
    }

    if (orient == wxHORIZONTAL) {
        inOnScroll = FALSE;
        event.Skip();
        return;
    } else {
        int newPos = m_yScrollPosition + nScrollInc;
        SetScrollPos(wxVERTICAL, newPos, TRUE);
    }

    if (orient == wxHORIZONTAL) {
        m_xScrollPosition += nScrollInc;
    } else {
        m_yScrollPosition += nScrollInc;
    }

    // Find targets in splitter window and send the event to them
    wxNode *node = GetChildren().First();
    while (node) {
        wxWindow *child = (wxWindow *) node->Data();
        if (child->IsKindOf(CLASSINFO(wxSplitterWindow))) {
            wxSplitterWindow *splitter = (wxSplitterWindow *) child;
            if (splitter->GetWindow1())
                splitter->GetWindow1()->ProcessEvent(event);
            if (splitter->GetWindow2())
                splitter->GetWindow2()->ProcessEvent(event);
            break;
        }
        node = node->Next();
    }

#ifdef __WXMAC__
    m_targetWindow->MacUpdateImmediately();
#endif

    inOnScroll = FALSE;
}
