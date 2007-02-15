///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// Written by
//   José Antonio Hurtado - joseantonio.hurtado@gmail.com
//   Juan Antonio Ortega  - jortegalalmolda@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include <component.h>
#include <plugin.h>
#include <xrcconv.h>
#include <tinyxml.h>
#include <wx/tokenzr.h>
#include <map>

#ifdef __WX24__
	#define wxFIXED_MINSIZE wxADJUST_MINSIZE
#endif

class SpacerComponent : public ComponentBase
{
private:
	void AddSizeProperty(XrcToXfbFilter &filter, TiXmlElement *xrcObj)
	{
		TiXmlElement *sizeProp = xrcObj->FirstChildElement("size");
		if (sizeProp)
		{
			TiXmlText *xmlValue = sizeProp->FirstChild()->ToText();
			if (xmlValue)
			{
				wxString width = wxT("");
				wxString height = wxT("");
				wxStringTokenizer tkz(wxString(xmlValue->Value(),wxConvUTF8),wxT(","));
				if (tkz.HasMoreTokens())
				{
					width = tkz.GetNextToken();
					if (tkz.HasMoreTokens())
						height = tkz.GetNextToken();
				}
				filter.AddPropertyValue(_("width"),width);
				filter.AddPropertyValue(_("height"),height);
			}
		}
	}

public:
	void OnCreated( wxObject* wxobject, wxWindow* wxparent )
	{
		// Get parent sizer
		wxObject* parent = GetManager()->GetParent( wxobject );
		wxSizer* sizer = wxDynamicCast( parent, wxSizer );

		if ( NULL == sizer )
		{
			wxLogError( wxT("The parent of a SizerItem is either missing or not a wxSizer - this should not be possible!") );
			return;
		}

		// Get IObject to read property values
		IObject* obj = GetManager()->GetIObject( wxobject );

		sizer->Add(
			obj->GetPropertyAsInteger(_("width")),
			obj->GetPropertyAsInteger(_("height")),
			obj->GetPropertyAsInteger(_("proportion")),
			obj->GetPropertyAsInteger(_("flag")),
			obj->GetPropertyAsInteger(_("border")));
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("spacer"));

		// las propiedades width y height se mapean como size
		xrc.AddPropertyValue(_("size"),
			wxString::Format(_("%d,%d"),obj->GetPropertyAsInteger(_("width")),
			obj->GetPropertyAsInteger(_("height"))));

		xrc.AddProperty(_("proportion"), _("option"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("flag"),   _("flag"),   XRC_TYPE_BITLIST);
		xrc.AddProperty(_("border"), _("border"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("spacer"));

		// la propiedad "size" hay que descomponerla en weight y height
		AddSizeProperty(filter, xrcObj);

		filter.AddProperty(_("option"), _("proportion"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("flag"),   _("flag"),   XRC_TYPE_BITLIST);
		filter.AddProperty(_("border"), _("border"), XRC_TYPE_INTEGER);
		return filter.GetXfbObject();
	}
};

class SizerItemComponent : public ComponentBase
{
public:
	void OnCreated( wxObject* wxobject, wxWindow* wxparent )
	{
		// Get parent sizer
		wxObject* parent = GetManager()->GetParent( wxobject );
		wxSizer* sizer = wxDynamicCast( parent, wxSizer );

		if ( NULL == sizer )
		{
			wxLogError( wxT("The parent of a SizerItem is either missing or not a wxSizer - this should not be possible!") );
			return;
		}

		// Get child window
		wxObject* child = GetManager()->GetChild( wxobject, 0 );
		if ( NULL == child )
		{
			wxLogError( wxT("The SizerItem component has no child - this should not be possible!") );
			return;
		}

		// Get IObject for property access
		IObject* obj = GetManager()->GetIObject( wxobject );

		// Add the child ( window or sizer ) to the sizer
		wxWindow* windowChild = wxDynamicCast( child, wxWindow );
		wxSizer* sizerChild = wxDynamicCast( child, wxSizer );

		if ( windowChild != NULL )
		{
			sizer->Add( windowChild,
				obj->GetPropertyAsInteger(_("proportion")),
				obj->GetPropertyAsInteger(_("flag")),
				obj->GetPropertyAsInteger(_("border")));
		}
		else if ( sizerChild != NULL )
		{
			sizer->Add( sizerChild,
				obj->GetPropertyAsInteger(_("proportion")),
				obj->GetPropertyAsInteger(_("flag")),
				obj->GetPropertyAsInteger(_("border")));
		}
		else
		{
			wxLogError( wxT("The SizerItem component's child is not a wxWindow or a wxSizer - this should not be possible!") );
		}
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("sizeritem"));
		xrc.AddProperty(_("proportion"), _("option"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("flag"),   _("flag"),   XRC_TYPE_BITLIST);
		xrc.AddProperty(_("border"), _("border"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("sizeritem"));
		filter.AddProperty(_("option"), _("proportion"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("flag"),   _("flag"),   XRC_TYPE_BITLIST);
		filter.AddProperty(_("border"), _("border"), XRC_TYPE_INTEGER);
		return filter.GetXfbObject();
	}
};

class BoxSizerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxBoxSizer(obj->GetPropertyAsInteger(_("orient")));
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxBoxSizer"));
		xrc.AddProperty(_("orient"), _("orient"), XRC_TYPE_TEXT);
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("wxBoxSizer"));
		filter.AddProperty(_("orient"),_("orient"),XRC_TYPE_TEXT);
		return filter.GetXfbObject();
	}
};

class StaticBoxSizerComponent : public ComponentBase
{
public:
	int m_count;
	StaticBoxSizerComponent()
	{
		m_count = 0;
	}
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		m_count++;
		wxStaticBox* box = new wxStaticBox((wxWindow *)parent, -1,
			obj->GetPropertyAsString(_("label")));

		wxStaticBoxSizer* sizer = new wxStaticBoxSizer(box,
			obj->GetPropertyAsInteger(_("orient")));

		return sizer;
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxStaticBoxSizer"));
		xrc.AddProperty(_("orient"), _("orient"), XRC_TYPE_TEXT);
		xrc.AddProperty(_("label"), _("label"), XRC_TYPE_TEXT);
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("wxStaticBoxSizer"));
		filter.AddProperty(_("orient"),_("orient"),XRC_TYPE_TEXT);
		filter.AddProperty(_("label"),_("label"),XRC_TYPE_TEXT);
		return filter.GetXfbObject();
	}
};

class GridSizerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxGridSizer(
			obj->GetPropertyAsInteger(_("rows")),
			obj->GetPropertyAsInteger(_("cols")),
			obj->GetPropertyAsInteger(_("vgap")),
			obj->GetPropertyAsInteger(_("hgap")));
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxGridSizer"));
		xrc.AddProperty(_("rows"), _("rows"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("cols"), _("cols"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("vgap"), _("vgap"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("hgap"), _("hgap"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("wxGridSizer"));
		filter.AddProperty(_("rows"), _("rows"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("cols"), _("cols"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("vgap"), _("vgap"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("hgap"), _("hgap"), XRC_TYPE_INTEGER);
		return filter.GetXfbObject();
	}
};

class FlexGridSizerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxFlexGridSizer *sizer = new wxFlexGridSizer(
			obj->GetPropertyAsInteger(_("rows")),
			obj->GetPropertyAsInteger(_("cols")),
			obj->GetPropertyAsInteger(_("vgap")),
			obj->GetPropertyAsInteger(_("hgap")));

		wxArrayInt gcols, grows;
		gcols = obj->GetPropertyAsArrayInt(_("growablecols"));
		grows = obj->GetPropertyAsArrayInt(_("growablerows"));

		unsigned int i;
		for (i=0; i < gcols.GetCount() ; i++)
			sizer->AddGrowableCol(gcols[i]);

		for (i=0; i < grows.GetCount() ; i++)
			sizer->AddGrowableRow(grows[i]);

		sizer->SetFlexibleDirection( obj->GetPropertyAsInteger(_("flexible_direction")) );
		sizer->SetNonFlexibleGrowMode( (wxFlexSizerGrowMode )obj->GetPropertyAsInteger(_("non_flexible_grow_mode")) );

		return sizer;
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxFlexGridSizer"));
		xrc.AddProperty(_("rows"), _("rows"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("cols"), _("cols"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("vgap"), _("vgap"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("hgap"), _("hgap"), XRC_TYPE_INTEGER);

		xrc.AddPropertyValue(_("growablecols"), obj->GetPropertyAsString(_("growablecols")));
		xrc.AddPropertyValue(_("growablerows"), obj->GetPropertyAsString(_("growablerows")));
		return xrc.GetXrcObject();
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		XrcToXfbFilter filter(xrcObj, _("wxFlexGridSizer"));
		filter.AddProperty(_("rows"), _("rows"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("cols"), _("cols"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("vgap"), _("vgap"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("hgap"), _("hgap"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("growablecols"),_("growablecols"),XRC_TYPE_TEXT);
		filter.AddProperty(_("growablerows"),_("growablerows"),XRC_TYPE_TEXT);
		return filter.GetXfbObject();
	}
};

class StdDialogButtonSizerComponent : public ComponentBase
{
private:
	void AddXRCButton( TiXmlElement* sizer, const std::string& id, const std::string& label )
	{
		TiXmlElement* button = new TiXmlElement( "object" );
		button->SetAttribute( "class", "button" );

		TiXmlElement* flag = new TiXmlElement( "flag" );
		flag->LinkEndChild( new TiXmlText( "wxALIGN_CENTER_HORIZONTAL|wxALL" ) );
		button->LinkEndChild( flag );

		TiXmlElement* border = new TiXmlElement( "border" );
		border->LinkEndChild( new TiXmlText( "5" ) );
		button->LinkEndChild( border );

		TiXmlElement* wxbutton = new TiXmlElement( "object" );
		wxbutton->SetAttribute( "class", "wxButton" );
		wxbutton->SetAttribute( "name", id );

		TiXmlElement* labelEl = new TiXmlElement( "label" );
		labelEl->LinkEndChild( new TiXmlText( label ) );
		wxbutton->LinkEndChild( labelEl );

		button->LinkEndChild( wxbutton );

		sizer->LinkEndChild( button );
	}

public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxStdDialogButtonSizer* sizer =  new wxStdDialogButtonSizer();
		if ( obj->GetPropertyAsInteger( _("OK") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_OK ) );
		}
		if ( obj->GetPropertyAsInteger( _("Yes") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_YES ) );
		}
		if ( obj->GetPropertyAsInteger( _("Save") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_SAVE ) );
		}
		if ( obj->GetPropertyAsInteger( _("Apply") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_APPLY ) );
		}
		if ( obj->GetPropertyAsInteger( _("No") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_NO ) );
		}
		if ( obj->GetPropertyAsInteger( _("Cancel") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_CANCEL ) );
		}
		if ( obj->GetPropertyAsInteger( _("Help") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_HELP ) );
		}
		if ( obj->GetPropertyAsInteger( _("ContextHelp") ) )
		{
			sizer->AddButton( new wxButton( (wxWindow*)parent, wxID_CONTEXT_HELP ) );
		}
		sizer->Realize();
		return sizer;
	}

	TiXmlElement* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxStdDialogButtonSizer"));
		TiXmlElement* sizer = xrc.GetXrcObject();

		if ( obj->GetPropertyAsInteger( _("OK") ) )
		{
			AddXRCButton( sizer, "wxID_OK", "&OK" );
		}
		if ( obj->GetPropertyAsInteger( _("Yes") ) )
		{
			AddXRCButton( sizer, "wxID_YES", "&Yes" );
		}
		if ( obj->GetPropertyAsInteger( _("Save") ) )
		{
			AddXRCButton( sizer, "wxID_SAVE", "&Save" );
		}
		if ( obj->GetPropertyAsInteger( _("Apply") ) )
		{
			AddXRCButton( sizer, "wxID_APPLY", "&Apply" );
		}
		if ( obj->GetPropertyAsInteger( _("No") ) )
		{
			AddXRCButton( sizer, "wxID_NO", "&No" );
		}
		if ( obj->GetPropertyAsInteger( _("Cancel") ) )
		{
			AddXRCButton( sizer, "wxID_CANCEL", "&Cancel" );
		}
		if ( obj->GetPropertyAsInteger( _("Help") ) )
		{
			AddXRCButton( sizer, "wxID_HELP", "&Help" );
		}
		if ( obj->GetPropertyAsInteger( _("ContextHelp") ) )
		{
			AddXRCButton( sizer, "wxID_CONTEXT_HELP", "" );
		}

		return sizer;
	}

	TiXmlElement* ImportFromXrc(TiXmlElement *xrcObj)
	{
		std::map< wxString, wxString > buttons;
		buttons[ _("OK") ] 			= wxT("0");
		buttons[ _("Yes") ] 		= wxT("0");
		buttons[ _("Save") ] 		= wxT("0");
		buttons[ _("Apply") ] 		= wxT("0");
		buttons[ _("No") ] 			= wxT("0");
		buttons[ _("Cancel") ] 		= wxT("0");
		buttons[ _("Help") ] 		= wxT("0");
		buttons[ _("ContextHelp") ] = wxT("0");

		XrcToXfbFilter filter(xrcObj, _("wxStdDialogButtonSizer"));

		TiXmlElement* button = xrcObj->FirstChildElement( "object" );
		for (  ; button != 0; button = button->NextSiblingElement( "object" ) )
		{
			if ( std::string("button") != button->Attribute( "class" ) )
			{
				continue;
			}

			TiXmlElement* wxbutton = button->FirstChildElement( "object" );
			if ( std::string("wxButton") != wxbutton->Attribute( "class" ) )
			{
				continue;
			}

			std::string name = wxbutton->Attribute( "name" );

			if ( name == "wxID_OK" )
			{
				buttons[ _("OK") ] = wxT("1");
			}
			else if ( name == "wxID_YES" )
			{
				buttons[ _("Yes") ] = wxT("1");
			}
			else if ( name == "wxID_SAVE" )
			{
				buttons[ _("Save") ] = wxT("1");
			}
			else if ( name == "wxID_APPLY" )
			{
				buttons[ _("Apply") ] = wxT("1");
			}
			else if ( name == "wxID_NO" )
			{
				buttons[ _("No") ] = wxT("1");
			}
			else if ( name == "wxID_CANCEL" )
			{
				buttons[ _("Cancel") ] = wxT("1");
			}
			else if ( name == "wxID_HELP" )
			{
				buttons[ _("Help") ] = wxT("1");
			}
			else if ( name == "wxID_CONTEXT_HELP" )
			{
				buttons[ _("ContextHelp") ] = wxT("1");
			}
		}

		std::map< wxString, wxString >::iterator prop;
		for ( prop = buttons.begin(); prop != buttons.end(); ++prop )
		{
			filter.AddPropertyValue( prop->first, prop->second );
		}

		return filter.GetXfbObject();
	}
};

///////////////////////////////////////////////////////////////////////////////

BEGIN_LIBRARY()
ABSTRACT_COMPONENT("spacer",SpacerComponent)
ABSTRACT_COMPONENT("sizeritem",SizerItemComponent)

SIZER_COMPONENT("wxBoxSizer",BoxSizerComponent)
SIZER_COMPONENT("wxStaticBoxSizer",StaticBoxSizerComponent)
SIZER_COMPONENT("wxGridSizer",GridSizerComponent)
SIZER_COMPONENT("wxFlexGridSizer",FlexGridSizerComponent)
SIZER_COMPONENT("wxStdDialogButtonSizer",StdDialogButtonSizerComponent)

// wxBoxSizer
MACRO(wxHORIZONTAL)
MACRO(wxVERTICAL)

// wxFlexGridSizer
MACRO(wxBOTH)
MACRO(wxFLEX_GROWMODE_NONE)
MACRO(wxFLEX_GROWMODE_SPECIFIED)
MACRO(wxFLEX_GROWMODE_ALL)


// Add
MACRO(wxALL)
MACRO(wxLEFT)
MACRO(wxRIGHT)
MACRO(wxTOP)
MACRO(wxBOTTOM)
MACRO(wxEXPAND)
MACRO(wxALIGN_BOTTOM)
MACRO(wxALIGN_CENTER)
MACRO(wxALIGN_CENTER_HORIZONTAL)
MACRO(wxALIGN_CENTER_VERTICAL)
MACRO(wxSHAPED)
MACRO(wxFIXED_MINSIZE)

SYNONYMOUS(wxGROW, wxEXPAND)


END_LIBRARY()

