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

#ifndef _USERCONF_H_
#define _USERCONF_H_

#include <wx/wx.h>

extern const wxString userConfName;

class CUserConf
{
public:
    CUserConf(const char *filename = userConfName);
    virtual ~CUserConf();
    bool Load(const char *filename = userConfName);
    bool Save(const char *filename = userConfName);

    enum ucCompiler { UC_ICCAVR, UC_AVRGCC };
    enum ucTarget { UC_ATMEGA128, UC_ATMEGA103 };
    enum ucAdapter { UC_STK500, UC_STK200 };
    enum ucEthernet { UC_LANC111, UC_RTL8019, UC_CS8900 };

    bool IsDirty();

    ucCompiler GetCompiler();
    ucTarget GetTarget();
    ucAdapter GetAdapter();
    ucEthernet GetEthernet();
    wxString GetPort();
    wxString GetIccPath();
    wxString GetGccPath();

    void SetCompiler(ucCompiler val);
    void SetTarget(ucTarget val);
    void SetAdapter(ucAdapter val);
    void SetEthernet(ucEthernet val);
    void SetPort(wxString val);

private:
    wxString GetInstallPath(wxString strAppKey);
    void FindPaths();

    bool m_bDirty;

    wxString m_strFileName;
    wxString m_strIccPath;
    wxString m_strGccPath;

    ucCompiler m_iCompiler;
    ucTarget m_iTarget;
    ucAdapter m_iAdapter;
    ucEthernet m_iEthernet;

    wxString m_strBurn;
    wxString m_strBurnFlags;
    wxString m_strPart;
    wxString m_strPort;
    wxString m_strSpeed;
    wxString m_strCrurom;
    wxString m_strBoard;
};

#endif
