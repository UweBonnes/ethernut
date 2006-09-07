#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

/* ----------------------------------------------------------------------------
 * Copyright (C) 2005-2006 by egnite Software GmbH
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
 * Revision 1.1  2006/09/07 08:58:27  haraldkipp
 * First check-in
 *
 */

#include <wx/listctrl.h>

#include "discovery.h"

class CUdpThread;

enum
{
    ID_SCAN = 1,
    ID_ABOUT,
    ID_LIST
};


/*!
 * \brief Application main window frame.
 */
class CMainFrame : public wxFrame
{
public:
    CMainFrame(const wxString& title);
    virtual ~CMainFrame();
    void AddNode(DISCOVERY_TELE *info);

    void OnActivated(wxListEvent& event);
    void OnUdpEvent(wxCommandEvent& event);

protected:
    void OnScan(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    wxListCtrl *m_nutList;

    CUdpThread *m_thread;

    DECLARE_EVENT_TABLE()
};

#endif
