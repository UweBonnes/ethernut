/*
 * Copyright (C) 2003 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*
 * $Log: maindlg.cpp,v $
 * Revision 1.1  2003/10/14 13:20:57  haraldkipp
 * First version using wxWindows
 *
 */

#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "maindlg.h"

enum {
    ID_CONFIG = 10000,
    IDRB_COMPILER,
    IDRB_TARGET,
    IDRB_ADAPTER,
    IDRB_ETHERNET,
    IDTC_PORT
};

BEGIN_EVENT_TABLE(CMainDlg, wxDialog)
    EVT_BUTTON(ID_CONFIG, CMainDlg::OnConfig)
    EVT_BUTTON(wxID_CANCEL, CMainDlg::OnQuit)
    EVT_CLOSE(CMainDlg::OnQuit)
    EVT_RADIOBOX(IDRB_COMPILER, CMainDlg::OnSelCompiler)
    EVT_RADIOBOX(IDRB_TARGET, CMainDlg::OnSelTarget)
    EVT_RADIOBOX(IDRB_ADAPTER, CMainDlg::OnSelAdapter)
    EVT_RADIOBOX(IDRB_ETHERNET, CMainDlg::OnSelEthernet)
    EVT_TEXT(IDTC_PORT, OnSetPort)
END_EVENT_TABLE()

/*!
 * List of Makefiles to copy.
 */
static const wxString filelist[] = {
    wxT("Makedefs"),
    wxT("Makerules"),
    wxT("app/Makedefs"),
    wxT("app/Makerules")
};

/*!
 * List of libraries to copy.
 */
static const wxString iccliblist[] = {
    wxT("crtnut.o"),
    wxT("crtnutram.o"),
    wxT("libnutcrt.a"),
    wxT("libnutcrtf.a"),
    wxT("libnutdev.a"),
    wxT("libnutfs.a"),
    wxT("libnutnet.a"),
    wxT("libnutos.a"),
    wxT("libnutpro.a")
};

/*!
 * \brief Compiler selections.
 */
static const wxString strComps[] = { 
    wxT("ImageCraft ICCAVR"), 
    wxT("GNU AVR-GCC") 
};

/*!
 * \brief Target selections.
 */
static const wxString strTargets[] = { 
    wxT("ATmega128 (red Ethernut)"),
    wxT("ATmega103 (green Ethernut)") 
};

/*!
 * \brief Adapter selections.
 */
static const wxString strAdapters[] = { 
    wxT("STK500 (serial port)"),
    wxT("STK200 (parallel port)") 
};

/*!
 * \brief Ethernet interface selections.
 */
static const wxString strEthernet[] = { 
    wxT("LAN91C111 (10/100 Mbps)"),
    wxT("RTL8019AS (10 MBps)"),
    wxT("CS8900 (10 MBps)") 
};

/*!
 * \brief Message texts.
 */
static const wxString strMsgTitle = wxT("Nut/OS");
static const wxString strMsgErrorTitle = wxT("Configuration Error");
static const wxString strMsgConfigured = wxT("Your system is now configured.");
static const wxString strMsgErrorUpdate = wxT("Updating user configuration file failed.");
static const wxString strMsgQueryRebuild = wxT("Rebuild Libraries?\n\nClicking YES will popup a text window.");
static const wxString strMsgErrorCreate = wxT("Can't create ");
static const wxString strMsgErrorNoFile = wxT(" not found.");
#ifdef __UNIX__
static const wxString strMsgUseMake = wxT("\nUse 'make install' to build the libraries.");
#endif

CMainDlg::CMainDlg(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxDialog((wxDialog *)NULL, -1, title, pos, size)
    , m_userConf()
{

    /*
     * Create compiler selection box.
     */
    m_rbCompiler = new wxRadioBox(this, IDRB_COMPILER, wxT("&Compiler"),
                                wxDefaultPosition, wxDefaultSize,
                                WXSIZEOF(strComps), strComps,
                                1, wxRA_SPECIFY_COLS);
    m_rbCompiler->SetSelection(m_userConf.GetCompiler());

#ifdef __UNIX__
    // No ICC on Linux.
    m_rbCompiler->Enable(0, false);
#else
    if(m_userConf.GetIccPath().IsEmpty()) {
        m_rbCompiler->Enable(0, false);
    }
    if(m_userConf.GetGccPath().IsEmpty()) {
        m_rbCompiler->Enable(1, false);
    }
#endif


    /*
     * Create target selection box.
     */
    m_rbTarget = new wxRadioBox(this, IDRB_TARGET, wxT("&Target"),
                                wxDefaultPosition, wxDefaultSize,
                                WXSIZEOF(strTargets), strTargets,
                                1, wxRA_SPECIFY_COLS);
    m_rbTarget->SetSelection(m_userConf.GetTarget());

    /*
     * Create adapter selection box.
     */
    m_rbAdapter = new wxRadioBox(this, IDRB_ADAPTER, wxT("&ISP Adapter"),
                                wxDefaultPosition, wxDefaultSize,
                                WXSIZEOF(strAdapters), strAdapters,
                                1, wxRA_SPECIFY_COLS);
    m_rbAdapter->SetSelection(m_userConf.GetAdapter());

    m_tcPort = new wxTextCtrl(this, IDTC_PORT);
    if (m_userConf.GetAdapter() == CUserConf::UC_STK200) {
        m_tcPort->Enable(false);
    }
    m_tcPort->SetValue(m_userConf.GetPort());

    /*
     * Create Ethernet selection box.
     */
    m_rbEthernet = new wxRadioBox(this, IDRB_ETHERNET, wxT("&Ethernet Chip"),
                                wxDefaultPosition, wxDefaultSize,
                                WXSIZEOF(strEthernet), strEthernet,
                                1, wxRA_SPECIFY_COLS);
    m_rbEthernet->SetSelection(m_userConf.GetEthernet());

    /*
     * Create buttons.
     */
    m_btnConfig = new wxButton( this, ID_CONFIG, "Configure", wxDefaultPosition, wxDefaultSize);
    m_btnCancel = new wxButton( this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
    m_btnConfig->Enable(m_userConf.IsDirty());

    /*
     * Layout the dialog using sizers.
     */
    wxSizer *szrButtons = new wxBoxSizer(wxHORIZONTAL);
    szrButtons->Add(m_btnConfig, 1, wxALIGN_BOTTOM | wxALL, 5);
    szrButtons->Add(m_btnCancel, 1, wxALIGN_BOTTOM | wxALL, 5);

    wxSizer *szrPort = new wxBoxSizer(wxHORIZONTAL);
    szrPort->Add(new wxStaticText(this, -1, "Port"), 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 5);
    szrPort->Add(m_tcPort, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxSizer *szrAdapter = new wxBoxSizer(wxVERTICAL);
    szrAdapter->Add(m_rbAdapter, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrAdapter->Add(szrPort, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrAdapter->Layout();

    wxSizer *szrEthernet = new wxBoxSizer(wxVERTICAL);
    szrEthernet->Add(m_rbEthernet, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxSizer *szrTop = new wxFlexGridSizer(2, 2);
    szrTop->Add(m_rbCompiler, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrTop->Add(m_rbTarget, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrTop->Add(szrAdapter, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrTop->Add(szrEthernet, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    szrTop->Add(szrButtons, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

    //szrTop->Layout();
    //SetAutoLayout(TRUE);
    SetSizer(szrTop);
    szrTop->SetSizeHints(this);
}

/*!
 * \brief Copy a file.
 */
bool CMainDlg::FileCopy(wxString src, wxString dst)
{
    bool rc = true;
    FILE *sp;
    FILE *dp;
    
    wxBeginBusyCursor();

    if((sp = fopen(src, "rb")) != 0) {
        if((dp = fopen(dst, "wb")) != NULL) {
            int cnt;
            char buf[512];

            while((cnt = fread(buf, 1, sizeof(buf), sp)) > NULL) {
                fwrite(buf, 1, cnt, dp);
            }
            fclose(dp);
        }
        else {
            wxMessageBox(strMsgErrorCreate + src, strMsgErrorTitle, wxICON_HAND | wxCANCEL, this);
        }
        fclose(sp);
    }
    else {
        wxMessageBox(src + strMsgErrorNoFile, strMsgErrorTitle, wxICON_HAND | wxCANCEL, this);
        rc = false;
    }

    wxEndBusyCursor();

    return rc;
}

/*!
 * \brief Rebuild libraries.
 *
 * \return true on success, false otherwise. On Linux it will
 *         always return false.
 */
bool CMainDlg::RebuildLibs()
{
    bool rc = false;

#ifdef __WXMSW__

    wxString cmd;
    wxString path;

    if(m_userConf.GetCompiler() == CUserConf::UC_AVRGCC) {
        path = m_userConf.GetGccPath();
        cmd.Printf(wxT("SET PATH=%s\\bin;%s\\utils\\bin;%%PATH%%"), path, path);
    }
    else if(m_userConf.GetCompiler() == CUserConf::UC_ICCAVR) {
        cmd.Printf(wxT("SET PATH=%s\\bin;%%PATH%%"), m_userConf.GetIccPath());
    }
    if(!cmd.IsEmpty()) {
        cmd += wxT(" & make clean & make install");
        rc = wxShell(cmd);
    }
#endif

    return rc;
}

/*!
 * \brief Configure button event.
 */
void CMainDlg::OnConfig(wxCommandEvent&)
{
    bool success = true;
    wxString ext;

    /*
     * Save configuration file.
     */
    if(m_userConf.IsDirty() && !m_userConf.Save()) {
        wxMessageBox(strMsgErrorUpdate, strMsgErrorTitle, wxICON_HAND, this);
    }
    else {

        if(m_userConf.GetCompiler() == CUserConf::UC_AVRGCC) {
            ext = wxT(".gcc");
        }
        else if(m_userConf.GetCompiler() == CUserConf::UC_ICCAVR) {
            ext = wxT(".icc");
        }
        for(int i = 0; i < WXSIZEOF(filelist); i++) {
            if((success = FileCopy(filelist[i] + ext, filelist[i])) == false) {
                break;
            }
        }

        if(success) {
#ifdef __WXMSW__
            /*
             * On Windows we offer to rebuild the libraries.
             */
            if(wxMessageBox(strMsgQueryRebuild, strMsgTitle, wxICON_QUESTION | wxYES_NO, this) == wxYES) {
                success = RebuildLibs();
                /*
                 * ICC supports a single libpath only. So we copy our libs.
                 */
                if(success && m_userConf.GetCompiler() == CUserConf::UC_ICCAVR) {
                    wxString strSrcPath = wxT("lib\\icc\\enhanced\\");
                    if(m_userConf.GetTarget() == CUserConf::UC_ATMEGA103)
			strSrcPath = wxT("lib\\icc\\mega\\");
		    wxString strDstPath = m_userConf.GetIccPath() + wxT("\\lib\\");
                    for(int i = 0; i < WXSIZEOF(iccliblist); i++) {
                        if((success = FileCopy(strSrcPath + iccliblist[i], strDstPath + iccliblist[i])) == false) {
                            break;
                        }
                    }
                }
            }
            if(success) {
                wxMessageBox(strMsgConfigured, strMsgTitle, wxICON_INFORMATION | wxOK, this);
                Destroy();
            }
#else
            wxMessageBox(strMsgConfigured + strMsgUseMake, strMsgTitle, wxICON_INFORMATION | wxOK, this);
            Destroy();
#endif
        }
    }
}

/*!
 * \brief Cancel button event.
 */
void CMainDlg::OnQuit(wxCommandEvent&)
{
    Destroy();
}

/*!
 * \brief Compiler selection event.
 */
void CMainDlg::OnSelCompiler(wxCommandEvent& evt)
{
    m_userConf.SetCompiler((CUserConf::ucCompiler)evt.m_commandInt);
    m_btnConfig->Enable(true);
}

/*!
 * \brief Target selection event.
 */
void CMainDlg::OnSelTarget(wxCommandEvent& evt)
{
    m_userConf.SetTarget((CUserConf::ucTarget)evt.m_commandInt);
    m_btnConfig->Enable(true);
}

/*!
 * \brief Adapter selection event.
 */
void CMainDlg::OnSelAdapter(wxCommandEvent& evt)
{
    CUserConf::ucAdapter iAdapter = (CUserConf::ucAdapter)evt.m_commandInt;

    m_userConf.SetAdapter(iAdapter);
    if(iAdapter == CUserConf::UC_STK500) {
        m_tcPort->Enable(true);
        m_userConf.SetPort(m_tcPort->GetValue());
    }
    else {
        m_tcPort->Enable(false);
    }
    m_btnConfig->Enable(true);
}

/*!
 * \brief Ethernet selection event.
 */
void CMainDlg::OnSelEthernet(wxCommandEvent& evt)
{
    m_userConf.SetEthernet((CUserConf::ucEthernet)evt.m_commandInt);
    m_btnConfig->Enable(true);
}

/*!
 * \brief Port name change event.
 */
void CMainDlg::OnSetPort(wxCommandEvent& evt)
{
    wxString val = m_tcPort->GetValue();

    if(val != m_userConf.GetPort()) {
        m_userConf.SetPort(val);
        m_btnConfig->Enable(true);
    }
}
