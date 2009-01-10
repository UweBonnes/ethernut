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
 * $Log: mainframe.cpp,v $
 * Revision 1.4  2009/01/10 20:07:20  haraldkipp
 * Allow unicode build.
 *
 * Revision 1.3  2008/01/31 15:33:33  haraldkipp
 * Linux fixes.
 *
 * Revision 1.2  2008/01/28 16:43:11  haraldkipp
 * Version 2.2
 *
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
#include <wx/log.h>
#include <wx/listbox.h>
#include <wx/timer.h>
#endif

#include <wx/config.h>

//#include "wx/propdlg.h"

#include "version.h"
#include "setup.h"
#include "propdialog.h"
#include "udpthread.h"
#include "mainframe.h"

//new
#if !defined(__WXMSW__)
#include "bitmaps/nutdisc.xpm"
#endif
//

BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
    EVT_LIST_ITEM_ACTIVATED(ID_LIST, CMainFrame::OnActivated)
    EVT_MENU(ID_SCAN, CMainFrame::OnScan)
    EVT_MENU(ID_AUTOSCAN, CMainFrame::OnAutoScan)
    EVT_MENU(wxID_EXIT, CMainFrame::OnExit)
    EVT_MENU(ID_HIDE, CMainFrame::OnHide)
    EVT_MENU(ID_ABOUT, CMainFrame::OnAbout)
    EVT_MENU(UDP_EVENT, CMainFrame::OnUdpEvent)
    EVT_TIMER(wxID_ANY, CMainFrame::OnScanTimer)
END_EVENT_TABLE()

#if 0
wxPropertyValidatorRegistry myListValidatorRegistry;
#endif

/*!
 * \brief Constructor.
 */
CMainFrame::CMainFrame(const wxString& title)
    : wxFrame(NULL, -1, title)
    , m_timer(this)
{
    SetIcon(wxICON(nutdisc));

    /*
     * Create a list control to display all nodes, which we detected.
     */
    m_nutList = new wxListCtrl(this, ID_LIST, wxDefaultPosition, wxDefaultSize,
                                    wxLC_REPORT | wxLC_SINGLE_SEL |
                                    wxSUNKEN_BORDER | wxLC_EDIT_LABELS);
    m_nutList->InsertColumn(0, wxT("MAC"), wxLIST_FORMAT_LEFT, 100);
    m_nutList->InsertColumn(1, wxT("IP"), wxLIST_FORMAT_LEFT, 100);
    m_nutList->InsertColumn(2, wxT("Mask"), wxLIST_FORMAT_LEFT, 100);
    m_nutList->InsertColumn(3, wxT("Gate"), wxLIST_FORMAT_LEFT, 100);
    m_nutList->InsertColumn(4, wxT("Host"), wxLIST_FORMAT_LEFT, 100);

    /*
     * Create the menu bar.
     */
    wxMenu *menuAction = new wxMenu;
    menuAction->Append(ID_SCAN, wxT("&Scan"));
    menuAction->AppendCheckItem(ID_AUTOSCAN, wxT("&Autoscan"));
    menuAction->Append(wxID_EXIT, wxT("E&xit"));

    wxMenu *menuView = new wxMenu;
    menuView->Append(ID_HIDE, wxT("&Hide"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(ID_ABOUT, wxT("&About"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuAction, wxT("&Action") );
    menuBar->Append(menuView, wxT("&View") );
    menuBar->Append(menuHelp, wxT("&Help") );
    SetMenuBar(menuBar);

    /*
     * Restore frame position and size.
     */
    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/MainFrame"));
    Move(pConfig->Read(wxT("x"), 50), pConfig->Read(wxT("y"), 50));
    SetClientSize(pConfig->Read(wxT("w"), 350), pConfig->Read(wxT("h"), 200));

    m_taskBarIcon = new CTaskBarIcon(this);
#if defined(__WXCOCOA__)
    m_dockIcon = new CTaskBarIcon(this, wxTaskBarIcon::DOCK);
#endif
    if (!m_taskBarIcon->SetIcon(wxICON(nutdisc), wxT("wxTaskBarIcon Sample"))) {
        wxMessageBox(wxT("Could not set icon."));
    }

    /*
     * Create the discovery thread.
     */
    m_thread = new CUdpThread(this);
    m_thread->Create();
    m_thread->Run();
}

/*!
 * \brief Destructor.
 */
CMainFrame::~CMainFrame()
{
    /* 
     * Kill the discovery thread.
     */
    m_thread->Kill();

    /*
     * Save frame size and position.
     */
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        int x, y;
        GetPosition(&x, &y);
        pConfig->Write(wxT("/MainFrame/x"), (long) x);
        pConfig->Write(wxT("/MainFrame/y"), (long) y);
        int w, h;
        GetClientSize(&w, &h);
        pConfig->Write(wxT("/MainFrame/w"), (long) w);
        pConfig->Write(wxT("/MainFrame/h"), (long) h);
    }
    delete m_taskBarIcon;
#if defined(__WXCOCOA__)
    delete m_dockIcon;
#endif
}

/*!
 * \brief Add a newly discovered node.
 */
void CMainFrame::AddNode(DISCOVERY_TELE *info)
{
    long ix;
    long id = -1;
    wxString str;

    DISCOVERY_TELE *icopy = (DISCOVERY_TELE *)malloc(sizeof(DISCOVERY_TELE));
    memcpy(icopy, info, sizeof(DISCOVERY_TELE));

    if((ix = m_nutList->GetItemCount()) > 0) {
        if((id = m_nutList->FindItem(-1, CSetup::MacToString(info->dist_mac))) != -1) {
            ix = id;
        }
    }
    if(id == -1) {
        ix = m_nutList->InsertItem(ix, CSetup::MacToString(info->dist_mac), 0);
    }
    m_nutList->SetItemData(ix, (long)icopy);

    if(info->dist_cip_addr) {
        m_nutList->SetItem(ix, 1, CSetup::IpToString(info->dist_cip_addr));
    }
    else {
        m_nutList->SetItem(ix, 1, CSetup::IpToString(info->dist_ip_addr));
    }

    m_nutList->SetItem(ix, 2, CSetup::IpToString(info->dist_ip_mask));
    m_nutList->SetItem(ix, 3, CSetup::IpToString(info->dist_gateway));
    m_nutList->SetItem(ix, 4, wxString((char *)info->dist_hostname, wxConvLocal));

    m_nutList->SetColumnWidth(0, wxLIST_AUTOSIZE);
    m_nutList->SetColumnWidth(1, wxLIST_AUTOSIZE);
    m_nutList->SetColumnWidth(2, wxLIST_AUTOSIZE);
    m_nutList->SetColumnWidth(3, wxLIST_AUTOSIZE);
    m_nutList->SetColumnWidth(4, wxLIST_AUTOSIZE);
}

/*!
 * \brief Event handler.
 *
 * The event is created by pressing enter or double clicking an item.
 */
void CMainFrame::OnActivated(wxListEvent& event)
{
    wxListItem item;
    long ix = event.GetIndex();

    DISCOVERY_TELE *dist = (DISCOVERY_TELE *)m_nutList->GetItemData(ix);

    item.SetId(ix);
    item.SetColumn(1);
    item.SetMask(wxLIST_MASK_TEXT);
    m_nutList->GetItem(item);

    CPropDialog dlg(wxT("Settings"), dist);
    if(dlg.ShowModal() == wxID_OK) {
        if(wxMessageBox(wxT("Update?"), wxT("Setup"), wxYES_NO) == wxYES) {
            dlg.GetSetup(dist);
            dist->dist_type = DIST_APPLY;
            m_nutList->DeleteAllItems();
            m_thread->Broadcast(dist);
        }
        else {
            m_nutList->DeleteAllItems();
            m_thread->Broadcast();
        }
    }
}

void CMainFrame::OnUdpEvent(wxCommandEvent& event)
{
    DISCOVERY_TELE *dist = (DISCOVERY_TELE *)event.GetInt();
    if(!IsActive()) {
        RequestUserAttention();
    }
    AddNode(dist);
}

void CMainFrame::OnScan(wxCommandEvent& WXUNUSED(event))
{
    m_nutList->DeleteAllItems();
    m_thread->Broadcast();
}

void CMainFrame::OnAutoScan(wxCommandEvent& event)
{
    if (event.IsChecked()) {
        m_nutList->DeleteAllItems();
        m_thread->Broadcast();
        m_timer.Start(1000);
    } else {
        if (m_timer.IsRunning()) {
            m_timer.Stop();
        }
    }
}

void CMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void CMainFrame::OnHide(wxCommandEvent& WXUNUSED(event))
{
    Show(false);
}

void CMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;

    msg.Printf(wxT("NutDiscover Version %s")
#ifdef __WXDEBUG__
        wxT(" (debug)")
#endif
        wxT("\n\nCopyright 2004-2007 by egnite GmbH")
        wxT("\n\nBuild with wxWidgets Version %s.\n\n"),
        wxT(VERSION), wxVERSION_STRING);

    wxMessageBox(msg, _T("About"), wxOK | wxICON_INFORMATION, this);
}

void CMainFrame::OnScanTimer(wxTimerEvent& WXUNUSED(event)) 
{ 
    m_nutList->DeleteAllItems();
    m_thread->Broadcast();
}
