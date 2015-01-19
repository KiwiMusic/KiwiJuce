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

#include "AttributeNumberComponent.h"

namespace Kiwi
{
	AttributeNumberComponent::AttributeNumberComponent (sAttr attribute, Attr::sManager attrsManager,
														const double rangeMin,
														const double rangeMax,
														const double interval)
	: AttributeComponent (attribute, attrsManager)
	{
		addAndMakeVisible (m_numberbox);
		
		m_flonum = (m_attr->getStyle() == Attr::Style::NumberDouble);
		
		m_numberbox.setFloatingPointStyle(m_flonum);
		m_numberbox.setMinMax(rangeMin, rangeMax, 0.001);
		m_numberbox.addChangeListener(this);
	}
	
	AttributeNumberComponent::~AttributeNumberComponent()
	{
		m_numberbox.removeChangeListener(this);
	}
	
	void AttributeNumberComponent::setValue (const double /*newValue*/)
	{
	}
	
	double AttributeNumberComponent::getValue() const
	{
		return m_numberbox.getValue();
	}
	
	void AttributeNumberComponent::refresh()
	{
		ElemVector state;
		getAttributeValue(state);
		
		if (!state.empty())
		{
			m_numberbox.setValue((double)state[0], dontSendNotification);
		}
	}
	
	void AttributeNumberComponent::changeListenerCallback(ChangeBroadcaster*)
	{
		setAttributeValue({ (m_flonum ? m_numberbox.getValue() : m_numberbox.getLongValue()) });
	}
}