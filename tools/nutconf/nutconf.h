#ifndef _NUTCONF_H_
#define _NUTCONF_H_

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

#include "settings.h"
#include "nutconfdoc.h"
#include "mainframe.h"

#define VERSION "0.9.1"

class NutConfApp:public wxApp {
    friend class CMainFrame;
  public:
     virtual bool OnInit();
    virtual int OnExit();

    CNutConfDoc *GetNutConfDoc() const;
    CMainFrame *GetMainFrame() const;
    wxDocManager *GetDocManager() const;

    wxDocManager *m_docManager;
    CNutConfDoc *m_currentDoc;
    CMainFrame *m_mainFrame;

    void SetStatusText(const wxString & text);
    bool Launch(const wxString & strFileName, const wxString & strViewer);

    CSettings* GetSettings();
    bool Build(const wxString &target = wxT("all"));

  protected:
    CSettings* m_settings;
    wxString m_initialPath;
};


DECLARE_APP(NutConfApp);

#endif
