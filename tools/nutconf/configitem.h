#ifndef CONFIGITEM_H_
#define CONFIGITEM_H_

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
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:24  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/variant.h>

#include "nutcomponent.h"

enum nutComponentType {
    nutConfigTypeNone,
    nutFolder,
    nutLibrary,
    nutModule,
    nutOption
};

enum nutOptionFlavor {
    nutFlavorNone,
    nutFlavorBool,
    nutFlavorBoolData,
    nutFlavorData
};

enum nutOptionType {
    nutOptionTypeNone,
    nutInteger,
    nutBool,
    nutString,
    nutEnumerated
};

enum nutUIHint {
    nutHintNone,
    nutHintCheck,
    nutHintRadio
};

class CTreeItemData;
class CConfigTree;

class CConfigItem:public wxObject {
    DECLARE_CLASS(CConfigItem)
    friend class CNutConfDoc;
  public:
     CConfigItem();
     CConfigItem(CConfigItem * parent, NUTCOMPONENTOPTION * option);
     CConfigItem(CConfigItem * parent, NUTCOMPONENT * compo);

    wxTreeItemId GetTreeItem() const;
    void SetTreeItem(wxTreeItemId id);

    nutComponentType GetConfigType() const;

    void SetUIHint(nutUIHint hint);
    nutUIHint GetUIHint() const;

    bool IsEnabled() const;

    void SetValue(const wxVariant & value);
     wxVariant & GetValue();

    wxString GetName() const;
    wxString GetBriefDescription() const;
    wxString GetDescription() const;
    nutOptionFlavor GetOptionFlavor() const;
    nutOptionType GetOptionType() const;
    int GetEnumStrings(wxArrayString & arEnumStrings) const;
    wxString GetRequirementList() const;
    wxString GetProvisionList() const;


    void SetActive(bool ena);
    bool IsActive() const;

    bool UpdateTreeItem(CConfigTree & treeCtrl);

    bool CanEdit() const;
    wxWindow *CreateEditWindow(wxWindow * parent);

    void OnIconLeftDown(CConfigTree & treeCtrl);

    wxString GetDisplayValue() const;
    wxString StringValue() const;
    wxString GetFilename() const;
    wxString GetMacro() const;

    bool TransferDataToWindow(wxWindow * window);
    bool TransferDataFromWindow(wxWindow * window);

    bool HasBool() const;

    bool ViewHeaderFile();


  protected:
     CConfigItem * m_parent;
    wxTreeItemId m_itemId;
    NUTCOMPONENT *m_compo;
    NUTCOMPONENTOPTION *m_option;

    wxString m_name;
    nutComponentType m_configType;
    nutOptionType m_optionType;
    nutUIHint m_hint;
    wxVariant m_value;
};

#endif
