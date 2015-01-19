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

#include "AttributeBoolComponent.h"

namespace Kiwi
{
	AttributeBoolComponent::AttributeBoolComponent (sAttr attribute, Attr::sManager attrsManager)
	: AttributeComponent (attribute, attrsManager)
	{
		addAndMakeVisible (button);
		button.setClickingTogglesState (false);
		button.addListener (this);
	}
	
	AttributeBoolComponent::~AttributeBoolComponent()
	{
		;
	}
	
	void AttributeBoolComponent::setState (const bool newState)
	{
		button.setToggleState (newState, dontSendNotification);
	}
	
	bool AttributeBoolComponent::getState() const
	{
		return button.getToggleState();
	}
	
	void AttributeBoolComponent::paint (Graphics& g)
	{
		AttributeComponent::paint (g);
		/*
		g.setColour (findColour (backgroundColourId));
		g.fillRect (button.getBounds());
		*/
		g.setColour(findColour (ComboBox::outlineColourId).withAlpha(0.6f));
		g.drawRect(button.getBounds());
	}
	
	void AttributeBoolComponent::refresh()
	{
		ElemVector state;
		getAttributeValue(state);
		bool val = false;
		
		if (!state.empty())
		{
			val = (long)state[0];
			button.setToggleState (val, dontSendNotification);
		}
	}
	
	void AttributeBoolComponent::buttonClicked (Button*)
	{
		setAttributeValue({ (long) !getState() });
	}	
}