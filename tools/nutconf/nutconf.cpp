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
 * Revision 1.4  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.3  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
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
#include <wx/cmdline.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/splash.h>
#include <wx/bitmap.h>
#include <wx/busyinfo.h>


#include "mainframe.h"
#include "nutconf.h"

#if !defined(__WXMSW__)
#include "bitmaps/nutconf.xpm"
#endif

IMPLEMENT_APP(NutConfApp);

/*!
 * \brief Application main entry.
 */
bool NutConfApp::OnInit()
{
    m_docManager = NULL;

    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        { wxCMD_LINE_SWITCH, wxT("v"), _T("verbose"), _T("be verbose") },
        { wxCMD_LINE_SWITCH, wxT("h"), _T("help"), _T("show usage"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
        { wxCMD_LINE_NONE }
    };

    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.SetLogo(_T("Nut/OS Configurator\n" VERSION
                      "Copyright (c) 2004 by egnite Software GmbH\n"
                      "Copyright (C) 1998, 1999, 2000 Red Hat, Inc."));
    if(parser.Parse()) {
        return false;
    }
    if(parser.Found(wxT("v"))) {
        wxLog::GetActiveTarget()->SetVerbose();
    }
    else {
        wxLog::GetActiveTarget()->SetVerbose(false);
    }

    /*
     * Load settings early.
     */
    SetVendorName(wxT("egnite"));
    SetAppName(wxT("NutConf"));
    m_settings = new CSettings();

    /*
     * Create a wxConfig object early.
     */
    if (!wxApp::OnInit())
        return false;

    wxGetEnv(wxT("PATH"), &m_initialPath);

    /*
     * The document manager will handle non application specific menu commands.
     */
    m_docManager = new wxDocManager;
    m_currentDoc = NULL;

    /*
     * The document template defines the relationship between document and view.
     */
    new wxDocTemplate(m_docManager, "Nut/OS Configuration", "*.conf", "", "conf", "NutconfDoc", "NutconfView",
                                             CLASSINFO(CNutConfDoc), CLASSINFO(CNutConfView));
    m_docManager->SetMaxDocsOpen(1);

    m_mainFrame = new CMainFrame(m_docManager, wxT("Nut/OS Configurator"));
    SetTopWindow(m_mainFrame);
    m_mainFrame->Show();
    SendIdleEvents();

    /*
     * Splash display.
     */
    wxBitmap bmp(wxBITMAP(SSB_NUTCONF));
    wxSplashScreen* splash = new wxSplashScreen(bmp, wxSPLASH_CENTRE_ON_PARENT, 
                0, m_mainFrame, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxSTAY_ON_TOP);
    wxYield();
    wxSleep(1);
    if(splash) {
        delete splash;
    }

    /*
     * Create the document. 
     */
    m_docManager->CreateDocument(m_settings->m_configname, 0);

    return true;
}

int NutConfApp::OnExit()
{
    delete wxConfigBase::Set((wxConfigBase *) NULL);
    if(m_docManager) {
        delete m_docManager;
    }
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

void NutConfApp::SetStatusText(const wxString & text)
{
    CMainFrame *mainFrame = GetMainFrame();
    if (mainFrame) {
        mainFrame->GetStatusBar()->SetStatusText(text, 0);
#ifdef __WXMSW__
        ::UpdateWindow((HWND) mainFrame->GetHWND());
#endif
    }
}

CMainFrame *NutConfApp::GetMainFrame() const
{
    return m_mainFrame;
}

wxDocManager *NutConfApp::GetDocManager() const
{
    return m_docManager;
}

CSettings* NutConfApp::GetSettings() 
{ 
    return m_settings; 
}

bool NutConfApp::Launch(const wxString & strFileName, const wxString & strViewer)
{
    bool ok = false;
    wxString cmd;

    if (!strViewer.IsEmpty()) {
        cmd = strViewer + wxString(wxT(" ")) + strFileName;
    } else {
        wxString path, filename, ext;
        wxSplitPath(strFileName, &path, &filename, &ext);

        wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
        if (!ft) {
            wxLogError(wxT("Impossible to determine the file type for extension '%s'"), ext.c_str());
            return false;
        }

        bool ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(strFileName, wxT("")));
        delete ft;

        if (!ok) {
            // TODO: some kind of configuration dialog here.
            wxMessageBox(wxT("Could not determine the command for opening this file."), wxT("Error"), wxOK | wxICON_EXCLAMATION);
            return false;
        }
    }

    ok = (wxExecute(cmd, false) != 0);

    return ok;
}

bool NutConfApp::Build(const wxString &target)
{
    CNutConfDoc *doc = GetNutConfDoc();
    if(doc == NULL) {
        return false;
    }

    /*
     * Change working directory to the top build directory.
     */
    wxString initialCwd = wxFileName::GetCwd();
    if(!m_settings->m_buildpath.IsEmpty()) {
        wxFileName::SetCwd(m_settings->m_buildpath);
    }

    /* Add tool directories to the PATH. */
#ifdef _WIN32
    wxString newPath(wxT("c:/WinAVR/bin;c:/WinAVR/utils/bin;"));
    newPath += m_initialPath;
    wxSetEnv(wxT("PATH"), newPath);
#endif

    /*
     * Assemble the command.
     */
    wxString cmd = wxT("make ") + target;

    /*
     * Start execution. stdout and stderr output is collected
     * in string arrays. Not very nice here, but works for now.
     */
    wxLogMessage(wxT("----- Running '%s' -----"), cmd.c_str());
    wxBusyInfo wait(wxT("Please wait, running '") + cmd + wxT("'..."));
    wxArrayString output, errors;
    int code = wxExecute(cmd, output, errors);

    /*
     * Display collected output in the output window.
     */
    if(code != -1) {
        wxTextCtrl *outwin = wxGetApp().GetMainFrame()->GetOutputWindow();
        size_t i;
        size_t count;

        if(wxLog::GetVerbose()) {
            count = output.GetCount();
            for (i = 0; i < count; i++) {
                outwin->AppendText(output[i]);
                outwin->AppendText(wxT("\n"));
            }
        }
        count = errors.GetCount();
        for (i = 0; i < count; i++) {
            outwin->AppendText(errors[i]);
            outwin->AppendText(wxT("\n"));
        }
    }
    if(code) {
        wxLogMessage(wxT("----- '%s' failed with error %d -----"), cmd.c_str(), code);
    }
    else {
        wxLogMessage(wxT("----- '%s' terminated successfully -----"), cmd.c_str(), code);
    }

    /* 
     * Restore initial environment. 
     */
    wxSetEnv(wxT("PATH"), m_initialPath);
    wxFileName::SetCwd(initialCwd);

    return (code == 0);
}
