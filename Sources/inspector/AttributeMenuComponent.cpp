/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "AttributeMenuComponent.h"

namespace Kiwi
{
	AttributeMenuComponent::AttributeMenuComponent (sAttr attribute, Attr::sManager attrsManager)
	: AttributeComponent (attribute, attrsManager)
	{
	}
	
	AttributeMenuComponent::~AttributeMenuComponent()
	{
	}
	
	//==============================================================================
	void AttributeMenuComponent::createComboBox()
	{
		addAndMakeVisible (m_combo_box);
		
		ElemVector elemValues;
		m_attr->getEnumValues(elemValues);
		
		for (int i = 0; i < elemValues.size(); ++i)
		{
			m_combo_box.addItem(toString(elemValues[i]), i + 1);
		}
		
		m_combo_box.setEditableText (false);
	}
	
	//==============================================================================
	void AttributeMenuComponent::refresh()
	{
		if (! m_combo_box.isVisible())
		{
			createComboBox();
			m_combo_box.addListener (this);
		}
		
		ElemVector state;
		getAttributeValue(state);
		
		if (!state.empty())
		{
			m_combo_box.setSelectedId ((long)state[0] + 1, dontSendNotification);
		}
	}
	
	void AttributeMenuComponent::comboBoxChanged (ComboBox*)
	{
		const int newIndex = m_combo_box.getSelectedId() - 1;
		setAttributeValue({ (long) newIndex });
	}
}