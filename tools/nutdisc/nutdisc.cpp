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
 * $Log: nutdisc.cpp,v $
 * Revision 1.1  2006/09/07 08:58:27  haraldkipp
 * First check-in
 *
 */


#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#endif

#include <wx/notebook.h>
#include <wx/config.h>

#include "mainframe.h"
#include "nutdisc.h"

#if !wxUSE_THREADS
    #error "This sample requires thread support!"
#endif // wxUSE_THREADS

IMPLEMENT_APP(CApp)

/*!
 * \brief Application main entry.
 */
bool CApp::OnInit()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int ec;

    SetVendorName(wxT("egnite"));
    SetAppName(wxT("nutdisc"));

    // Initialize the socket interface.
    wVersionRequested = MAKEWORD(1, 1);
    if((ec = WSAStartup(wVersionRequested, &wsaData)) != 0) {
        return 0;
    }
    if(LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
        WSACleanup();
        return 0;
    }

    if (!wxApp::OnInit())
        return false;

    wxFrame *frame = new CMainFrame(wxT("Nut/OS Discoverer"));
    frame->Show();

    return true;
}

int CApp::OnExit()
{
    delete wxConfigBase::Set((wxConfigBase *) NULL);

    WSACleanup();

    return 0;
}

