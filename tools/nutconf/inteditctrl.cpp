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
 * $Log: inteditctrl.cpp,v $
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "valuewindow.h"
#include "inteditctrl.h"

BEGIN_EVENT_TABLE(CIntEditCtrl, wxSpinCtrl)
    EVT_TEXT_ENTER(-1, CIntEditCtrl::OnEnter)
    EVT_KILL_FOCUS(CIntEditCtrl::OnKillFocus)
    END_EVENT_TABLE();

IMPLEMENT_CLASS(CIntEditCtrl, wxSpinCtrl);

CIntEditCtrl::CIntEditCtrl(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style)
: wxSpinCtrl(parent, id, wxEmptyString, pos, size, style, -32000, 32000, 0)
{
}

void CIntEditCtrl::OnEnter(wxCommandEvent & event)
{
    CValueWindow *parent = (CValueWindow *) GetParent();
    parent->EndEditing();
}

void CIntEditCtrl::OnKillFocus(wxFocusEvent & event)
{
    CValueWindow *parent = (CValueWindow *) GetParent();
    parent->EndEditing();
}
