/*
 * Copyright (C) 2003-2004 by egnite Software GmbH. All rights reserved.
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
 * $Log: nutconf.cpp,v $
 * Revision 1.2  2004/06/07 16:08:07  haraldkipp
 * Complete redesign based on eCos' configtool
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
#include <wx/mimetype.h>


#include "mainframe.h"
#include "nutconf.h"

IMPLEMENT_APP(NutConfApp);

/*!
 * \brief Application main entry.
 */
bool NutConfApp::OnInit()
{
    SetVendorName(wxT("egnite"));

    if (!wxApp::OnInit())
        return FALSE;

    m_docManager = new wxDocManager;
    m_currentDoc = NULL;
    new wxDocTemplate(m_docManager, "Nutconf", "*.ecc", "", "ecc", "Nutconf Doc", "Nutconf View",
                                             CLASSINFO(CNutConfDoc), CLASSINFO(CNutConfView));
    m_docManager->SetMaxDocsOpen(1);

    SetAppName(wxT("Nutconf"));

    m_mainFrame = new CMainFrame(m_docManager, wxT("Nut/OS Configuration"));
    SetTopWindow(m_mainFrame);
    m_mainFrame->Show();
    SendIdleEvents();

    m_docManager->CreateDocument(wxString(""), wxDOC_NEW);

    return TRUE;
}

int NutConfApp::OnExit()
{
    delete wxConfigBase::Set((wxConfigBase *) NULL);
    delete m_docManager;

    return 0;
}

CNutConfDoc *NutConfApp::GetNutConfDoc() const
{
    if (m_currentDoc)
        return m_currentDoc;

    if (!m_docManager)
        return NULL;

    return wxDynamicCast(m_docManager->GetCurrentDocument(), CNutConfDoc);
}

void NutConfApp::Log(const wxString & msg)
{
    CMainFrame *frame = (CMainFrame *) GetTopWindow();
    if (frame) {
        frame->GetOutputWindow()->AppendText(msg);
        if ((msg == wxEmptyString) || (msg.Last() != wxT('\n'))) {
            frame->GetOutputWindow()->AppendText(wxT("\n"));
        }
    }
}

void NutConfApp::SetStatusText(const wxString & text, bool clearFailingRulesPane)
{
    CMainFrame *mainFrame = GetMainFrame();
    if (mainFrame) {
        mainFrame->GetStatusBar()->SetStatusText(text, 0);
        if (clearFailingRulesPane)
            mainFrame->GetStatusBar()->SetStatusText(wxT(""), 3);
#ifdef __WXMSW__
        ::UpdateWindow((HWND) mainFrame->GetHWND());
#endif
    }
}

bool NutConfApp::Launch(const wxString & strFileName, const wxString & strViewer)
{
    bool ok = FALSE;
    wxString cmd;

    if (!strViewer.IsEmpty()) {
        cmd = strViewer + wxString(wxT(" ")) + strFileName;
    } else {
        wxString path, filename, ext;
        wxSplitPath(strFileName, &path, &filename, &ext);

        wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
        if (!ft) {
            wxLogError(wxT("Impossible to determine the file type for extension '%s'"), ext.c_str());
            return FALSE;
        }

        bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(strFileName, wxT("")));
        delete ft;

        if (!ok) {
            // TODO: some kind of configuration dialog here.
            wxMessageBox(wxT("Could not determine the command for opening this file."), wxT("Error"), wxOK | wxICON_EXCLAMATION);
            return FALSE;
        }
    }

    ok = (wxExecute(cmd, FALSE) != 0);

    return ok;
}

CMainFrame *NutConfApp::GetMainFrame() const
{
    return m_mainFrame;
}

wxDocManager *NutConfApp::GetDocManager() const
{
    return m_docManager;
}
