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
 * $Log: conflictlist.cpp,v $
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "ids.h"
#include "conflictlist.h"

IMPLEMENT_CLASS(CConflictList, wxListCtrl);

CConflictList::CConflictList(wxWindow * parent, wxWindowID id, const wxPoint & pt, const wxSize & sz, long style):
wxListCtrl(parent, id, pt, sz, style)
{
    InsertColumn(0, "Item", wxLIST_FORMAT_LEFT, 200);
    InsertColumn(1, "Conflict", wxLIST_FORMAT_LEFT, 80);
    InsertColumn(2, "Property", wxLIST_FORMAT_LEFT, 200);

    m_contextMenu = new wxMenu;
    m_contextMenu->Append(ID_WHATS_THIS, wxT("&What's This?"));
}

CConflictList::~CConflictList()
{
    delete m_contextMenu;
}
