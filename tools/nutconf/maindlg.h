#ifndef _MAINDLG_H_
#define _MAINDLG_H_

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
 * $Log$
 * Revision 1.1  2003/10/14 13:20:57  haraldkipp
 * First version using wxWindows
 *
 */

#include <wx/wx.h>

#include "userconf.h"

class CMainDlg: public wxDialog
{
public:    
    CMainDlg(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnConfig(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnSelCompiler(wxCommandEvent& evt);
    void OnSelTarget(wxCommandEvent& evt);
    void OnSelAdapter(wxCommandEvent& evt);
    void OnSelEthernet(wxCommandEvent& evt);
    void OnSetPort(wxCommandEvent& evt);

protected:
    wxRadioBox *m_rbCompiler;
    wxRadioBox *m_rbTarget;
    wxRadioBox *m_rbAdapter;
    wxRadioBox *m_rbEthernet;
    wxTextCtrl *m_tcPort;
    wxButton *m_btnConfig;
    wxButton *m_btnCancel;

private:
    CUserConf m_userConf;
    bool FileCopy(wxString src, wxString dst);
    bool RebuildLibs();

    DECLARE_EVENT_TABLE()
};

#endif
