#ifndef SCROLLEDTREE_H_
#define SCROLLEDTREE_H_

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

#include <wx/wx.h>
#include <wx/treectrl.h>

class CScrolledTreeCtrl:public wxTreeCtrl {
    DECLARE_CLASS(CScrolledTreeCtrl)
  public:
    CScrolledTreeCtrl(wxWindow * parent, wxWindowID id = -1, const wxPoint & pt = wxDefaultPosition,
                      const wxSize & sz = wxDefaultSize, long style = wxTR_HAS_BUTTONS);
    ~CScrolledTreeCtrl();

    void OnSize(wxSizeEvent & event);
    void OnExpand(wxTreeEvent & event);
    void OnScroll(wxScrollWinEvent & event);
    void OnPaint(wxPaintEvent& event);

    virtual void SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                               int noUnitsX, int noUnitsY, int xPos = 0, int yPos = 0, bool noRefresh = false);
    virtual void GetViewStart(int *x, int *y) const;
    virtual void PrepareDC(wxDC & dc);
    virtual int GetScrollPos(int orient) const;

    void SetCompanionWindow(wxWindow * companion);

     DECLARE_EVENT_TABLE()
  protected:
    void AdjustRemoteScrollbars();
    wxScrolledWindow *GetScrolledWindow() const;
    void HideVScrollbar();
    void ScrollToLine(int posHoriz, int posVert);
    wxWindow *GetCompanionWindow() const;
    void CalcTreeSize(wxRect & rect);
    void CalcTreeSize(const wxTreeItemId & id, wxRect & rect);

    wxWindow *m_companionWindow;
    bool m_drawRowLines;
};


#endif
