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
 * $Log: userconf.cpp,v $
 * Revision 1.1  2003/10/14 13:20:57  haraldkipp
 * First version using wxWindows
 *
 */

#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/msw/registry.h>

#include "userconf.h"

enum ucItems { UCI_MCU, UCI_BURN, UCI_BURNFLAGS, UCI_CRUROM, UCI_AVRPATH, UCI_HWDEF };

static const wxString itemlist[] = {
    wxT("MCU="),
    wxT("BURN="),
    wxT("BURNFLAGS="),
    wxT("CRUROM="),
    wxT("AVR="),
    wxT("HWDEF=")
};

const wxString userConfName = wxT("UserConf.mk");

#ifdef __UNIX__
static const wxString tooldir = wxT("$(top_srcdir)/tools/linux/");
#elif defined(__WXMSW__)
static const wxString tooldir = wxT("$(top_srcdir)\\tools\\win32\\");
#endif

CUserConf::CUserConf(const char *filename)
    : m_iCompiler(UC_ICCAVR)
    , m_iTarget(UC_ATMEGA128)
    , m_iAdapter(UC_STK500)
    , m_iEthernet(UC_LANC111)
{
    FindPaths();

    /*
     * Load an existing configuration. If it fails, we set
     * the dirty flag to force its creation.
     */
    m_bDirty = !Load(filename);
}

CUserConf::~CUserConf()
{
}

/*!
 * \brief Find paths of compiler installations.
 */
void CUserConf::FindPaths()
{
#ifdef __WXMSW__
    wxPathList pathList;
    pathList.AddEnvList(wxT("PATH"));

    wxString strPath = GetInstallPath(wxT("ICC_AVR"));
    if(!strPath.IsEmpty()) {
        pathList.Add(strPath + wxT("\\bin"));
    }
    strPath = GetInstallPath(wxT("WinAVR"));
    if(!strPath.IsEmpty()) {
        pathList.Add(strPath + wxT("\\bin"));
    }

    m_strIccPath = pathList.FindAbsoluteValidPath(wxT("iccavr.exe"));
    if(!m_strIccPath.IsEmpty()) {
        m_strIccPath = m_strIccPath.BeforeLast('\\');
        m_strIccPath = m_strIccPath.BeforeLast('\\');
    }
    m_strGccPath = pathList.FindAbsoluteValidPath(wxT("avr-gcc.exe"));
    if(!m_strGccPath.IsEmpty()) {
        m_strGccPath = m_strGccPath.BeforeLast('\\');
        m_strGccPath = m_strGccPath.BeforeLast('\\');
    }
#else
    m_strGccPath = "";
    m_strIccPath = "";
#endif
}

/*!
 * \brief Query installation path of the specified compiler.
 */
wxString CUserConf::GetInstallPath(wxString strAppKey)
{
    wxString strPath;

#ifdef __WXMSW__
    wxString strNode = wxT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    strNode += strAppKey + wxT("\\");
    wxRegKey key(strNode);

    key.QueryValue("UninstallString", strPath);
    strPath.Replace("\"", " ");
    strPath.Trim();
    strPath.Trim(false);
    if(strPath.Contains(" "))
        strPath = strPath.AfterFirst(' ');
    strPath = strPath.BeforeLast('\\');
#endif

    return strPath;
}

/*!
 * \brief Load configuration.
 */
bool CUserConf::Load(const char *filename)
{
    bool rc = false;
    wxString line;
    wxString val;
    int pos;
    
    if(wxFileExists(filename)) {
        
        wxFileInputStream stream(filename);
        if(stream.Ok()) {
            wxTextInputStream cfile(stream);
            rc = true;
            while(true) {
                line = cfile.ReadLine();
                if(stream.Eof()) {
                    break;
                }
                for(int item = 0; item < WXSIZEOF(itemlist); item++) {
                    if(line.StartsWith(itemlist[item], &val)) {
                        break;
                    }
                }
                switch(item) {
                case UCI_MCU:
                    if(val.IsSameAs(wxT("atmega103"))) {
                        m_iCompiler = UC_AVRGCC;
                        m_iTarget = UC_ATMEGA103;
                    }
                    else if(val.IsSameAs(wxT("atmega128"))) {
                        m_iCompiler = UC_AVRGCC;
                        m_iTarget = UC_ATMEGA128;
                    }
                    else if(val.IsSameAs(wxT("atmega"))) {
                        m_iCompiler = UC_ICCAVR;
                        m_iTarget = UC_ATMEGA103;
                    }
                    else if(val.IsSameAs(wxT("enhanced"))) {
                        m_iCompiler = UC_ICCAVR;
                        m_iTarget = UC_ATMEGA128;
                    }
                    break;
                case UCI_BURN:
                    m_strBurn = val;
                    break;
                case UCI_BURNFLAGS:
                    m_strBurnFlags = val;
                    if(val.Contains(wxT("stk200"))) {
                        m_iAdapter = UC_STK200;
                    }
                    else if(val.Contains(wxT("stk500"))) {
                        m_iAdapter = UC_STK500;
                    }
                    if((pos = val.Find(wxT("-dpart="))) >= 0) {
                        m_strPart = val.Mid(pos + 7);
                        m_strPart = m_strPart.BeforeFirst(' ');
                    }
                    if((pos = val.Find(wxT("-dserial="))) >= 0) {
                        m_strPort = val.Mid(pos + 9);
                        m_strPort = m_strPort.BeforeFirst(' ');
                    }
                    if((pos = val.Find(wxT("-dspeed="))) >= 0) {
                        m_strSpeed = val.Mid(pos + 8);
                        m_strSpeed = m_strSpeed.BeforeFirst(' ');
                    }
                    break;
                case UCI_CRUROM:
                    m_strCrurom = val;
                    break;
                case UCI_AVRPATH:
                    break;
                case UCI_HWDEF:
                    m_strBoard = val;
                    if(val.Contains(wxT("-DETHERNUT2"))) {
                        m_iEthernet = UC_LANC111;
                    }
                    else if(val.Contains(wxT("-DULTRALITE"))) {
                        m_iEthernet = UC_CS8900;
                    }
                    else {
                        m_iEthernet = UC_RTL8019;
                    }
                    break;
                }
            }
        }
    }
    return rc;
}

/*!
 * \brief Save configuration.
 */
bool CUserConf::Save(const char *filename)
{
    bool rc = true;
    wxString val;

    wxFileOutputStream sout(filename);

    if(sout.Ok()) {
        wxTextOutputStream tout(sout);

        /*
         * Set target name.
         */
        if(m_iCompiler == UC_AVRGCC) {
            if(m_iTarget) {
                val = wxT("atmega103");
            }
            else {
                val = wxT("atmega128");
            }
        }
        else if(m_iCompiler == UC_ICCAVR) {
            if(m_iTarget) {
                val = wxT("atmega");
            }
            else {
                val = wxT("enhanced");
            }
        }
        tout << itemlist[UCI_MCU] << val << wxT("\n");

        /*
         * Set programmer name.
         */
        if(m_strBurn.IsEmpty()) {
            if(m_iCompiler == UC_ICCAVR) {
                m_strBurn = tooldir + wxT("uisp");
            }
            else {
                m_strBurn = wxT("uisp");
            }
        }
        tout << itemlist[UCI_BURN] << m_strBurn << wxT("\n");

        /*
         * Set programmer options.
         */
        if(m_strBurnFlags.IsEmpty()) {
            if(m_strPart.IsEmpty()) {
                if(m_iTarget == UC_ATMEGA103) {
                    m_strPart = wxT("atmega103");
                }
                else {
                    m_strPart = wxT("atmega128");
                }
            }
            if(m_iAdapter == UC_STK200) {
                m_strBurnFlags = wxT("-dprog=stk200");
            }
            else if(m_iAdapter == UC_STK500) {
                if(m_strPort.IsEmpty()) {
                    m_strPort = wxT("/dev/ttyS0");
                }
                if(m_strSpeed.IsEmpty()) {
                    m_strSpeed = wxT("115200");
                }
                m_strBurnFlags = wxT("-dprog=stk500 -dserial=") + m_strPort + wxT(" -dspeed=") + m_strSpeed;
            }
            m_strBurnFlags += wxT(" -dpart=") + m_strPart + wxT(" --erase --upload if=$(TARG)");
        }
        tout << itemlist[UCI_BURNFLAGS] << m_strBurnFlags << wxT("\n");

        /*
         * Set crurom utility.
         */
        if(m_strCrurom.IsEmpty()) {
            m_strCrurom = tooldir + wxT("crurom");
        }
        tout << itemlist[UCI_CRUROM] << m_strCrurom << wxT("\n");

        /*
         * Set ICCAVR include directory.
         */
        if(m_iCompiler == UC_ICCAVR) {
            tout << itemlist[UCI_AVRPATH] << m_strIccPath << wxT("\n");
        }

        /*
         * Set hardware definition.
         */
        if(m_strBoard.IsEmpty()) {
            if(m_iEthernet == UC_LANC111) {
                m_strBoard = wxT("-DETHERNUT2");
            }
            else if(m_iEthernet == UC_CS8900) {
                m_strBoard = wxT("-DULTRALITE");
            }
        }
        if(!m_strBoard.IsEmpty()) {
            tout << itemlist[UCI_HWDEF] << m_strBoard << wxT("\n");
        }
    }
    else {
        rc = false;
    }
    return rc;
}

bool CUserConf::IsDirty()
{
    return m_bDirty;
}

CUserConf::ucCompiler CUserConf::GetCompiler()
{
    return m_iCompiler;
}

CUserConf::ucTarget CUserConf::GetTarget()
{
    return m_iTarget;
}

CUserConf::ucAdapter CUserConf::GetAdapter()
{
    return m_iAdapter;
}

CUserConf::ucEthernet CUserConf::GetEthernet()
{
    return m_iEthernet;
}

wxString CUserConf::GetPort()
{
    if(m_strPort.IsEmpty() && m_iAdapter == UC_STK500) {
        return wxT("/dev/ttyS0");
    }
    return m_strPort;
}

wxString CUserConf::GetIccPath()
{
    return m_strIccPath;
}

wxString CUserConf::GetGccPath()
{
    return m_strGccPath;
}

void CUserConf::SetCompiler(CUserConf::ucCompiler val)
{
    m_bDirty = true;
    m_iCompiler = val;
}

void CUserConf::SetTarget(CUserConf::ucTarget val)
{
    m_bDirty = true;
    m_strPart = wxT("");
    m_iTarget = val;
}

void CUserConf::SetAdapter(CUserConf::ucAdapter val)
{
    m_bDirty = true;
    m_strBurnFlags = wxT("");
    m_iAdapter = val;
}

void CUserConf::SetEthernet(CUserConf::ucEthernet val)
{
    m_bDirty = true;
    m_strBoard = wxT("");
    m_iEthernet = val;
}

void CUserConf::SetPort(wxString val)
{
    m_bDirty = true;
    m_strPort = val;
}

